#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#include "time.h"

inline unsigned long long rdtsc() {
  unsigned int lo, hi;
  asm volatile (
     "cpuid \n"
     "rdtsc" 
   : "=a"(lo), "=d"(hi) /* outputs */
   :                    /* inputs */
   : "%ebx", "%ecx");     /* clobbers*/
  return (((unsigned long long)hi) << 32) | lo;
}

void simple_time(){ 
  unsigned long long start = rdtsc(); 
  int pid = getpid(); 
  unsigned long long stop = rdtsc(); 
  unsigned long long cycles = start - stop;
  printf("The process id is %d\n", pid);
  printf("The time it took to execute is %llu\n", cycles/3591338000);
}

int main (int agrc, char ** argv){ 
  int i;
  for (i =0; i < 10; i++) simple_time(); 
} 
