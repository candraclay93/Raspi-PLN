import paho.mqtt.client as mqtt
# import time
# import json
from .utils import Utils


QTClient = mqtt.Client()
QTClient.on_connect = Utils.on_connect
QTClient.on_publish = Utils.on_publish

broker = "alphaorionis.my.id"
QTClient.username_pw_set("supersun", "12345678")
port = 1883

QTClient.connect(broker, port, 60)
QTClient.loop_start()
