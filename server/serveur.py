import http.server
import requests
import csv
from itertools import count

class Handler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        self.send_response(201, message='we good chief')
        self.end_headers()

        #print(self.headers)
        t = self.headers['t']
        x = self.headers['x']
        y = self.headers['y']
        z = self.headers['z'] 

        #print(f"accélération en z = {self.liste_accel[self.instant_t]['z']}")
        write_data(t, y)

def write_data(t, y):
    t = float(t)
    y = float(y)
    fieldnames = ['t', 'y']
    if(t == 0.00):
        with open('data.csv', 'w') as csv_file:
            csv_writer = csv.DictWriter(csv_file, fieldnames = fieldnames, lineterminator='\n')
            csv_writer.writerow({"t":"t", "y":"y"})

    with open('data.csv', 'a') as csv_file:
        csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames, lineterminator='\n')
        info = {
            "t":t,
            "y":y
        }
        csv_writer.writerow(info)
    
if __name__ == '__main__':
    print('serving on port 4500')
    httpd = http.server.HTTPServer(('127.0.0.2',4500), Handler)
    httpd.serve_forever()

