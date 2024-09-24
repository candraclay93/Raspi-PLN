import time
from .board_config import BOARD
from .loraSetup import LoRa
import json
import re

class Utils:
    
    @staticmethod
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Terhubung ke broker MQTT!")
        else:
            print(f"Gagal terhubung, kode error {rc}")
            
    @staticmethod
    def on_publish(client, userdata, mid):
        print(f"Data berhasil dikirim dengan id pesan: {mid}")
        
    @staticmethod
    def receive_callback():
        BOARD.receiveLed(True)
        # Log.info()
        time.sleep(.3)
        
    @staticmethod
    def cek_status_lora():
        status = LoRa.status()
        if status == LoRa.STATUS_CRC_ERR:
            print("CRC error")
            # Log.error("CRC error")
        elif status == LoRa.STATUS_HEADER_ERR:
            print("Packet header error")
            # Log.error("Packet header error")
            
    @staticmethod
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