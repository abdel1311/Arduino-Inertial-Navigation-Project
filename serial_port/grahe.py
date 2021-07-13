import numpy as np
import pylab as plt

path = 'data.dat'

DATA = np.loadtxt(path,delimiter=',')
X = DATA[:,0] # Temp
acc_b_x = DATA[:,1] 
acc_b_y = DATA[:,2] 
acc_b_z = DATA[:,3] 

acc_g_x_corec = DATA[:,4]
acc_g_y_corec = DATA[:,5]
acc_g_z_corec = DATA[:,6]

acc_g_x = DATA[:,7] 
acc_g_y = DATA[:,8]
acc_g_z= DATA[:,9]


plt.figure(figsize=(15,8))
plt.plot(X,acc_g_x_corec , color='r', marker='.', label=r'corrigé')
plt.plot(X,acc_g_y_corec , color='g', marker='.', label=r'corrigé')
plt.plot(X,acc_g_z_corec , color='b', marker='.', label=r'corrigé')

#plt.plot(X,acc_g_x , X,acc_g_y , X,acc_g_z , color='b', marker='', label=r'non corrigé')

plt.xlabel('t (s)')
plt.ylabel('Acc (m/s^2)')
plt.grid()
plt.legend()
plt.draw()
#plt.title(r'')
plt.savefig('graphe_data.png')