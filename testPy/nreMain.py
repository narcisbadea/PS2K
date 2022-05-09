
from tkinter import *
from matplotlib import pyplot as plt
import requests
import json
import pandas as pd
import tkinter as tk
from tkinter import *
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

Top = Tk()
Top.geometry("850x450")

x = []
y = []
func_id = None
figure = plt.Figure(figsize=(5, 3), dpi=100)
ax = figure.add_subplot(111)
line = FigureCanvasTkAgg(figure, Top)
line.get_tk_widget().pack(side=tk.LEFT, fill=tk.BOTH)

def read_data():
    global func_id
    url = "http://arduino.vhealth.me/Powers"
    r = requests.get(url, verify=False)

    page = json.loads(r.content)
    # print(page)

    df = pd.DataFrame(page)
    # print (df)
    x = df[df.columns[0]].to_numpy()

    y = df[df.columns[1]].to_numpy()

    ax.clear();
    ax.plot(x, y, 'r-')  # Returns a tuple of line objects, thus the comma
    ax.set_title('Wh')
    ax.grid()

    func_id = Top.after(1000, read_data)

def close_plot():
    global func_id
    #to no longer update the plot
    Top.after_cancel(func_id)
    plt.close()
    global x, y
    del x[:]
    del y[:]


Button(Top, text='Read', command=read_data).pack()

mainloop()