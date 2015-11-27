CC=gcc
INCLUDE_PATHS=/usr/local/include/
CFLAGS=-std=c99 -I $(INCLUDE_PATHS) -pthread

all: out blink button buttonmt tilt buzzer

out:
	mkdir out

buzzer: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

tilt: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

buttonmt: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

blink: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

button: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

%.o: %.c
	$(CC) $(CFLAGS) -o out/$@ $< 

