#include <stdio.h>
#include <bcm2835.h>

#define BUZ_PIN RPI_GPIO_P1_15

int main(int argc, char **argv) {
    if (!bcm2835_init()) {
        printf("cannot init\n");
	return 1;
    }

    bcm2835_gpio_fsel(BUZ_PIN, BCM2835_GPIO_FSEL_OUTP);

    for (int i = 0; i < 10; i++) {
	printf("buzzer pin high\n");
	bcm2835_gpio_write(BUZ_PIN, HIGH);
	bcm2835_delay(1000);

	printf("buzzer pin low\n");
        bcm2835_gpio_write(BUZ_PIN, LOW);
	bcm2835_delay(1000);
    }

    bcm2835_close();
    return 0;
}
