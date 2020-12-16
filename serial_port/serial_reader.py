import serial
from datetime import datetime

serial_port = '/dev/cu.usbmodem14101'
baud_rate = 9600
path = "%s.csv" % (str(datetime.now())[5:19])

ser = serial.Serial(serial_port, baud_rate)
ser.setDTR(True)
ser.timeout = 1
with open(path, 'w+') as file:
    file.writelines("Temps, Acc,,, Gyro,,, Magn,,,,vit_ang_x")
    while True:
        line = ser.readline()
        print(line.decode(encoding='UTF-8',errors='strict'))
        file.writelines(line.decode(encoding='UTF-8',errors='strict'))