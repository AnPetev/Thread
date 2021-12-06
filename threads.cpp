#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
struct map_argument {
    void** things;
    void** results;
    void* (*f)(void*);
    int from;
    int to;
}; 
void** concurrent_map(void** things, void* (*f)(void*), int length,
    int nthreads) {

    void** results = malloc(sizeof(void*) * length);
    struct map_argument arguments[nthreads];
    pthread_t threads[nthreads];
    int chunk_size = length / nthreads;

    for (int j = 0; j < nthreads; j++) {
        int from = j * chunk_size;
        struct map_argument* argument = &arguments[j];
        init_map_argument(argument, things, results, f, from, from + chunk_size);
        pthread_create(&threads[j], NULL, chunk_map, (void*)argument);
    }


    for (int i = 0; i < length % nthreads; i++) {
        int idx = chunk_size * nthreads + i;
        results[idx] = (*f)(things[idx]);
    }

    for (int k = 0; k < nthreads; k++) {
        pthread_join(threads[k], NULL);
    }

    return results;
}