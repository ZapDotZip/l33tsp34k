#include <stdio.h> // for stdout
#include <stdlib.h> // for exit
#include <fcntl.h> // for file read options
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h> // argument parsing

#include "reference.h"
#include "avx.h"
#include "threaded.h"

static int mode = 0;
static int bufsize = 0;


void loadFile(char* path) {
	int fd = open(path, O_RDONLY);
	if(fd == -1) {
		printf("error: cannot open file %s\n", path);
		exit(1);
	}
	struct stat st;
	fstat(fd, &st);
	char *fptr = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (fptr == MAP_FAILED) {
		printf("error: cannot open file %s\n", path);
		exit(2);
	}
	if(mode == 0) avxFromFile(fptr, st.st_size);
	else threadedlaunch(fptr, st.st_size, bufsize);
}

int main(int argc, char *argv[]) {
	if(argc > 1) {
		switch(argv[1][1]) {
			case 'r':
				fromstdin();
				break;
			case 'a':
				mode = 0;
				for(int i = 2; i < argc; i++) {
					loadFile(argv[i]);
				}
				break;
			case 't':
				mode = 1;
				bufsize = atoi(argv[2]);
				for(int i = 3; i < argc; i++) {
					loadFile(argv[i]);
				}
				break;
			default:
				fprintf(stderr, "No option specified. Going with ??? as the default.\n");
				for(int i = 2; i < argc; i++) {
					loadFile(argv[i]);
				}
				break;
		}
		
	} else {
		fromstdin();
	}
}
