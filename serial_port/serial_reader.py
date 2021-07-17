import serial, keyboard, math
import numpy as np
import pylab as plt
from datetime import datetime

class fragile(object):
    class Break(Exception):
      """Break out of the with statement"""

    def __init__(self, value):
        self.value = value

    def __enter__(self):
        return self.value.__enter__()

    def __exit__(self, etype, value, traceback):
        error = self.value.__exit__(etype, value, traceback)
        if etype == self.Break:
            return True
        return error

serial_port = '/dev/cu.usbmodem14101'
baud_rate = 9600
path = "%s.dat" % (str(datetime.now())[5:19])

ser = serial.Serial(serial_port, baud_rate)
ser.setDTR(True)
ser.timeout = 1

with fragile(open(path, 'w+')) as file:
    #file.writelines("Temps, Acc,,, Gyro,,, Magn,,,,vit_ang_x")
    while True:
        line = ser.readline()
        l =line.decode(encoding='UTF-8',errors='strict')
        print(l)
        if l!='':
            file.writelines(l[:-5] +"\n")
        if keyboard.is_pressed("p"):
            raise fragile.Break
            break


def plot_grid_legend_and_draw(plt):
    plt.grid()
    plt.legend()
    plt.draw()

DATA = np.loadtxt(path,delimiter=',')
X = DATA[:,0] # Temps

x_1 , y_1 , z_1 = DATA[:,1], DATA[:,2], DATA[:,3]
x_2 , y_2 , z_2 = DATA[:,4], DATA[:,5], DATA[:,6]
x_3 , y_3 , z_3 = DATA[:,7], DATA[:,8], DATA[:,9]

Rxx , Rxy , Rxz = DATA[:,10], DATA[:,11], DATA[:,12]
Ryx , Ryy , Ryz = DATA[:,13], DATA[:,14], DATA[:,15]
Rzx , Rzy , Rzz = DATA[:,16], DATA[:,17], DATA[:,18]

reste = DATA[:,19]

rot_x , rot_y , rot_z = DATA[:,20], DATA[:,21], DATA[:,22]
vit_x , vit_y , vit_z = DATA[:,23], DATA[:,24], DATA[:,25]
pos_x , pos_y , pos_z = DATA[:,26], DATA[:,27], DATA[:,28]

vit_x_no_corec, vit_y_no_corec, vit_z_no_corec = DATA[:,29], DATA[:,30], DATA[:,31]
pos_x_no_corec, pos_y_no_corec, pos_z_no_corec = DATA[:,32], DATA[:,33], DATA[:,34] 

plt.figure(figsize=(15,8))
plt.plot(X,x_1,'--' , color='r', marker=' ', label=r' Coordonnées dans le repère corps ')
plt.plot(X,x_3, color='b', marker='.', label=r' Coordonnées dans le repère monde ')
plt.xlabel('t (s)')
plt.ylabel('Accélération (m/s^2)')
plot_grid_legend_and_draw(plt)
plt.savefig('acc_x_corps_monde.png')

plt.figure(figsize=(15,8))
plt.plot(X,y_1,'--', color='r', marker=' ', label=r' Coordonnées dans le repère corps ')
plt.plot(X,y_3, color='b', marker='.', label=r' Coordonnées dans le repère monde ')
plt.xlabel('t (s)')
plt.ylabel('Accélération (m/s^2)')
plot_grid_legend_and_draw(plt)
plt.savefig('acc_y_corps_monde, math.png', keyboard)

plt.figure(figsize=(15,8))
plt.plot(X,z_3, color='b', marker=' ', label=r' Coordonnées dans le repère monde ')
plt.ylabel('Accélération (m/s^2)')
plot_grid_legend_and_draw(plt)
plt.savefig('acc_z_corps_monde.png')

