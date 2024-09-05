#include <stdio.h>
#include <ctype.h>

int main() {
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
