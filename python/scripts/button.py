import RPi.GPIO as GPIO
import time

OUTPUT=12
INPUT=7

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(OUTPUT, GPIO.OUT)
GPIO.setup(INPUT, GPIO.IN, pull_up_down=GPIO.PUD_UP)

while 1:
    if (GPIO.input(INPUT) == False):
        print("pushed!")
        GPIO.output(OUTPUT, True)
    else:
        print("not pushed!")
        GPIO.output(OUTPUT, False)

GPIO.cleanp()

