#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
struct thread_args {
  int a;
  int b;
  char *s;

  // return args
  int sum;
};

void print_hi(void *_args) {
  struct thread_args *args = (struct thread_args*)_args;
  fprintf(stderr, "a: %d\n", args->a);
  fprintf(stderr, "b: %d\n", args->b);
  fprintf(stderr, "s: %s\n", args->s);

  for (int i = 0; i < 10; ++i) {
    fprintf(stderr, "Hello! %d\n", i);
    sleep(1);
  }

  args->sum = args->a + args->b;
}

int main(void) {
  pthread_t thread;

  struct thread_args args = {
    .a = 4,
    .b = 2,
    .s = "asdf",
  };

  int thread_create = pthread_create(&thread, NULL, (void*)print_hi, &args);
  if (thread_create != 0) {
    fprintf(stderr, "pthread_create failed with error %d\n", thread_create);
    exit(1);
  }

  int value;
  int thread_join = pthread_join(thread, (void*)&value);
  if (thread_join != 0) {
    fprintf(stderr, "pthread_join failed with error %d\n", thread_join);
    exit(1);
  }

  fprintf(stderr, "Done!\n");
  fprintf(stderr, "sum is %d\n", args.sum);

}
