import paho.mqtt.client as mqtt
import requests
import json
from pymongo import MongoClient

url_chat_bot = "http://10.209.23.135:3030/send_alarm"
url_be_1 = "http://10.209.23.119:8080/api/feeder/jowjer2"
mongo_uri = 'mongodb://database:12345@localhost/?authSource=BE2'
client = mqtt.Client()
monClient = MongoClient(mongo_uri)
db = monClient["BE2"]
collection = db["comunicaciones"]

data = {"phone":"3116033166","feeder":"fd01"}

def on_connect(client, userdata, flags, rc):
    print("Se conectó con mqtt: "+str(rc))
    client.subscribe("alarm")
    client.subscribe("weight")

def clean_payload(payld):
    payld = payld[2:len(payld)-1]
    return payld

def str_to_json(message):
    return message.replace("'", '"')

def on_message(client, userdata, msg):
    #Se guardan el payload y el topic del mensaje en las variables payload y topic
    payload = clean_payload(str(msg.payload))
    topic = str(msg.topic)

    #Se guarda una versión inicial del json que se guardará dentro de la bd pero con comillas simples 
    bd_data = str({"topic":topic,"mensaje":payload})

    #Se utiliza el metodo str_to_json para reemplazar las comillas simples por comillas dobles 
    #Esto con el fin de que el json tenga el formato correcto
    bd_data = (str_to_json(bd_data))
    print(bd_data)
    collection.insert_one(json.loads(bd_data))
    if msg.topic == "alarm":
        response = requests.post(url_chat_bot, json=data)
        #client.publish("feed","5 kg")
        print(response)
        response2 = requests.get(url_be_1)
        if response2.status_code == 200:
            print(response2.json)
    #elif msg.topic == "weight":
    #    print("Actualmente hay esta cantidad en el alimentador: " + str(msg.payload))



client.on_connect = on_connect
client.on_message = on_message

client.connect("test.mosquitto.org", 1883, 60)
client.loop_forever()