#ifndef HEADER_FILE
#define HEADER_FILE 
uint64_t rdtsc();
float * simple_time();
float clock_pid();
float clock_ppid();
float clock_time_of_day();
float read_time(char *);
float * mmap_time(char *); 
#endif
