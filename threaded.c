#include <stdio.h> // for stdout
#include <immintrin.h>
#include <pthread.h>
#include <unistd.h>

#include "avx.h"

struct thread_args {
	char* input;
	char* output;
	size_t len;
};


static const int batch_size = 256 / 8; // avx2 is 256-bit and chars are 8 bits long
static const int findcount = 10; // the number of characters to replace in the find and replace arrays

void threaded_avx(struct thread_args* args) {
	char* str = args->input;
	char* out = args->output;
	__m256i find[findcount] = {_mm256_set1_epi8('a'), _mm256_set1_epi8('b'), _mm256_set1_epi8('e'), _mm256_set1_epi8('i'), _mm256_set1_epi8('l'), _mm256_set1_epi8('g'), _mm256_set1_epi8('o'), _mm256_set1_epi8('s'), _mm256_set1_epi8('t'), _mm256_set1_epi8('z')};
	
	__m256i replace[findcount] = {_mm256_set1_epi8('4'), _mm256_set1_epi8('8'), _mm256_set1_epi8('3'), _mm256_set1_epi8('1'), _mm256_set1_epi8('1'), _mm256_set1_epi8('6'), _mm256_set1_epi8('0'), _mm256_set1_epi8('5'), _mm256_set1_epi8('7'), _mm256_set1_epi8('2')};
	
	size_t i = 0;
	__m256i batch;
	for(; i + (batch_size-1) < args->len; i += batch_size) {
		if(str != args->input) {
			exit(69);
		}
		batch = _mm256_loadu_si256((__m256i *)(str+i));
		for(int j = 0; j < findcount; j++) {
			batch = _mm256_blendv_epi8(batch, replace[j], _mm256_cmpeq_epi8(batch, find[j]));
		}
		_mm256_storeu_si256((__m256i*)(out+i), batch);
	}
}


void threadedlaunch(char str[], size_t len, size_t bufsize ) {
	size_t THREAD_BATCH_SIZE = 1024 * 1024 * bufsize; // variable buffer size per core
	long core_count = sysconf(_SC_NPROCESSORS_ONLN);
	size_t trueBatchSize = THREAD_BATCH_SIZE * core_count;
	char* outs[core_count];
	for(int c = 0; c < core_count; c++) {
		if(posix_memalign((void**)&outs[c], sysconf(_SC_PAGESIZE), THREAD_BATCH_SIZE * sizeof(char)) != 0) {
			fprintf(stderr, "Error allocating memory!\n");
			exit(3);
		}
	}
	char* end = &str[len];
	pthread_t threads[core_count];
	if(len > THREAD_BATCH_SIZE) {
		while(str+trueBatchSize < end) {
			for(int c = 0; c < core_count; c++) {
				struct thread_args* args = malloc(sizeof(struct thread_args)); // a small memory leak for a few dozen MB/s.
				args->input = &str[THREAD_BATCH_SIZE*c];
				args->output = outs[c];
				args->len = THREAD_BATCH_SIZE;
				pthread_create(&threads[c], NULL, (void*)threaded_avx, (void*)args);
			}
			for(int c = 0; c < core_count; c++) {
				pthread_join(threads[c], NULL);
				fwrite(outs[c], sizeof(char), THREAD_BATCH_SIZE, stdout);
			}
			str += trueBatchSize;
		}
	}
	// run through the rest of the input
	avxFromFile(str, end-str);
}
