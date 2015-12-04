#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

#define GREEN_PIN RPI_GPIO_P1_12
#define PWM_CHANNEL 0
#define RANGE 1024

static char running = 1;

void termination_handler(int signum) {
    printf("caught control-c\n");
    running = 0;
}

int main(int argc, char **argv) {
    signal(SIGINT, termination_handler);

    if (!bcm2835_init()) {
        printf("cannot init\n");
	return 1;
    }

    bcm2835_gpio_fsel(GREEN_PIN, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);

    int data = 1;
    short direction = 1;
    while(running) {
	if (direction == 1 && data >= RANGE-1) {
	    direction = -1;
	} else if (direction == -1 && data <= 0) {
	    direction = 1;
	}
	data += direction;
	printf("%d %d\n", direction, data);
	bcm2835_pwm_set_data(PWM_CHANNEL, data);
	bcm2835_delay(10);
    }
    bcm2835_close();

    return 0;
}
