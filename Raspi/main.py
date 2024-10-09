import time
import json
from systemd import journal
from pln import LoRa, Log, BOARD, Utils, QTClient


BOARD.setup()


def sendToMqttServer(message):
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
      "error": "Invalid CRC"
    }

  payload = json.dumps([raw_data])
  topic = "supersun/25810618-7a40-4df7-b16f-abbcfd7800fb/data"    

  msg_info = QTClient.publish(topic, payload)
  msg_info.wait_for_publish()

  
def main():  
  try:
    while True:
      for index, idSlave in enumerate(Utils.idSlaveLora):
        counter = 1
        while counter <= 2:
          data_to_send = {
            "from": Utils.idLora,
            "to": idSlave,
            "data": {"msg" : "request"}
            }
          json_data = json.dumps(data_to_send)
          json_data + '\0'
          
          print(f"send request {counter} to id slave {idSlave} with message: ")
          print(json_data)
          
          messageList = list(Utils.encodePayload(json_data))
          for i in range(len(messageList)) : messageList[i] = ord(messageList[i])
          
          LoRa.beginPacket()
          LoRa.write(101)
          LoRa.write(messageList, len(messageList))
          LoRa.endPacket()
          
          print(f"-" * 50)
          LoRa.wait()

          print("Transmit time: {0:0.2f} ms | Data rate: {1:0.2f} byte/s".format(LoRa.transmitTime(), LoRa.dataRate()))
          print(f"-" * 50)
          
          time.sleep(1)

          start_time = time.time()
          print(f"Waiting {counter} for a response from the slave ID : {idSlave}")
          while True:
            BOARD.receiveLed(False)
            LoRa.request(4000)
            LoRa.wait()
            status = LoRa.status()
            if status == LoRa.STATUS_RX_DONE :
              response = Utils.readMessage()
    
              if(response is not None):
                packet_index = response.get("packetIndex")
                total_packets = response.get("totalPackets", 0)
                
                if packet_index is not None and packet_index <= total_packets:
                  start_time = time.time()
                  
                  print(f"{idSlave}, receive {response['packetIndex']} packet, Total : {response['totalPackets']}, with RSSI: {response['rssi']}, SNR: {response['snr']}")
                  
                  if(response["message"] is not None):  
                    message = response["message"]
                    print(message)
                    Log.info(f"Received message: {message}")
                    Log.info(f"with RSSI: {response['rssi']}, SNR: {response['snr']}")
                    sendToMqttServer(message)
                    print(f"-"* 30)
                    counter += 2
                    break
            if time.time() - start_time > 5:
              break
            LoRa.onReceive(Utils.receive_callback())
          time.sleep(5)
          print(f"Waiting {counter} for a response from the slave ID : {idSlave} timeout")
          print(f"-" * 50)
          counter += 1
        print("finished changing the next ID")
        print(f"=" * 70)
  except Exception as e:
    print(f"An error occurred: {e}")
    Log.exception(e)
    
  finally :
    BOARD.cleanup()
    Log.close()
    

if __name__ == "__main__":
  journal.write("sent to journal")
  main();
  


  
