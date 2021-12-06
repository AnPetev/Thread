#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


#define N
void* result;

void* reduce(void* (function_p)(void*), void* data_p) {
	pthread_t threads[N];
	for (int i = 0; i < N; ++i) {
		pthread_t p;
		pthread_create(&p, NULL, function_p, data_p + i * N);
		threads[i] = p;
	}

	for (int i = 0; i < N; ++i) {
		pthread_join(threads[i], NULL);
	}
	return finction_p(result);
}
void function_p(void* arr) {
	int v = 0;
	for (int i = 0; i < N; ++i) {
		v += arr[i];
		{
		}
		return;