import requests
import json
import pandas as pd
import tkinter as tk
from tkinter import *
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

root = tk.Tk()
root.geometry("1060x480")

figure = plt.Figure(figsize=(5, 3), dpi=100)
ax = figure.add_subplot(111)
line = FigureCanvasTkAgg(figure, root)
line.get_tk_widget().pack(side=tk.LEFT, fill=tk.BOTH)
myLabel = Label(root, text=0, font=("Arial", 18), bd=1, relief="sunken")
myLabel.pack(pady=20)


def clock():
    global ax
    #figure.clear()

    url = "http://arduino.vhealth.me/Powers"
    r = requests.get(url, verify=False)
    page = json.loads(r.content)
    df = pd.DataFrame(page)
    # x = df.loc['mWh']
    # y = df.loc['time']
    # print(x)
    # print(y)
    ax = df.plot(x='hour', y='mWh', kind='line', legend=True, ax=ax, color='b', marker='o', fontsize=10)
    ax.set_title('mWh')
    ax.grid()

    r = requests.get("http://arduino.vhealth.me/Powers/live", verify=False)
    page = json.loads(r.content)
    lbl = "Live Power\t Total Power\t Time(utc)\n"+str(page[0]['livePower'])+"\t\t"+str(page[0]['totalPower'])+"\t\t"+page[0]['time'];
    myLabel["text"] = lbl
    root.after(1000, clock)

clock()

root.mainloop()
