import RPi.GPIO as GPIO
import time

global ECHO, TRIGGER, RED, GREEN
ECHO  = 12
TRIGGER = 7
RED = 18
GREEN = 16
BUZZER = 22

minimum_distance = int(raw_input("enter minimum distance: "))
maximum_distance = int(raw_input("enter maximum distance: "))

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(TRIGGER, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)
GPIO.setup(RED, GPIO.OUT)
GPIO.setup(GREEN, GPIO.OUT)
GPIO.setup(BUZZER, GPIO.OUT)

GPIO.output(TRIGGER, False)
GPIO.output(RED, False)
GPIO.output(GREEN, False)
GPIO.output(BUZZER, False)

for num in range(1, 3):
  print "Starting!!!"
  GPIO.output(RED, True)
  GPIO.output(BUZZER, True)
  GPIO.output(GREEN, True)
  time.sleep(1)
  GPIO.output(TRIGGER, False)
  GPIO.output(RED, False)
  GPIO.output(GREEN, False)
  GPIO.output(BUZZER, False)
  time.sleep(1)

in_range = False
try:
  while True:
    GPIO.output(TRIGGER, False)
    time.sleep(2)
    GPIO.output(TRIGGER, True)
    time.sleep(0.00001)
    GPIO.output(TRIGGER, False)

    start = time.time()
    while GPIO.input(ECHO) == 0:
      start = time.time()

    stop = time.time()
    while GPIO.input(ECHO) == 1:
      stop = time.time()

    elapsed = stop - start

    distance = elapsed*13504
    distance = distance/2
 
    print "Distance: %.1f inches" % distance
    
    if (distance > maximum_distance):
      print "too far..."
      GPIO.output(RED, True)
      GPIO.output(GREEN, False)
      GPIO.output(BUZZER, True)
      in_range = False
    elif (distance < minimum_distance):
      print "too close..."
      GPIO.output(RED, True)
      GPIO.output(GREEN, False)
      GPIO.output(BUZZER, True)
      in_range = False
    else:
      print "ok..."
      if in_range == False:
        in_range = True
        GPIO.output(RED, False)
        GPIO.output(GREEN, True)
        for num in range(1,3):
          GPIO.output(BUZZER, True)
          time.sleep(0.5)
          GPIO.output(BUZZER, False)
          time.sleep(0.5)

  GPIO.cleanup()
except:
  print "cleanup"
  GPIO.cleanup()
