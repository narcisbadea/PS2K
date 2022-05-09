from matplotlib.animation import FuncAnimation
import requests
import json
import pandas as pd
import matplotlib.pyplot as plt

figure = plt.figure(figsize=(10, 5), dpi=100, facecolor='#DEDEDE')
graph = figure.add_subplot(1, 2, 1)
label = figure.add_subplot(1, 2, 2)
label.axis('off')

graph.grid()
graph.set_title('Generated Power')
graph.set_ylabel('P[mW]')
graph.set_xlabel('Hour')

x = []
y = []
def animate(i):
    url = "http://arduino.vhealth.me/Powers"
    req = requests.get(url, verify=False)
    page = json.loads(req.content)
    df = pd.DataFrame(page)
    y = df.loc[:, "mWh"].to_list()
    x = df.loc[:, "hour"].to_list()

    req = requests.get("http://arduino.vhealth.me/Powers/live", verify=False)
    page = json.loads(req.content)
    df = pd.DataFrame(page)
    table = label.table(cellText=df.values, cellLoc = 'center', colLabels=df.columns, loc = 'center', edges = 'open')
    
    # x.append(next(index))
    # y.append(random.randint(0,10))
    graph.plot(x, y)

ani = FuncAnimation(plt.gcf(), animate, interval = 1000)

plt.show()
