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



while True:
    data = consume_api(url)
    if(is_time_to_feed("03:28")):
        print("Alimenteme pues, guevón...")
    else:
        print("Toy llenito, marica")
    time.sleep(60)





    
    
    