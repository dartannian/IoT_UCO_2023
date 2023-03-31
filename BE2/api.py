from flask import Flask, request, jsonify
from pymongo import MongoClient
from flask_cors import CORS

mongo_uri = 'mongodb://database:12345@localhost/?authSource=BE2'
mClient = MongoClient(mongo_uri)

db = mClient['BE2']
collection = db['alimentador']

app = Flask(__name__)
CORS(app)

@app.route('/alimentadores', methods=['POST'])
def add_feeders():
    data = request.json
    print(data)
    collection.insert_one(data)
    return "received"

if __name__ == '__main__':
    app.run(debug=True, port=4000,host='10.209.23.144')
