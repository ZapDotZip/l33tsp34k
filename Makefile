CFLAGS = -Wall -O3

all: h4x unh4x

h4x: h4x.c
	$(CC) $(CFLAGS) h4x.c -o h4x

unh4x: unh4x.c
	$(CC) $(CFLAGS) unh4x.c -o unh4x

clean:
	rm -rf h4x unh4x
