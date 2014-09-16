#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#include <stdint.h>
#include <sys/time.h> 
#include "time.h"
#include <inttypes.h>

inline uint64_t rdtsc() {
  unsigned int lo, hi;
  __asm__  __volatile__(
     "cpuid \n"
     "rdtsc" 
   : "=a"(lo), "=d"(hi) /* outputs */
   :                    /* inputs */
   : "%ebx", "%ecx");     /* clobbers*/
  return ((((uint64_t)hi) << 32) | lo);
}

void simple_time(){ 
  struct timeval begin;
  uint64_t start = rdtsc(); 
  //int pid = getppid(); 
  gettimeofday(&begin, NULL);
  uint64_t stop = rdtsc(); 
  uint64_t cycles = (uint64_t)stop - start;
  unsigned long long ans = cycles/3591338000;
  //printf("The process id is %d\n", pid);
  printf("the start is %llu\n", start);
  printf("the stop is %llu\n", stop);
  printf("The time it took to execute is %llu\n", ans);
  printf("%" PRIu64 "\n", cycles); 
}

int main (int agrc, char ** argv){ 
  int i;
  for (i =0; i < 10; i++) simple_time(); 
} 
