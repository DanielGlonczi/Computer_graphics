CC=gcc
CFLAGS=-std=c11 `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`

all: lines rects

lines: line_draw.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

rects: line_draw.c
	$(CC) $(CFLAGS) -DRECTS -o $@ $< $(LDFLAGS)

clean:
	rm -f lines rects