plt.figure(figsize=(15,8))
plt.plot(X,Rxx, color='r', marker=' ', label=r' x ')
plt.plot(X,Rxy, color='g', marker='+', label=r' y ')
plt.plot(X,Rxz, color='b', marker='.', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(' ')
plot_grid_legend_and_draw(plt)
plt.title(r'Rx')
plt.savefig('graphe_Rx.png')

plt.figure(figsize=(15,8))
plt.plot(X,Ryx, color='r', marker=' ', label=r' x ')
plt.plot(X,Ryy, color='g', marker='+', label=r' y ')
plt.plot(X,Ryz, color='b', marker='.', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(' ')
plot_grid_legend_and_draw(plt)
plt.title(r'Ry')
plt.savefig('graphe_Ry.png')

plt.figure(figsize=(15,8))
plt.plot(X,Rzx, color='r', marker=' ', label=r' x ')
plt.plot(X,Rzy, color='g', marker='+', label=r' y ')
plt.plot(X,Rzz, color='b', marker='.', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(' ')
plot_grid_legend_and_draw(plt)
plt.title(r'Rz')
plt.savefig('graphe_Rz.png')

plt.figure(figsize=(15,8))
plt.plot(X,x_1,"--" ,color='r', marker=' ', label=r' x corps')
plt.plot(X,y_1,"--" ,color='g', marker=' ', label=r' y corps ')
plt.plot(X,z_1,"--" ,color='b', marker=' ', label=r' z corps')
plt.plot(X,x_3, color='darkred', marker=' ', label=r' x monde')
plt.plot(X,y_3, color='darkgreen', marker=' ', label=r' y monde')
plt.plot(X,z_3, color='darkblue', marker=' ', label=r' z monde')
plt.xlabel('t (s)')
plt.ylabel(' ')
plot_grid_legend_and_draw(plt)
plt.savefig('all.png')

plt.figure(figsize=(15,8))
plt.plot(X,reste ,color='r', marker=' ', label=r' ')
plt.xlabel('t (s)')
plt.ylabel(' % ')
plot_grid_legend_and_draw(plt)
plt.savefig('reste.png')

plt.figure(figsize=(15,8))
plt.plot(X,x_1 ,color='r', marker='+', label=r' ')
plt.plot(X,y_1 ,color='g', marker='+', label=r' ')
plt.plot(X,z_1 ,color='b', marker='+', label=r' ')
plt.xlabel('t (s)')
plt.ylabel(r'Accélération (m/s^2)')
plot_grid_legend_and_draw(plt)
plt.savefig('Accelerometre.png')

plt.figure(figsize=(15,8))
plt.plot(X,rot_x ,'--' ,color='r', marker='+', label=r' x ')
plt.plot(X,rot_y ,'--' ,color='g', marker='+', label=r' y ')
plt.plot(X,rot_z ,'--'  ,color='b', marker='+', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(r'Vitesse angulaire °/s')
plot_grid_legend_and_draw(plt)
plt.title(r' mesure gyromètrique en fonction du temps ')
plt.savefig('gyrometre.png')

plt.figure(figsize=(15,8))
plt.plot(X,x_2 ,'--' ,color='r', marker=' ', label=r' x ')
plt.plot(X,x_3 ,'--' ,color='r', marker=' ', label=r' x ')
plt.plot(X,y_2 ,'--' ,color='g', marker=' ', label=r' y ')
plt.plot(X,y_3 ,'--' ,color='g', marker=' ', label=r' y ')
plt.plot(X,z_2 ,'--'  ,color='b', marker=' ', label=r' z ')
plt.plot(X,z_3 ,'--'  ,color='b', marker=' ', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(r'Accélération en m/s^2')
plot_grid_legend_and_draw(plt)
plt.title(r' Accélération en fonction du temps ')
plt.savefig('Data_acc.png')

plt.figure(figsize=(15,8))
plt.plot(X,x_2 ,'--' ,color='r', marker=' ', label=r' x ')
plt.plot(X,y_2 ,'--' ,color='g', marker=' ', label=r' y ')
plt.plot(X,z_2 ,'--'  ,color='b', marker=' ', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(r'Accélération en m/s^2')
plot_grid_legend_and_draw(plt)
plt.title(r' Accélération en fonction du temps ')
plt.savefig('Data_acc_no_corec.png')

plt.figure(figsize=(15,8))
plt.plot(X,vit_x ,color='r', marker='+', label=r' x ')
plt.plot(X,vit_x_no_corec  ,'--' ,color='r', marker=' ', label=r' x ')
plt.plot(X,vit_y ,color='g', marker='+', label=r' y ')
plt.plot(X,vit_y_no_corec  ,'--' ,color='g', marker=' ', label=r' x ')
plt.plot(X,vit_z ,color='b', marker='+', label=r' z ')
plt.plot(X,vit_z_no_corec  ,'--' ,color='b', marker=' ', label=r' x ')
plt.xlabel('t (s)')
plt.ylabel(r'Vitesse m/s')
plot_grid_legend_and_draw(plt)
plt.title(r' Vitesse en fonction du temps ')
plt.savefig('Data_vit.png')

plt.figure(figsize=(15,8))
plt.plot(X,vit_x_no_corec  ,'--' ,color='r', marker=' ', label=r' x ')
plt.plot(X,vit_y_no_corec  ,'--' ,color='g', marker=' ', label=r' x ')
plt.plot(X,vit_z_no_corec  ,'--' ,color='b', marker=' ', label=r' x ')
plt.xlabel('t (s)')
plt.ylabel(r'Vitesse m/s')
plot_grid_legend_and_draw(plt)
plt.title(r' Vitesse en fonction du temps ')
plt.savefig('Data_vit_no_corec.png')

plt.figure(figsize=(15,8))
plt.plot(X,pos_x ,color='r', marker=' ', label=r' x ')
plt.plot(X,pos_x_no_corec  ,'--' ,color='r', marker=' ', label=r' x ')
plt.plot(X,pos_y ,color='g', marker=' ', label=r' y ')
plt.plot(X,pos_y_no_corec  ,'--' ,color='g', marker=' ', label=r' x ')
plt.plot(X,pos_z ,color='b', marker=' ', label=r' z ')
plt.plot(X,pos_z_no_corec  ,'--' ,color='b', marker=' ', label=r' x ')
plt.xlabel('t (s)')
plt.ylabel(r'Position m ')
plt.title(r' Position en fonction du temps ')
plt.savefig('Data_pos.png')

plt.figure(figsize=(15,8))
plt.plot(X,pos_x_no_corec  ,'--' ,color='r', marker=' ', label=r' x ')
plt.plot(X,pos_y_no_corec  ,'--' ,color='g', marker=' ', label=r' x ')
plt.plot(X,pos_z_no_corec  ,'--' ,color='b', marker=' ', label=r' x ')
plt.xlabel('t (s)')
plt.ylabel(r'Position m ')
plot_grid_legend_and_draw(plt)
plt.title(r' Position en fonction du temps ')
plt.savefig('Data_pos_no_corec.png')