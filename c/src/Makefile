CC=gcc
POCKET_SPHINX_INCLUDE_PATHS=-I /usr/local/include/sphinxbase -I /usr/local/include/pocketsphinx
CFLAGS=-std=c99 -g -pthread

all: out blink button buttonmt tilt buzzer lizzy rgbled dimled servo dimledpwm

out:
	mkdir out

dimledpwm: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

servo: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

dimled: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

rgbled: out
	$(CC) $(CFLAGS) -o out/$@ $@.c -l bcm2835

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

lizzy: out
	$(CC) $(CFLAGS) $(POCKET_SPHINX_INCLUDE_PATHS) -o out/$@ $@.c -l pocketsphinx -l sphinxad

%.o: %.c
	$(CC) $(CFLAGS) -o out/$@ $< 

