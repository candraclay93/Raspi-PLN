from RPi import GPIO
import os
import sys
import signal


currentdir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.dirname(os.path.dirname(currentdir)))

def signal_handler(sig, frame):
    print("Stopping service...")
    BOARD.runningLed(False)
    BOARD.cleanup()
    sys.exit(0)

signal.signal(signal.SIGTERM, signal_handler)
signal.signal(signal.SIGINT, signal_handler)

class BOARD :
    LED_RUNNING_PIN = 25
    LED_RECEIVE_PIN = 23
    
    @staticmethod
    def setup():
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(BOARD.LED_RUNNING_PIN, GPIO.OUT)
        GPIO.setup(BOARD.LED_RECEIVE_PIN, GPIO.OUT)
        GPIO.output(BOARD.LED_RUNNING_PIN, GPIO.HIGH)
        
    @staticmethod
    def cleanup():
        GPIO.output(BOARD.LED_RUNNING_PIN, GPIO.LOW)
        GPIO.cleanup()
        
    def receiveLed(on = False):
        if(on):
            GPIO.output(BOARD.LED_RECEIVE_PIN, GPIO.HIGH)
        GPIO.output(BOARD.LED_RECEIVE_PIN, GPIO.LOW)
        
    def runningLed(on = False):
        if(on):
            GPIO.output(BOARD.LED_RUNNING_PIN, GPIO.HIGH)
        GPIO.output(BOARD.LED_RUNNING_PIN, GPIO.LOW)
        

