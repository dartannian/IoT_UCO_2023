import paho.mqtt.client as mqtt
import requests
import json


def on_connect(client, userdata, flags, rc):
    print("Se conectó con mqtt: "+str(rc))
    client.subscribe("alarm")
    client.subscribe("weight")


def on_message(client, userdata, msg):
    if msg.topic == "alarm":
        print("Señor usuario, el cuido se está acabando")
        client.publish("feed", "5 Kg")
    elif msg.topic == "weight":
        print("Actualmente hay esta cantidad en el alimentador: " + str(msg.payload))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("test.mosquitto.org", 1883, 60)
client.loop_forever()