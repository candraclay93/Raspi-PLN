import time
from LoRaRF import SX127x, LoRaSpi, LoRaGpio
import os
import sys
import json
import re
import signal
from RPi import GPIO
import logging
from cysystemd.journal import JournaldLogHandler


currentdir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.dirname(os.path.dirname(currentdir)))

logger = logging.getLogger(__name__)

journald_handler = JournaldLogHandler()

journald_handler.setFormatter(logging.Formatter(
    '[%(levelname)s] %(message)s'
))

logger.addHandler(journald_handler)
logger.setLevel(logging.DEBUG)

spi = LoRaSpi(0, 0)
cs = LoRaGpio(0, 8)
reset = LoRaGpio(0, 24)


GPIO.setmode(GPIO.BCM)
LED_RUNNING_PIN = 25
LED_RECEIVE_PIN = 23
GPIO.setwarnings(False)
GPIO.setup(LED_RUNNING_PIN, GPIO.OUT)
GPIO.setup(LED_RECEIVE_PIN, GPIO.OUT)
GPIO.output(LED_RUNNING_PIN, GPIO.HIGH)



LoRa = SX127x(spi, cs, reset)


if not LoRa.begin():
    raise Exception("Something wrong, can't begin LoRa radio")
LoRa.setFrequency(433000000)
LoRa.setRxGain(LoRa.RX_GAIN_POWER_SAVING, LoRa.RX_GAIN_AUTO)
LoRa.setSpreadingFactor(10)
LoRa.setBandwidth(125000)
LoRa.setCodeRate(4)
LoRa.setHeaderType(LoRa.HEADER_EXPLICIT)
LoRa.setPreambleLength(12)
LoRa.setPayloadLength(64)
LoRa.setCrcEnable(True)
LoRa.setSyncWord(0x34)

print("\n-- LoRa Receiver --\n")

def signal_handler(sig, frame):
    print("Stopping service...")
    GPIO.output(LED_RUNNING_PIN, GPIO.LOW)
    GPIO.cleanup()
    sys.exit(0)

signal.signal(signal.SIGTERM, signal_handler)
signal.signal(signal.SIGINT, signal_handler)


def receive_callback():
    # print("Receive")
    GPIO.output(LED_RECEIVE_PIN, GPIO.HIGH)
    # logger.info()
    time.sleep(.3)
    
def extract_json_string(input_string):
    # Mencari isi dalam kurung kurawal
    match = re.search(r'\{.*\}', input_string)
    if match:
        json_str = match.group(0)
        try:
            # Mengonversi string JSON menjadi dictionary
            data = json.loads(json_str)
            return data
        except json.JSONDecodeError:
            return "Invalid JSON"
    return "No JSON found"

try:
    while True:
        GPIO.output(LED_RECEIVE_PIN, GPIO.LOW)
        LoRa.request()
        LoRa.wait()

        message = ""
        while LoRa.available() > 1:
            message += chr(LoRa.read())
        counter = LoRa.read()
        
        print(message)
        print("********************************")
        message = extract_json_string(message)
        LoRa.onReceive(receive_callback())
        # logger.info(f"received message {message} with RSII {LoRa.packetRssi()}")
        
        # print(f"{message}  {counter}")
        print("Packet status: RSSI = {0:0.2f} dBm | SNR = {1:0.2f} dB".format(
            LoRa.packetRssi(), LoRa.snr()))

        status = LoRa.status()
        if status == LoRa.STATUS_CRC_ERR:
            print("CRC error")
            # logger.error("CRC error")
        elif status == LoRa.STATUS_HEADER_ERR:
            print("Packet header error")
            # logger.error("Packet header error")
            
        print(f"---" * 30)
        
        
except Exception as e:
    print(f"An error occurred: {e}")
    logger.exception(e)
    
finally :
    GPIO.cleanup()
    
