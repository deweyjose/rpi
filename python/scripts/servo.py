import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
servoPin=12
GPIO.setup(servoPin, GPIO.OUT)
pwm=GPIO.PWM(servoPin, 50)
pwm.start(7)
for i in range(0,20):
    desiredPosition=input("where do you wnt it? ")
    DC=1./18.*(desiredPosition)/+2
    pwm.ChangeDutyCycle(DC)
pwm.stop()
GPIO.cleanup()

