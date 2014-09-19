#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <stdint.h>
#include <sys/time.h> 
#include "time.h"
#include <inttypes.h>
#include <fcntl.h>
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
    printf("microseconds for the call clockppid %f\n", res);
    sum+=res;
  }
  float avg1 = sum/10;
  retArray[0] = avg1;  
  
  sum = 0; 
  for (i = 0; i < 10; i++){ 
    float res;
    res = clock_pid();
    printf("microseconds for the call clockpid %f\n", res);
    sum+=res;
  } 
  float avg2 = sum/10; 
  retArray[1] = avg2;
  
  sum = 0;
  for (i = 0; i < 10; i++){ 
    float res;
    res = clock_time_of_day();
    printf("microseconds for the call clocktimeofday%f\n", res);
    sum+=res;
  } 
  float avg3 = sum/10; 
  retArray[2] = avg2;
  
  return retArray; 
}

float read_time(char * file){ 
  int fd,rd;
  struct stat *buf;
  buf = malloc(sizeof(struct stat));
  fd=(int) open(file,O_RDONLY); //get file descriptor
  if(fd>1){
    stat(file, buf);
    int size = buf->st_size;
    free(buf); 
    char buff[size];
    uint64_t start = rdtsc(); 
    rd=read(fd,buff,size);
    uint64_t stop = rdtsc(); 
    unsigned int lo = start & (((uint64_t)2 << 32) - 1);
    unsigned int lo2 = stop & (((int64_t)2 << 32) - 1); 
    int cycles = lo2 - lo; 
    float t = ((float) (cycles));
    float factor = 3591338000.0  * .000001;
    t = t / factor; 
    return t;
  }else{ 
    printf("Error with open");
    free(buf);
    return -1;
  } 
} 

float *  mmap_time(char * file){ 
  struct stat *buf;
  int fd;
  float * result = (float *) malloc((sizeof(float) * 2) + 1); 
  fd=(int) open(file,O_RDONLY); //get file descriptor
  buf = malloc(sizeof(struct stat));
  stat(file, buf);
  size_t size = buf->st_size;
  free(buf); 
  
  uint64_t start = rdtsc(); 
  char * m = mmap(0, size, PROT_READ,MAP_SHARED,fd,0);
  uint64_t stop = rdtsc(); 
  
  unsigned int lo = start & (((uint64_t)2 << 32) - 1);
  unsigned int lo2 = stop & (((int64_t)2 << 32) - 1); 
  int cycles = lo2 - lo; 
  float t = ((float) (cycles));
  float factor = 3591338000.0  * .000001;
  t = t / factor; 
  result[0] = t; 

  char * destination = (char *) malloc(size + 1);
  uint64_t start1 = rdtsc(); 
  memcpy(destination, m, size);   
  uint64_t stop1 = rdtsc();
  
  unsigned int lo1 = start1 & (((uint64_t)2 << 32) - 1);
  unsigned int lo3 = stop1 & (((int64_t)2 << 32) - 1); 
  int cycles1 = lo3 - lo1; 
  float tz = ((float) (cycles1));
  float fact = 3591338000.0  * .000001;
  tz = tz / fact; 
  result[1] = tz;

  if (m == MAP_FAILED) {
      perror ("mmap");
      return result;
  }

  if (close (fd) == -1) {
      perror ("close");
      return result;
  }
  int len;
  int yo= size;
  for (len = 0; len < yo; len++)
    putchar (m[len]);
   

  if (munmap (m, size) == -1) {
      perror ("munmap");
      return result;
  }
  return result; 
} 

int main (int agrc, char ** argv){ 
  //float * a = simple_time();
  //int i;
  //for (i =0; i < 3; i++) printf("average time is %f\n", a[i]);
  /*char * file = argv[1];
  for (i = 0; i < 50; i++){ 
    float time = read_time(file); 
    printf("the time that it took to took to read %f\n", time); 
  }*/
  char * file = argv[1];
  float *  mmap = mmap_time(file);    
} 
