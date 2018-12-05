/*
# ------------------------------------------------------------
# Compilation:  gcc raceC.c -lpthread -o raceC
#
# This program tests for a possible race condition.
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

pthread_t        TID[NTHREAD];
pthread_mutex_t  create_mutex;
// ------------------------------
void mutex_init (pthread_mutex_t* mutex)
{
    int rval= pthread_mutex_init(mutex, NULL);
    if (rval) {fprintf(stderr, "mutex_init: %s\n",strerror(rval)); exit(1); }
}    

void mutex_lock (pthread_mutex_t* mutex)
{
    int rval= pthread_mutex_lock(mutex);
    if (rval) {fprintf(stderr, "mutex_lock: %s\n",strerror(rval)); exit(1); }
}    

void mutex_unlock (pthread_mutex_t* mutex)
{
    int rval= pthread_mutex_unlock(mutex);
    if (rval) {fprintf(stderr, "mutex_unlock: %s\n",strerror(rval)); exit(1);}
}
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
  TID[threadNum-START] = pthread_self();

  printf("thread: number= %ld, ID= %lu \n", threadNum, TID[threadNum-START]);
  
  delay(10);	  	       	   //simulate doing something
  
  mutex_unlock (&create_mutex);
  pthread_exit(NULL);
}
// ------------------------------
int main(int argc, char *argv[])
{
  int       rval;
  long      idx;
  pthread_t ntid;

  mutex_init(&create_mutex);

  for (idx = START; idx < START + COUNT; idx++) {
      mutex_lock(&create_mutex);
      rval = pthread_create(&ntid, NULL, athread, (void *)idx);
      if (rval) {
          fprintf(stderr, "pthread_create: %s\n", strerror(rval));
          exit(1);
      }
  }

  for (idx = 0; idx < COUNT; idx++) {
      printf("TID[idx]: %p\n",TID[idx]);
      rval = pthread_join(TID[idx], NULL);
      if (rval) {
          fprintf(stderr, "\n** pthread_join: %s\n", strerror(rval));
          exit(1);
      }
  }
} 
