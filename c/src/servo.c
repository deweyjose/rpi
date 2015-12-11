#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#define LED_PIN RPI_GPIO_P1_07

static char running = 1;
static int RANGE = 100;

typedef struct {
    char running;
    int duty_cycle;
} pwm_info ; 

static int total_cycle = 20000000;
static int degrees_180 = 3000000;
static int degrees_90  = 1750000;
static int degrees_0   = 800000;

void move(int degrees) {
    for (int i = 0; i < 15; ++i) {
	bcm2835_gpio_write(LED_PIN, HIGH);
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = degrees;
	nanosleep(&ts, NULL);
	bcm2835_gpio_write(LED_PIN, LOW);
	ts.tv_nsec = total_cycle - degrees;
	nanosleep(&ts, NULL);
    }
}

void * pwm_thread(void * arg) {
    pwm_info * info;
    info = (pwm_info*)arg;

    while (info->running) {
	move(degrees_180);
	sleep(1);
	//move(degrees_90);
	//sleep(1);
	move(degrees_0);
	sleep(1);
    }
}

void termination_handler(int signum) {
    printf("caught control-c\n");
    running = 0;
}

void set_priority(int priority) {
    struct sched_param sched;
    memset(&sched, 0, sizeof(sched));
    sched.sched_priority = priority;
    sched_setscheduler(0, SCHED_RR, &sched);
}

int main(int argc, char **argv) {
    signal(SIGINT, termination_handler);

    set_priority(50);

    if (!bcm2835_init()) {
        printf("cannot init\n");
	return 1;
    }

    bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_OUTP);

    pwm_info info;
    info.running = 1;
    info.duty_cycle = 1500000;

    pthread_t thread;
    pthread_create(&thread, NULL, pwm_thread, &info);

    while (running) {
	char buffer[10];
	fgets(buffer, 10, stdin);
	info.duty_cycle = atoi(buffer);
	printf("setting duty cycle to %d\n", info.duty_cycle);
	sleep(1);
    }

    info.running = 0;

    pthread_join(thread, NULL);

    bcm2835_close();

    return 0;
}
