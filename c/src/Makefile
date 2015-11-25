CC=gcc
INCLUDE_PATHS=/usr/local/include/
CFLAGS=-std=c99 -c -I $(INCLUDE_PATHS)

blink: 
	$(CC) $(CFLAGS) -o blink blink.c -l bcm2835

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $< 

