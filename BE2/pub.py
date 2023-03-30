import schedule
import time
import paho.mqtt.client as mqtt
import requests
import json
from pymongo import MongoClient

cli = mqtt.Client()
cli.connect("test.mosquitto.org", 1883, 60)

url = "http://worldtimeapi.org/api/timezone/America/Bogota"

mongo_uri = 'mongodb://localhost:27017'
mClient = MongoClient(mongo_uri)

db = mClient['BE2']
collection = db['alimentador']

def get_hours(data):
    return str(data["datetime"][11:16])

def consume_api(url):
    response = requests.get(url)
    if response.status_code == 200:
        return response.json()

def get_time_now(data):
    return str(get_hours(data))

#El parametro schedule debe tener un formato HH:MM, y debe ser un formato de 24 horas        
def is_time_to_feed(schedule):
    return get_time_now(consume_api(url)) == schedule

#Este método se utiliza para consultar el horario que se tiene definido en el alimentador para "alimentar"
def get_schedule_from_feeder(serial):
    feeder =  collection.find_one({"serial":serial})
    return feeder["horario"]

#Este método valida si se requiere "alimentar" los alimentadores que se encuentren registrados en la bd
def validate_feeders(col):
    for a in col:
        if(is_time_to_feed(get_schedule_from_feeder(a["serial"]))):
            cli.publish("feed", str(a["cantidad"]))#a["cantidad"]
            print("E alimentador con serial", a["serial"], " acaba de llenarse con", a["cantidad"])
        else:
            print("Todavía hay alimento")

while True:
    data = consume_api(url)
    validate_feeders(collection.find())
    time.sleep(60)





    
    
    