from flask import Flask, request, jsonify
from pymongo import MongoClient

mongo_uri = 'mongodb://localhost:27017'
mClient = MongoClient(mongo_uri)

db = mClient['BE2']
collection = db['alimentador']
app = Flask(__name__)

@app.route('/alimentadores', methods=['POST'])
def add_feeders():
    data = request.json
    print(data)
    collection.insert_one(data)
    return "received"

if __name__ == '__main__':
    app.run(debug=True, port=4000)
