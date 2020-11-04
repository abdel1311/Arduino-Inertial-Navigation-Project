# courtesy of CoreyMSchafer

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd

plt.style.use('fivethirtyeight')

def animate(i):
    data = pd.read_csv('data.csv')
    x=data['t']
    y1=data['y']

    plt.cla()

    plt.plot(x, y1, label='accélération y')

    plt.legend(loc='upper left')
    plt.tight_layout()

ani = FuncAnimation(plt.gcf(), animate, interval = 25)

plt.tight_layout()
plt.show()
