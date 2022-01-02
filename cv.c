#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define N 5
#define NUM_LOOPS 20000

struct buffer {
  int arr[N];
  int size;
} buf;

pthread_mutex_t mutex;
pthread_cond_t buf_empty;
pthread_cond_t buf_full;

void producer(void) {
  for (int i = 0; i < NUM_LOOPS; ++i) {
    assert(pthread_mutex_lock(&mutex) == 0);
    while (buf.size == 5) {
      assert(pthread_cond_wait(&buf_empty, &mutex) == 0);
    }

    for (int i = buf.size; i < N; ++i) {
      buf.arr[i] = i;
      fprintf(stderr, "Producer placing %d in index %d\n", buf.arr[i], i);
    }
    buf.size = N;

    fprintf(stderr, "Producer waiting for empty buffer...\n");
    assert(pthread_cond_signal(&buf_full) == 0);
    assert(pthread_mutex_unlock(&mutex) == 0);
  }
}

void consumer(void) {
  for (int i = 0; i < NUM_LOOPS; ++i) {
    assert(pthread_mutex_lock(&mutex) == 0);
    while (buf.size == 0) { // empty
      assert(pthread_cond_wait(&buf_full, &mutex) == 0);
    }

    for (int i = 0; i < buf.size; ++i) {
      fprintf(stderr, "Consumer received %d in index %d\n", buf.arr[i], i);
      buf.arr[i] = -1;
    }
    buf.size = 0;

    fprintf(stderr, "Consumer waiting for full buffer...\n");
    assert(pthread_cond_signal(&buf_empty) == 0);
    assert(pthread_mutex_unlock(&mutex) == 0);
  }
}

int main(void) {
  int rv;
  pthread_t producer_thread;
  pthread_t consumer_thread;

  assert(pthread_mutex_init(&mutex, NULL) == 0);
  assert(pthread_cond_init(&buf_empty, NULL) == 0);
  assert(pthread_cond_init(&buf_full, NULL) == 0);

  assert(pthread_create(&producer_thread, NULL, (void*)producer, NULL) == 0);
  assert(pthread_create(&consumer_thread, NULL, (void*)consumer, NULL) == 0);

  assert(pthread_join(producer_thread, NULL) == 0);
  assert(pthread_join(consumer_thread, NULL) == 0);

  assert(pthread_mutex_destroy(&mutex) == 0);
  assert(pthread_cond_destroy(&buf_empty) == 0);
  assert(pthread_cond_destroy(&buf_full) == 0);
}
