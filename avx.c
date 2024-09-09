#include <stdio.h> // for stdout
#include <immintrin.h>

const int batch_size = 256 / 8; // avx2 is 256-bit and chars are 8 bits long
const int findcount = 10; // the number of characters to replace in the find and replace arrays
const int bigbufsize = 4096;

void avxFromFile(char str[], int len) {
	__m256i find[findcount] = {_mm256_set1_epi8('a'), _mm256_set1_epi8('b'), _mm256_set1_epi8('e'), _mm256_set1_epi8('i'), _mm256_set1_epi8('l'), _mm256_set1_epi8('g'), _mm256_set1_epi8('o'), _mm256_set1_epi8('s'), _mm256_set1_epi8('t'), _mm256_set1_epi8('z')};
	
	__m256i replace[findcount] = {_mm256_set1_epi8('4'), _mm256_set1_epi8('8'), _mm256_set1_epi8('3'), _mm256_set1_epi8('1'), _mm256_set1_epi8('1'), _mm256_set1_epi8('6'), _mm256_set1_epi8('0'), _mm256_set1_epi8('5'), _mm256_set1_epi8('7'), _mm256_set1_epi8('2')};
	
	int i = 0;
	char bigbuf[bigbufsize];
	char* bigbufptr = bigbuf;
	char* bigbufend = bigbufptr+bigbufsize;
	__m256i batch = _mm256_loadu_si256((__m256i *)(str+i));
	if(len > batch_size) {
		for(; i + (batch_size-1) < len; i += batch_size) {
			batch = _mm256_loadu_si256((__m256i *)(str+i));
			for(int j = 0; j < findcount; j++) {
				batch = _mm256_blendv_epi8(batch, replace[j], _mm256_cmpeq_epi8(batch, find[j]));
			}
			_mm256_storeu_si256((__m256i*)bigbufptr, batch);
			bigbufptr += batch_size;
			if(bigbufptr >= bigbufend) {
				fwrite(bigbuf, sizeof(char), bigbufsize, stdout);
				bigbufptr = bigbuf;
			}
		}
	}
	fwrite(bigbuf, sizeof(char), bigbufptr-bigbuf, stdout);
	// run through the rest of the input
	if (i < len) {
		char printbuffer[batch_size];
		int lastbits = len - i;
		batch = _mm256_loadu_si256((__m256i *)(str + len - batch_size));
		for(int j = 0; j < findcount; j++) {
			batch = _mm256_blendv_epi8(batch, replace[j], _mm256_cmpeq_epi8(batch, find[j]));
		}
		_mm256_storeu_si256((__m256i*)printbuffer, batch);
		fwrite(printbuffer+(batch_size-lastbits), sizeof(char), lastbits, stdout);
	}
	
}
