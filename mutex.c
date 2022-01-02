#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 5;

static int sum;
pthread_mutex_t mutex;

void increment(void) {
  for (int i = 0; i < 100000; ++i) {
    int rv;
    if ((rv = pthread_mutex_lock(&mutex)) != 0) {
      fprintf(stderr, "mutex lock failed: %d\n", rv);
      pthread_exit((void*)1);
    }
    sum++;
    if ((rv = pthread_mutex_unlock(&mutex)) != 0) {
      fprintf(stderr, "mutex unlock failed: %d\n", rv);
      pthread_exit((void*)1);
    }
  }
}

int main(void) {
  int rv;
  pthread_t t[N];

  if ((rv = pthread_mutex_init(&mutex, NULL)) != 0) {
    fprintf(stderr, "mutex_init failed with %d\n", rv);
    exit(1);
  }

  for (int i = 0; i < N; ++i) {
    if ((rv = pthread_create(&t[i], NULL, (void*)increment, NULL)) != 0) {
      fprintf(stderr, "pthread_create failed: %d", rv);
      exit(1);
    }
  }

  for (int i = 0; i < N; ++i) {
    if ((rv = pthread_join(t[i], NULL)) != 0) {
      fprintf(stderr, "pthread_join failed: %d", rv);
      exit(1);
    }
  }

  fprintf(stderr, "sum is %d\n", sum);
  if ((rv = pthread_mutex_destroy(&mutex)) != 0) {
    fprintf(stderr, "mutex destroy failed: %d\n", rv);
  }
}
