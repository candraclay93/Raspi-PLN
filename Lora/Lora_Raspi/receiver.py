import time
from LoRaRF import SX127x, LoRaSpi, LoRaGpio
import os
import sys
from RPi import GPIO

currentdir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.dirname(os.path.dirname(currentdir)))

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

print("Begin LoRa radio")
if not LoRa.begin():
    raise Exception("Something wrong, can't begin LoRa radio")

print("Set frequency to 433 Mhz")
LoRa.setFrequency(433000000)

print("Set RX gain to power saving gain")
LoRa.setRxGain(LoRa.RX_GAIN_POWER_SAVING, LoRa.RX_GAIN_AUTO)

print("Set modulation parameters:\n\tSpreading factor = 7\n\tBandwidth = 125 kHz\n\tCoding rate = 4/5")
LoRa.setSpreadingFactor(10)
LoRa.setBandwidth(125000)
LoRa.setCodeRate(5)

print("Set packet parameters:\n\tExplicit header type\n\tPreamble length = 12\n\tPayload Length = 15\n\tCRC on")
LoRa.setHeaderType(LoRa.HEADER_EXPLICIT)
LoRa.setPreambleLength(12)
LoRa.setPayloadLength(15)
LoRa.setCrcEnable(True)

print("Set syncronize word to 0x34")
LoRa.setSyncWord(0x34)

print("\n-- LoRa Receiver --\n")

def receive_callback():
    # print("Receive")
    GPIO.output(LED_RECEIVE_PIN, GPIO.HIGH)
    time.sleep(.3)

try:
    while True:
        GPIO.output(LED_RECEIVE_PIN, GPIO.LOW)
        LoRa.request()
        LoRa.wait()

        message = ""
        while LoRa.available() > 1:
            message += chr(LoRa.read())
        counter = LoRa.read()

        # print(LoRa.get())
        LoRa.onReceive(receive_callback())
        
        print(f"{message}  {counter}")
        print("Packet status: RSSI = {0:0.2f} dBm | SNR = {1:0.2f} dB".format(
            LoRa.packetRssi(), LoRa.snr()))

        status = LoRa.status()
        if status == LoRa.STATUS_CRC_ERR:
            print("CRC error")
        elif status == LoRa.STATUS_HEADER_ERR:
            print("Packet header error")
            
        print(f"---" * 30)
        
        
except KeyboardInterrupt:
    print("exit code")
finally :
    GPIO.cleanup()
