import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.OUT)
#GPIO.setup(12, GPIO.OUT)

def blink(numtimes,speed):
    for i in range(0,numtimes):
        print "Iteration " + str(i+1)
        GPIO.output(7, GPIO.HIGH)
        time.sleep(speed)
        GPIO.output(7, GPIO.LOW)
        time.sleep(speed)
    print "Done"

    GPIO.output(7, GPIO.LOW)

    GPIO.cleanup()

iterations = raw_input("enter total number of times to blink: ")
speed = raw_input("enter length of each blink(seconds): ")


blink(int(iterations),float(speed))
