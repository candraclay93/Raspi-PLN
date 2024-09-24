import time
from pln import LoRa, Log, BOARD, Utils, QTClient
import json

BOARD.setup()
    


try:
    while True:
        BOARD.receiveLed(False)
        LoRa.request()
        LoRa.wait()

        message = ""
        while LoRa.available() > 1:
            message += chr(LoRa.read())
        counter = LoRa.read()
        
        print("********************************")
        message = Utils.extract_json_string(message)
        LoRa.onReceive(Utils.receive_callback())
        # Log.info(f"received message {message} with RSII {LoRa.packetRssi()}")
        
        print(message)
        # print(f"{message}  {counter}")
        print("Packet status: RSSI = {0:0.2f} dBm | SNR = {1:0.2f} dB".format(
            LoRa.packetRssi(), LoRa.snr()))
        
        
        payload = json.dumps([message])
        topic = "supersun/25810618-7a40-4df7-b16f-abbcfd7800fb/data"        
        result = QTClient.publish(topic, payload)


        Utils.cek_status_lora()
        print(f"---" * 30)
        
        
except Exception as e:
    print(f"An error occurred: {e}")
    Log.exception(e)
    
finally :
    BOARD.cleanup()
    Log.close()
    
