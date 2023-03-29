import schedule
import time
import paho.mqtt.client as mqtt
import requests
import json

cli = mqtt.Client()
cli.connect("test.mosquitto.org", 1883, 60)


def feed():
    cli.publish("feed", "4 kg")
    print("Hay comida!")
    

# Programar la tarea diaria para ejecutarse todos los días a las 8 a.m.
schedule.every().day.at("22:20").do(feed)
schedule.every().day.at("22:21").do(feed)
schedule.every().day.at("22:22").do(feed)

# Bucle infinito para mantener el programa en ejecución
while True:
    # Comprobar si hay tareas programadas para ejecutar
    schedule.run_pending()
    # Esperar 1 segundo antes de volver a comprobar si hay tareas programadas
    time.sleep(1)
    
    
    