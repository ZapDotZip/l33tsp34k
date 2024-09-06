#include <stdio.h> // for EOF
#include <ctype.h> // for tolower
#include <stdlib.h> // for exit
#include <fcntl.h> // for file read options
#include <sys/mman.h>
#include <sys/stat.h>
#include <immintrin.h>

void fromstdin() {
	char c = '\0';
	while((c = getchar()) != EOF) {
		switch(c) { // tolower(c) -- can't do tolower() right now because we need to test the avx func
			case 'a':
				putchar('4');
				break;
			case 'b':
				putchar('8');
				break;
			case 'e':
				putchar('3');
				break;
			case 'i':
				putchar('1');
				break;
			case 'l':
				putchar('1');
				break;
			case 'g':
				putchar('6');
				break;
			case 'o':
				putchar('0');
				break;
			case 's':
				putchar('5');
				break;
			case 't':
				putchar('7');
				break;
			case 'z':
				putchar('2');
				break;
			default:
				putchar(c);
				break;
		}
	}
}

const int batch_size = 256 / 8; // avx2 is 256-bit and chars are 8 bits long
const int findcount = 10; // the number of characters to replace in the find and replace arrays


void fromFile(char str[], int len) {
	__m256i find[findcount] = {_mm256_set1_epi8('a'), _mm256_set1_epi8('b'), _mm256_set1_epi8('e'), _mm256_set1_epi8('i'), _mm256_set1_epi8('l'), _mm256_set1_epi8('g'), _mm256_set1_epi8('o'), _mm256_set1_epi8('s'), _mm256_set1_epi8('t'), _mm256_set1_epi8('z')};
	
	__m256i replace[findcount] = {_mm256_set1_epi8('4'), _mm256_set1_epi8('8'), _mm256_set1_epi8('3'), _mm256_set1_epi8('1'), _mm256_set1_epi8('1'), _mm256_set1_epi8('6'), _mm256_set1_epi8('0'), _mm256_set1_epi8('5'), _mm256_set1_epi8('7'), _mm256_set1_epi8('2')};
	
	int i = 0;
	char printbuffer[batch_size];
	
	if(len > batch_size) {
		for(; i + (batch_size-1) < len; i += batch_size) {
			__m256i batch = _mm256_loadu_si256((__m256i *)(str+i));
			for(int j = 0; j < findcount; j++) {
				__m256i cmp = _mm256_cmpeq_epi8(batch, find[j]);
				batch = _mm256_blendv_epi8(batch, replace[j], cmp);
			}
			_mm256_storeu_si256((__m256i*)printbuffer, batch);
			fwrite(printbuffer, sizeof(char), batch_size, stdout);
		}
	}
	// run through the rest of the input
	int lastbits = len - i;
	if (i < len) {
		__m256i batch = _mm256_loadu_si256((__m256i *)(str + len - batch_size));
		for(int j = 0; j < findcount; j++) {
			__m256i cmp = _mm256_cmpeq_epi8(batch, find[j]);
			batch = _mm256_blendv_epi8(batch, replace[j], cmp);
		}
		_mm256_storeu_si256((__m256i*)printbuffer, batch);
		fwrite(printbuffer+(batch_size-lastbits), sizeof(char), lastbits, stdout);
	}
	
	/*
	for(int i = 0; i < len; i++) {
		switch(tolower(str[i])) {
			case 'a':
				putchar('4');
				break;
			case 'b':
				putchar('8');
				break;
			case 'e':
				putchar('3');
				break;
			case 'i':
				putchar('1');
				break;
			case 'l':
				putchar('1');
				break;
			case 'g':
				putchar('6');
				break;
			case 'o':
				putchar('0');
				break;
			case 's':
				putchar('5');
				break;
			case 't':
				putchar('7');
				break;
			case 'z':
				putchar('2');
				break;
			default:
				putchar(str[i]);
				break;
		}
	}*/
}

void loadFile(char* path) {
	int fd = open(path, O_RDONLY);
	if(fd == -1) {
		printf("error: cannot open file %s", path);
		exit(1);
	}
	struct stat st;
	fstat(fd, &st);
	char *fptr = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (fptr == MAP_FAILED) {
		printf("error: cannot open file %s", path);
		exit(2);
	}
	fromFile(fptr, st.st_size);
}

int main(int argc, char *argv[]) {
	if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			loadFile(argv[i]);
		}
	} else {
		fromstdin();
	}
}
