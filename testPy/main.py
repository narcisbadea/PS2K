from matplotlib.animation import FuncAnimation
import requests
import json
import pandas as pd
import matplotlib.pyplot as plt


# root = tk.Tk()
# root.geometry("1060x480")

# figure = plt.Figure(figsize=(5, 3), dpi=100)
# ax = figure.add_subgraph(111)
# line = FigureCanvasTkAgg(figure, root)
# line.get_tk_widget().pack(side=tk.LEFT, fill=tk.BOTH)
# myLabel = Label(root, text=0, font=("Arial", 18), bd=1, relief="sunken")
# myLabel.pack(pady=20)


# def clock():
#     global ax
#     #figure.clear()

#     url = "http://arduino.vhealth.me/Powers"
#     r = requests.get(url, verify=False)
#     page = json.loads(r.content)
#     df = pd.DataFrame(page)
#     # x = df.loc['mWh']
#     # y = df.loc['time']
#     # print(x)
#     # print(y)
#     ax = df.graph(x='hour', y='mWh', kind='line', legend=True, ax=ax, color='b', marker='o', fontsize=10)
#     ax.set_title('mWh')
#     ax.grid()

#     r = requests.get("http://arduino.vhealth.me/Powers/live", verify=False)
#     page = json.loads(r.content)
#     lbl = "Live Power\t Total Power\t Time(utc)\n"+str(page[0]['livePower'])+"\t\t"+str(page[0]['totalPower'])+"\t\t"+page[0]['time'];
#     myLabel["text"] = lbl
#     root.after(1000, clock)

# clock()

# root.mainloop()

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
