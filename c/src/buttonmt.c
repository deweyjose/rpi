#include <bcm2835.h>
#include <pthread.h>
#include <stdio.h>

#define PIN RPI_GPIO_P1_07

typedef struct {
    char running;
    int pin;
    char signaled;
} pin_monitor_info ; 

void * pin_monitor(void * arg) {
    pin_monitor_info * info;
    info = (pin_monitor_info*)arg;
   
    printf("starting monitor for %d, running %d\n", info->pin, info->running);
    
    time_t seconds = 0;
    while (info->running) {
	uint8_t value = bcm2835_gpio_lev(PIN);
	if (value == 0) {
	    printf("button pressed!\n");
	    if (seconds == 0) {
		seconds = time(NULL);
	    } else {
		if ((time(NULL) - seconds) > 2) {
		    printf("kill code received\n");
		    info->signaled = 1;
		}
	    }
	} else {
	    seconds = 0;
	}
	bcm2835_delay(100);
    }

    printf("shutting down pin %d monitor\n", info->pin);
}

int main(int argc, char **argv) {
    if (!bcm2835_init()) {
	return 1;
    }

    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);

    pin_monitor_info info;
    info.running = 1;
    info.signaled = 0;
    info.pin = PIN;


    printf("info running %d, pin %d, ", info.running, info.pin);
    pthread_t thread;
    pthread_create(&thread, NULL, pin_monitor, &info);


    while (!info.signaled) {
	sleep(1);
    }


    printf("button signaled! set running to 0\n");
    info.running = 0;
    pthread_join(thread, NULL);


    bcm2835_close();
    return 0;
}
