#include <stdio.h> // for stdout
#include <stdlib.h> // for exit
#include <fcntl.h> // for file read options
#include <sys/mman.h>
#include <sys/stat.h>

#include "reference.h"
#include "avx.h"


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
	avxFromFile(fptr, st.st_size);
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
