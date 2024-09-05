#include <stdio.h>
#include <ctype.h>

int main() {
	char c = '\0';
	while((c = getchar()) != EOF) {
		switch(tolower(c)) {
			case '4':
				putchar('a');
				break;
			case '8':
				putchar('b');
				break;
			case '3':
				putchar('e');
				break;
			case '1':
				putchar('[');
				putchar('i');
				putchar(',');
				putchar('l');
				putchar(']');
				break;
			case '6':
				putchar('g');
				break;
			case '0':
				putchar('o');
				break;
			case '5':
				putchar('s');
				break;
			case '7':
				putchar('t');
				break;
			case '2':
				putchar('z');
				break;
			default:
				putchar(c);
				break;
		}
		
	}
	
}
