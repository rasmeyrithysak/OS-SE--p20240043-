#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

void *worker(void *arg) {
    long id = (long)arg;
    int computed = (int)(id * 10 + 5);
    printf("[Worker %ld] tid(pthread)=%lu, pid=%d, computed value=%d\n",
           id, (unsigned long)pthread_self(), getpid(), computed);
    sleep(1);
    int *result = malloc(sizeof(int));
    *result = computed;
    pthread_exit((void *)result);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_t extra_threads[2];
    int sum = 0;

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        void *ret;
        pthread_join(threads[i], &ret);
        sum += *(int *)ret;
        free(ret);
    }

    printf("Original %d workers joined. Now starting 2 EXTRA workers (live curveball)...\n", NUM_THREADS);

    for (long i = 0; i < 2; i++) {
        long id = NUM_THREADS + i;
        pthread_create(&extra_threads[i], NULL, worker, (void *)id);
    }

    sleep(2);

    for (int i = 0; i < 2; i++) {
        void *ret;
        pthread_join(extra_threads[i], &ret);
        sum += *(int *)ret;
        free(ret);
    }

    printf("Summary: all %d workers joined, total = %d\n", NUM_THREADS + 2, sum);
    return 0;
}
