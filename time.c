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
   : "=a"(lo), "=d"(hi) 
   :                    
   : "%ebx", "%ecx");     
  return ((((uint64_t)hi) << 32) | lo);
}

void simple_time(){ 
  struct timeval begin;
  uint64_t start = rdtsc(); 
  gettimeofday(&begin, NULL);
  uint64_t stop = rdtsc(); 
  unsigned int lo = start & (((uint64_t)2 << 32) - 1);
  unsigned int lo2 = stop & (((int64_t)2 << 32) - 1); 
  int cycles = lo2 - lo; 
  float t = ((float) (cycles));
  float factor = 3591338000.0  * .000001;
  t = t / factor; 
  printf("the start is %llu\n", start);
  printf("the stop is %llu\n", stop);
  printf("cycles : %d\n", cycles); 
  printf("time in microseconds : %f\n", t);
}

int main (int agrc, char ** argv){ 
  int i;
  for (i =0; i < 10; i++) simple_time(); 
} 
