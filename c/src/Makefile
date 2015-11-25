CC=gcc
INCLUDE_PATHS=/usr/local/include/
CFLAGS=-std=c99 -I $(INCLUDE_PATHS)

all: blink button

blink:
	$(CC) $(CFLAGS) -o $@ $@.c -l bcm2835

button:
	$(CC) $(CFLAGS) -o $@ $@.c -l bcm2835

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $< 

