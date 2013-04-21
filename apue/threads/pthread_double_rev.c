#include "../apue.h"
#include <pthread.h>

#define NHASH 29
#define HASH(fp) (((unsigned long) fp) % NHASH)

struct foo {
  int f_count;
  pthread_mutex_t f_lock;
  struct foo *f_next;
  int f_id;
  /* other stuff */
};

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo *
foo_alloc(void)
{
  struct foo *fp;
  int idx;

  if ((fp = malloc(sizeof(struct foo))) == NULL)
    return NULL;

  fp->f_count = 1;
  if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
    free(fp);
    return NULL;
  }

  pthread_mutex_lock(&hashlock);
  idx = HASH(fp);
  fp->f_next = fh[idx];
  fh[idx] = fp;
  pthread_mutex_lock(&fp->f_lock);
  pthread_mutex_unlock(&hashlock);
  /* continue initialization */
  pthread_mutex_unlock(&fp->f_lock);

  return(fp);
}

void
foo_hold(struct foo *fp)
{
  pthread_mutex_lock(&fp->f_lock);
  fp->f_count++;
  pthread_mutex_unlock(&fp->f_lock);
}

struct foo *
foo_find(int id)
{
  struct foo *fp;
  int idx;

  pthread_mutex_lock(&hashlock);

  for (idx=0; idx<NHASH; idx++) {
    for (fp = fh[idx]; fp != NULL; fp = fp->f_next) {
      if (fp->f_id == id) {
        fp->f_count++;
        pthread_mutex_unlock(&hashlock);
        return(fp);
      }
    }
  }

  pthread_mutex_unlock(&hashlock);
  return(NULL);
}

void
foo_rele(struct foo *fp)
{
  struct foo *temp;
  int idx;

  pthread_mutex_lock(&hashlock);
  if (fp->f_count == 1) {
    fp->f_count--;
    idx = HASH(fp);
    temp = fh[idx];

    if (temp == fp) {
      fh[idx] = fp->f_next;
    } else {
      while (temp->f_next != fp)
        temp = temp->f_next;
      temp->f_next = fp->f_next;
    }

    pthread_mutex_unlock(&hashlock);
    pthread_mutex_destroy(&fp->f_lock);
    free(fp);
  } else {
    fp->f_count--;
    pthread_mutex_unlock(&hashlock);
  }
}

int
main(void)
{
  struct foo *fp, *temp;

  fp = foo_alloc();
  printf("fp->f_count = %d\n", fp->f_count);

  fp->f_id = 1337;
  temp = foo_find(1337);
  printf("fp->f_count = %d\n", fp->f_count);

  foo_rele(fp);
  printf("fp->f_count = %d\n", fp->f_count);

  foo_rele(fp);

  exit(0);
}
