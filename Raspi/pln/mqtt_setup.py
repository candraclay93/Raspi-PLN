import paho.mqtt.client as mqtt
from .utils import Utils
from .loggers import Log

try:
    QTClient = mqtt.Client()
    QTClient.on_connect = Utils.on_connect
    QTClient.on_publish = Utils.on_publish

    broker = "alphaorionis.my.id"
    QTClient.username_pw_set("supersun", "12345678")
    port = 1883

    QTClient.connect(broker, port, 60)
    QTClient.loop_start()
except Exception as e:
    print(f"Error initializing MQTT client: {str(e)}")
    Log.error(f"Error initializing MQTT client: {str(e)}")