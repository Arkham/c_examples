#include "../apue.h"
#include <pthread.h>

struct foo {
  int f_count;
  pthread_mutex_t f_lock;
  /* more */
};

struct foo *
foo_alloc(void)
{
  struct foo *fp;

  if ((fp = malloc(sizeof(struct foo))) == NULL)
    return(NULL);

  fp->f_count = 1;
  if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
    free(fp);
    return(NULL);
  }

  return(fp);
}

void
foo_hold(struct foo *fp)
{
  pthread_mutex_lock(&fp->f_lock);
  fp->f_count++;
  pthread_mutex_unlock(&fp->f_lock);
}

void
foo_rele(struct foo *fp)
{
  pthread_mutex_lock(&fp->f_lock);
  if (--fp->f_count == 0) {
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_destroy(&fp->f_lock);
    free(fp);
  } else {
    pthread_mutex_unlock(&fp->f_lock);
  }
}

#define LENGTH 1024

void *
thr_fn(void *arg)
{
  struct foo *fp = arg;

  sleep(1);
  foo_hold(fp);
  pthread_exit((void *) 0);
}

int
main(void)
{
  int i, err;
  void *tret;
  pthread_t tids[LENGTH];
  struct foo *fp;

  fp = foo_alloc();

  for (i=0; i<LENGTH; i++) {
    if ((err = pthread_create(tids+i, NULL, thr_fn, (void *) fp)) != 0)
      continue;
  }

  for (i=0; i<LENGTH; i++) {
    if ((err = pthread_join(tids[i], &tret)) != 0)
      continue;
  }

  printf("count = %d\n", fp->f_count);

  foo_rele(fp);
  exit(0);
}
