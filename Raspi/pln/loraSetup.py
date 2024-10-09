from LoRaRF import SX127x, LoRaSpi, LoRaGpio
from .loggers import Log

try :
    spi = LoRaSpi(0, 0)
    cs = LoRaGpio(0, 8)
    reset = LoRaGpio(0, 24)

    LoRa = SX127x(spi, cs, reset)

    if not LoRa.begin():
        raise Exception("Something wrong, can't begin LoRa radio")
    LoRa.setFrequency(433000000)
    LoRa.setRxGain(LoRa.RX_GAIN_BOOSTED, LoRa.RX_GAIN_AUTO)
    LoRa.setTxPower(20, LoRa.TX_POWER_PA_BOOST)
    LoRa.setSpreadingFactor(10)
    LoRa.setBandwidth(125000)
    LoRa.setCodeRate(5)
    LoRa.setHeaderType(LoRa.HEADER_EXPLICIT)
    LoRa.setPreambleLength(12)
    LoRa.setPayloadLength(64)
    LoRa.setCrcEnable(True)
    LoRa.setSyncWord(0x34)

    print("\n-- LoRa Receiver --\n")
except Exception as e:
    print("Error:", str(e))
    Log.error("Error:", str(e))
