import numpy as np
import pylab as plt

path = 'data.dat'


DATA = np.loadtxt(path,delimiter=',')
X = DATA[:,0] 
x_1 = DATA[:,1]
y_1 = DATA[:,2]
z_1 = DATA[:,3]

x_2 = DATA[:,4]
y_2 = DATA[:,5]
z_2 = DATA[:,6]

x_3 = DATA[:,7]
y_3 = DATA[:,8]
z_3 = DATA[:,9]

Rxx = DATA[:,10]
Rxy = DATA[:,11]
Rxz = DATA[:,12]

Ryx = DATA[:,13]
Ryy = DATA[:,14]
Ryz = DATA[:,15]

Rzx = DATA[:,16]
Rzy = DATA[:,17]
Rzz = DATA[:,18]




plt.figure(figsize=(15,8))
plt.plot(X,x_1, color='r', marker=' ', label=r' body ')
#plt.plot(X,x_2, color='g', marker='+', label=r' world  ')
plt.plot(X,x_3, color='b', marker='.', label=r' world no correction ')
plt.xlabel('t (s)')
plt.ylabel('Acc (m/s^2)')
plt.grid()
plt.legend()
plt.draw()
plt.title(r'x')
plt.savefig('graphe_x.png')


plt.figure(figsize=(15,8))
plt.plot(X,y_1, color='r', marker=' ', label=r' body ')
#plt.plot(X,y_2, color='g', marker='+', label=r' world  ')
plt.plot(X,y_3, color='b', marker='.', label=r' world no correction ')
plt.xlabel('t (s)')
plt.ylabel('Acc (m/s^2)')
plt.grid()
plt.legend()
plt.draw()
plt.title(r'y')
plt.savefig('graphe_y.png')


plt.figure(figsize=(15,8))
plt.plot(X,z_1, color='r', marker=' ', label=r' corps ')
#plt.plot(X,z_2, color='g', marker='+', label=r' world  ')
plt.plot(X,z_3,'--', color='b', marker=' ', label=r' global')
plt.xlabel('t (s)')
plt.ylabel('Acc (m/s^2)')
plt.grid()
plt.legend()
plt.draw()
plt.title(r'z')
plt.savefig('graphe_z.png')





plt.figure(figsize=(15,8))
plt.plot(X,Rxx, color='r', marker=' ', label=r' x ')
plt.plot(X,Rxy, color='g', marker='+', label=r' y ')
plt.plot(X,Rxz, color='b', marker='.', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(' ')
plt.grid()
plt.legend()
plt.draw()
plt.title(r'Rx')
plt.savefig('graphe_Rx.png')


plt.figure(figsize=(15,8))
plt.plot(X,Ryx, color='r', marker=' ', label=r' x ')
plt.plot(X,Ryy, color='g', marker='+', label=r' y ')
plt.plot(X,Ryz, color='b', marker='.', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(' ')
plt.grid()
plt.legend()
plt.draw()
plt.title(r'Ry')
plt.savefig('graphe_Ry.png')


plt.figure(figsize=(15,8))
plt.plot(X,Rzx, color='r', marker=' ', label=r' x ')
plt.plot(X,Rzy, color='g', marker='+', label=r' y ')
plt.plot(X,Rzz, color='b', marker='.', label=r' z ')
plt.xlabel('t (s)')
plt.ylabel(' ')
plt.grid()
plt.legend()
plt.draw()
plt.title(r'Rz')
plt.savefig('graphe_Rz.png')

