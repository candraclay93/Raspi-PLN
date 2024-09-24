import paho.mqtt.client as mqtt
import time
import json
import datetime

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Terhubung ke broker MQTT!")
    else:
        print(f"Gagal terhubung, kode error {rc}")

def on_publish(client, userdata, mid):
    print(f"Data berhasil dikirim dengan id pesan: {mid}")


client = mqtt.Client()
client.on_connect = on_connect
client.on_publish = on_publish

# broker = "127.0.0.1"
broker = "alphaorionis.my.id"
client.username_pw_set("supersun", "12345678")
port = 1883

client.connect(broker, port, 60)
client.loop_start()

try:
    while True:
        raw_data = [
            {
                "voltage": "124",
                "current": "124",
                "latitude": "-7.0091521",
                "longitude": "110.441913",
                "timestamp": "1724638823546"
            },
            {
                "voltage": "124",
                "current": "124",
                "latitude": "-7.0091521",
                "longitude": "110.441913",
                "timestamp": "1724638823546"
            },
        ]
        
        payload = json.dumps(raw_data)
        topic = "supersun/25810618-7a40-4df7-b16f-abbcfd7800fb/data"
        
        result = client.publish(topic, payload)
        
        print(f"Payload yang dikirim: {payload}")
        
        time.sleep(20)

except KeyboardInterrupt:
    print("Program dihentikan oleh pengguna")

finally:
    client.loop_stop()
    client.disconnect()
