CFLAGS = -Wall -O3 -march=skylake
H4XSRC = h4x.c reference.c avx.c threaded.c

all: h4x unh4x

h4x: $(H4XSRC)
	$(CC) $(CFLAGS) $(H4XSRC) -o h4x

unh4x: unh4x.c
	$(CC) $(CFLAGS) unh4x.c -o unh4x

clean:
	rm -rf h4x unh4x
