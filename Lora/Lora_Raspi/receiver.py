import time
from pln import LoRa, Log, BOARD, Utils, QTClient
import json
# import base64


BOARD.setup()
    
def main():
    try:
        while True:
            BOARD.receiveLed(False)
            LoRa.request()
            LoRa.wait()
            message = Utils.readMessage()
            print(message)
            
            LoRa.onReceive(Utils.receive_callback())

            # Log.info(f"received message {message} with RSII {LoRa.packetRssi()}")
            
            print("Packet status: RSSI = {0:0.2f} dBm | SNR = {1:0.2f} dB".format(
                LoRa.packetRssi(), LoRa.snr()))
            
            if isinstance(message, dict):
                raw_data = {
                    "voltase" : message["data"]["v"],
                    "current" : message["data"]["a"],
                    "latitude": message["data"]["lat"],
                    "longitude": message["data"]["lng"],
                    "timestamp": message["timestamp"]
                }
            else:
                raw_data= {
                    "error": "Invalid"
                }
            
            payload = json.dumps(raw_data)
            topic = "supersun/25810618-7a40-4df7-b16f-abbcfd7800fb/data"        
            
            msg_info = QTClient.publish(topic, payload)
            msg_info.wait_for_publish()

            Utils.cek_status_lora()
            print(f"---" * 30)
        
    except Exception as e:
        print(f"An error occurred: {e}")
        Log.exception(e)
        
    finally :
        BOARD.cleanup()
        # Log.close()
        

if __name__ == "__main__":
    main();
    


    
