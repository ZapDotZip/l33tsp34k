all: h4x unh4x

h4x: h4x.c
	cc h4x.c -o h4x

unh4x: unh4x.c
	cc unh4x.c -o unh4x

clean:
	rm -rf h4x unh4x
