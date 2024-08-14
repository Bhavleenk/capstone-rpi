import pyrebase
import pandas as pd
import numpy as np

config = {
  "apiKey": "database-secret",
  "authDomain": "project-id.firebaseapp.com",
  "databaseURL": "https://database-url.firebaseio.com",
  "storageBucket": "project-id.appspot.com"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()
