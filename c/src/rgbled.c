#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

#define RED_PIN RPI_GPIO_P1_11
#define GREEN_PIN RPI_GPIO_P1_12
#define BLUE_PIN RPI_GPIO_P1_22

static char running = 1;

void update_display();
void display_none();

void termination_handler(int signum) {
    printf("caught control-c\n");
    running = 0;
    display_none();
}

void display_red() {
    update_display(HIGH, LOW, LOW);
}

void display_green() {
    update_display(LOW, HIGH, LOW);
}

void display_blue() {
    update_display(LOW, LOW, HIGH);
}

void display_all() {
    for (int red = 0; red < 2; red++) {
	for (int green = 0; green < 2; green++) {
	    for (int blue = 0; blue < 2; blue++) {
		update_display(red, green, blue);
		bcm2835_delay(1000);
	    }
	}
    }
}

void display_none() {
    update_display(LOW, LOW, LOW);
}

void update_display(int RED, int GREEN, int BLUE) {
    bcm2835_gpio_write(RED_PIN, RED);
    bcm2835_gpio_write(GREEN_PIN, GREEN);
    bcm2835_gpio_write(BLUE_PIN, BLUE);	
}

int main(int argc, char **argv) {
    signal(SIGINT, termination_handler);

    if (!bcm2835_init()) {
        printf("cannot init\n");
	return 1;
    }

    bcm2835_gpio_fsel(RED_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GREEN_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(BLUE_PIN, BCM2835_GPIO_FSEL_OUTP);

    display_none();
    display_all ();
    display_none();

    for (int i = 1; i < atoi(argv[1]); i++) {
	int delay = 1000/i;
	display_red();
	bcm2835_delay(delay);
	display_green();
	bcm2835_delay(delay);
	display_blue();
	bcm2835_delay(delay);
	display_none();
	bcm2835_delay(delay);
    }

    while (running ) {
	update_display(HIGH, HIGH, LOW);
	display_none();
    }

    bcm2835_close();

    return 0;
}
