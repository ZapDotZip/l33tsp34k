#include <stdio.h> // for EOF
#include <ctype.h> // for tolower
#include <stdlib.h> // for exit
#include <fcntl.h> // for file read options
#include <sys/mman.h>
#include <sys/stat.h>


void fromstdin() {
	char c = '\0';
	while((c = getchar()) != EOF) {
		switch(tolower(c)) {
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

void fromFile(char str[], int len) {
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
	}
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
