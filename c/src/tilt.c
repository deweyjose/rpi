#include <stdio.h>
#include <bcm2835.h>

#define LED_PIN RPI_GPIO_P1_07
#define BUZ_PIN RPI_GPIO_P1_11
#define TLT_PIN RPI_GPIO_P1_15


int main(int argc, char **argv) {
    if (!bcm2835_init()) {
	return 1;
    }

    bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(BUZ_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(TLT_PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(TLT_PIN, BCM2835_GPIO_PUD_UP);


    for (int i = 0; i < 100; i++) {
	uint8_t value = bcm2835_gpio_lev(TLT_PIN);
	printf("read from pin 7: %d\n", value);
	if (value == 1) {
	    bcm2835_gpio_write(LED_PIN, LOW);
	    bcm2835_gpio_write(BUZ_PIN, HIGH);
	} else {
	    bcm2835_gpio_write(LED_PIN, HIGH);
	    bcm2835_gpio_write(BUZ_PIN, LOW);
	}
	bcm2835_delay(100);
    }
    bcm2835_close();
    return 0;
}
