/*
# ------------------------------------------------------------
# Compilation:  gcc raceA.c -lpthread -o raceA
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


int NTER;

pthread_t  TID[NTER];
// ------------------------------
void *athread(void *arg)
{
  long  threadNum= (long) arg; 

  TID[threadNum - START] = pthread_self();
  pthread_t tid;
  tid = pthread_self();
  printf("pthread_self: number= %ld, ID= %lu \n", threadNum,tid);
  printf("thread: number= %ld, ID= %lu \n", threadNum, TID[threadNum-START]);
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
  }

  for (idx = 0; idx < COUNT; idx++) {
      rval = pthread_join(TID[idx], NULL);
      if (rval) {
          fprintf(stderr, "\n** pthread_join: %s\n", strerror(rval));
          exit(1);
      }
  }
} 
