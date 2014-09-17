#include <stdio.h>
#include <stdlib.h>
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

float clock_time_of_day (){
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
  return t;
}

float clock_pid(){ 
  uint64_t start = rdtsc(); 
  int hi = getpid(); 
  uint64_t stop = rdtsc(); 
  unsigned int lo = start & (((uint64_t)2 << 32) - 1);
  unsigned int lo2 = stop & (((int64_t)2 << 32) - 1); 
  int cycles = lo2 - lo; 
  float t = ((float) (cycles));
  float factor = 3591338000.0  * .000001;
  t = t / factor; 
  return t;
}

float clock_ppid(){ 
  uint64_t start = rdtsc(); 
  int hi = getppid(); 
  uint64_t stop = rdtsc(); 
  unsigned int lo = start & (((uint64_t)2 << 32) - 1);
  unsigned int lo2 = stop & (((int64_t)2 << 32) - 1); 
  int cycles = lo2 - lo; 
  float t = ((float) (cycles));
  float factor = 3591338000.0  * .000001;
  t = t / factor; 
  return t;
}

float * simple_time(){ 
  float * retArray = malloc(sizeof(float) * 3);
  int i;
  float sum = 0;
  for (i =0; i < 10; i++){ 
    float res;
    res=clock_ppid(); 
    printf("miliseconds for the call clockppid %f\n", res);
    sum+=res;
  }
  float avg1 = sum/10;
  retArray[0] = avg1;  
  
  sum = 0; 
  for (i = 0; i < 10; i++){ 
    float res;
    res = clock_pid();
    printf("miliseconds for the call clockpid %f\n", res);
    sum+=res;
  } 
  float avg2 = sum/10; 
  retArray[1] = avg2;
  
  sum = 0;
  for (i = 0; i < 10; i++){ 
    float res;
    res = clock_time_of_day();
    printf("miliseconds for the call clocktimeofday%f\n", res);
    sum+=res;
  } 
  float avg3 = sum/10; 
  retArray[2] = avg2;
  
  return retArray; 
  
}

int main (int agrc, char ** argv){ 
  float * a = simple_time();
  int i;
  for (i =0; i < 3; i++) printf("%f\n", a[i]);
} 
