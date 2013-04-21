#include "../apue.h"
#include <pthread.h>

pthread_t ntid;

void
printids(const char *s)
{
  pid_t pid;
  pthread_t tid;

  pid = getpid();
  tid = pthread_self();
  printf("%s pid %u tid %lu (0x%lx)\n", s, (unsigned int) pid,
      (unsigned long) tid, (unsigned long) tid);
}

void *
thr_fn(void *arg)
{
  printids("new thread:  ");
  return((void *) 0);
}

int
main(void)
{
  int err;

  if ( (err = pthread_create(&ntid, NULL, thr_fn, NULL)) < 0 )
    err_sys("pthread_create error");

  printids("main thread: ");
  sleep(1);
  exit(0);
}
