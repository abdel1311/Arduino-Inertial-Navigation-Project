import requests
import json
from time import sleep
from itertools import count
from math import sin

session = requests.Session()
query = 'http://127.0.0.2:4500'
t = -0.05

while True:
    t += 0.05
    y=sin(t)
    data_and_stuff = {
        #'Connection':'Close',
        'Content-Type' : 'application/json',
        "t":t.__str__(),
        "x":"0.61",
        "y":y.__str__(),
        "z":"0.63"
    }   

    response = session.post(
    query,
    headers = data_and_stuff)

    print(f"response : {response}")
    sleep(0.05)        