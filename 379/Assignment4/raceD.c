/*
# ------------------------------------------------------------
# Compilation:  gcc raceD.c -lpthread -o raceD
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
// ------------------------------
void *athread(void *arg)
{
  int  threadNum=  *((int *) arg); 
  TID[threadNum-START] = pthread_self();

  printf("thread: number= %d, ID= %lu \n", threadNum, TID[threadNum-START]);
  pthread_exit(NULL);
}
// ------------------------------
int main(int argc, char *argv[])
{
  int       rval;
  int       idx;
  pthread_t ntid;

  for (idx = START; idx < START + COUNT; idx++) {
      rval = pthread_create(&ntid, NULL, athread, (void *) &idx);
      if (rval) {
          fprintf(stderr, "pthread_create: %s\n", strerror(rval));
          exit(1);
      }
  }

  sleep(1);
  
  for (idx = 0; idx < COUNT; idx++) {
      rval = pthread_join(TID[idx], NULL);
      if (rval) {
          fprintf(stderr, "\n** pthread_join: %s\n", strerror(rval));
          exit(1);
      }
  }
} 
