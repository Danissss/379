/*
# ------------------------------------------------------------
# Compilation:  gcc raceB.c -lpthread -o raceB
#
# This program tests for a possible race condition
# ------------------------------------------------------------
*/
#define _REENTRANT

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREAD 10
#define START   100
#define COUNT   NTHREAD

pthread_t  TID[NTHREAD];
// ------------------------------
void delay (int x)                                 // milliseconds
{
  struct timespec interval;

  interval.tv_sec =  (long) x / 1000;              // seconds 
  interval.tv_nsec = (long) ((x % 1000) * 1E6);    // nanoseconds
  if (nanosleep(&interval, NULL) < 0)
      printf("warning: delay: %s\n", strerror(errno));
}
// ------------------------------
void *athread(void *arg)
{
  long  threadNum= (long) arg; 

  printf("thread: number= %ld, ID= %lu \n", threadNum, TID[threadNum-START]);
  pthread_exit(NULL);
}
// ------------------------------
int main(int argc, char *argv[])
{
  int       rval;
  long      idx;
  pthread_t ntid;
  
  for (idx = START; idx < START + COUNT; idx++) {
      rval = pthread_create(&ntid, NULL, athread, (void *)idx);
      if (rval) {
          fprintf(stderr, "pthread_create: %s\n", strerror(rval));
          exit(1);
      }

      delay(10);                       // simulate doing something
      TID[idx - START]= ntid;
  }

  for (idx = 0; idx < COUNT; idx++) {
      rval = pthread_join(TID[idx], NULL);
      if (rval) {
          fprintf(stderr, "\n** pthread_join: %s\n", strerror(rval));
          exit(1);
      }
  }
} 
