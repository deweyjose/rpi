import RPi.GPIO as GPIO
import time
import sys

#################################


class RANGE_STATE:
  PENDING=1
  SENDING=2
  RECEIVED=3
  READY=4

class range_finder:

  def __init__(self):
    self.state = RANGE_STATE.READY

  def trigger(self):
    if self.state == RANGE_STATE.READY:
      self.state = RANGE_STATE.PENDING
      print "triggering..."
      GPIO.output(TRIGGER, False)
      time.sleep(2)
      GPIO.output(TRIGGER, True)
      time.sleep(0.00001)
      GPIO.output(TRIGGER, False)
    else:
      print "not ready to trigger %d" % self.state

  def echo(self, channel):
    if self.state == RANGE_STATE.PENDING:
      self.state = RANGE_STATE.SENDING
      self.start_time = time.time()
#      print "echo handler sending"
    elif self.state == RANGE_STATE.SENDING:
      self.state = RANGE_STATE.RECEIVED
      self.end_time = time.time()
#      print "echo handler received"
#    else:
#      print "echo handler bad state %d" % self.state

  def get_distance(self):
    if self.state == RANGE_STATE.RECEIVED:
      print "end %.5f, start %.5f, elapsed %.5f" % (self.end_time, self.start_time,self.end_time-self.start_time)
      elapsed = self.end_time - self.start_time
      distance = elapsed*13504
      distance = distance/2
      self.state = RANGE_STATE.READY
      return distance
    else:
      print "Not received yet..."
      return None
    

#################################

global ECHO, TRIGGER, RED, GREEN
ECHO  = 12
TRIGGER = 7
RED = 18
GREEN = 16
BUZZER = 22

minimum_distance = 3 #int(raw_input("enter minimum distance: "))
maximum_distance = 11 #int(raw_input("enter maximum distance: "))

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(TRIGGER, GPIO.OUT)
GPIO.setup(RED, GPIO.OUT)
GPIO.setup(GREEN, GPIO.OUT)
GPIO.setup(BUZZER, GPIO.OUT)

GPIO.setup(ECHO, GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.output(TRIGGER, False)
GPIO.output(RED, False)
GPIO.output(GREEN, False)
GPIO.output(BUZZER, False)

for num in range(1, 1):
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

range_finder = range_finder()

callback = range_finder.echo

GPIO.add_event_detect(ECHO, GPIO.BOTH, callback=callback)

for num in range(0,100):
  range_finder.trigger()
  distance = range_finder.get_distance()
  if distance != None: 
    print "Distance: %.1f inches" % distance

die
    
in_range = False
try:
  while True:
    time.sleep(1)
    range_finder.trigger()
    distance = range_finder.get_distance()
    if distance != None: 
      print "Distance: %.1f inches" % distance
    
    if (distance > maximum_distance):
      print "too far..."
      GPIO.output(RED, True)
      GPIO.output(GREEN, False)
      #GPIO.output(BUZZER, True)
      in_range = False
    elif (distance < minimum_distance):
      print "too close..."
      GPIO.output(RED, True)
      GPIO.output(GREEN, False)
      #GPIO.output(BUZZER, True)
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
  print "Unexpected error:", sys.exc_info()[0]
  GPIO.cleanup()
