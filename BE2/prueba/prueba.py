from pymongo import MongoClient

mongo_uri = 'mongodb://10.209.23.131:27017/'
mClient = MongoClient(mongo_uri)

db = mClient['prueba']
collection = db['prueba']

data = {"hola":"Hola Mundo"}

collection.insert_one(data)