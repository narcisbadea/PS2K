import json
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib import pyplot as plt
import pandas as pd
import requests
import PySimpleGUI as sg

_VARS = {
        'window': False,
        'fig_agg': False,
        'pltFig': False,
        'graph': False
        }

def draw_figure(canvas, figure):
    figure_canvas_agg = FigureCanvasTkAgg(figure, canvas)
    figure_canvas_agg.draw()
    figure_canvas_agg.get_tk_widget().pack(side='top', fill='both', expand=1)
    return figure_canvas_agg

def get_table():
    req = requests.get("http://arduino.vhealth.me/Powers/live", verify=False)
    page = json.loads(req.content)
    df = pd.DataFrame(page)

    return df

df = get_table()
headings = list(df)
data = df.to_numpy().tolist()
col_widths = list(map(lambda x:len(x)+2, headings)) # find the widths of columns in character.
max_col_width = len('ParameterNameToLongToFitIntoAColumn')  # Set max midth of all columns of data to show

font = ('Courier New', 11)

layout = [
    [sg.Table(
        values=data,                # Empty data must be with auto_size_columns=False
        headings=headings,
        auto_size_columns=True,
        hide_vertical_scroll=True,  # Not required if no more rows of data
        def_col_width=20,
        num_rows=1,
        col_widths=col_widths,      # Define each column width as len(string)+2
        font=font,                  # Use monospaced font for correct width
        header_background_color='#b8e0d2',
        header_text_color='#000000',
        alternating_row_color='#d6eadf',
        text_color='#000000',
        key='-TABLE-')],
    [sg.Canvas(
        size=(700,625), 
        key='-CANVAS-'
        )],
    [sg.Button('EXIT')]
]
_VARS['window'] = sg.Window('Title', layout, finalize=True, size=(800,700), element_justification='center', margins=(50, 0), background_color='#95b8d1')

def create_graph():
    px = 1/plt.rcParams['figure.dpi']
    _VARS['pltFig'] = plt.figure(figsize=(700*px, 625*px), dpi=100, facecolor='#b8e0d2')
    _VARS['graph'] = _VARS['pltFig'].add_subplot(1, 1, 1)

    _VARS['graph'].grid()
    _VARS['graph'].set_title('Generated Power')
    _VARS['graph'].set_ylabel('P[mW]')
    _VARS['graph'].set_xlabel('Hour')

    _VARS['fig_agg'] = draw_figure(_VARS['window']['-CANVAS-'].TKCanvas, _VARS['pltFig'])

    
def update_graph():
    req = requests.get("http://arduino.vhealth.me/Powers", verify=False)
    page = json.loads(req.content)
    df = pd.DataFrame(page)
    y = df.loc[:, "mWh"].to_list()
    x = df.loc[:, "hour"].to_list()

    _VARS['graph'].cla()
    _VARS['graph'].grid()
    _VARS['graph'].set_title('Generated Power')
    _VARS['graph'].set_ylabel('P[mW]')
    _VARS['graph'].set_xlabel('Hour')
    _VARS['graph'].autoscale()
    _VARS['graph'].plot(x, y, '-kx')
    _VARS['fig_agg'].draw()


char_width = sg.Text.char_width_in_pixels(font)     # Get character width in pixel
table = _VARS['window']['-TABLE-']
table_widget = table.Widget
table.expand(expand_x=True, expand_y=True)          # Expand table in both directions of 'x' and 'y'
for cid in headings:
    table_widget.column(cid, stretch=True)          # Set column stretchable when window resize

create_graph()
update_graph()
while True:
    event, values = _VARS['window'].read(timeout=1000)
    if event == sg.WIN_CLOSED or event == 'EXIT':
        break
    print(data)
    df = get_table()
    data = df.to_numpy().tolist()
    str_data = [str(x) for x in data[0]]
    all_data = [headings] + [str_data]                   
    # All data include headings
    # Find width in pixel and 2 extra characters for each column
    col_widths = [min([max(map(len, columns)), max_col_width])*char_width for columns in zip(*all_data)]
    table.update(values=data)                   # update all new data
    # Redraw table to update new size of table if horizontal scrollbar not used, care if widget too large to fit your window or screen.
    table_widget.pack_forget()
    for cid, width in zip(headings, col_widths):    # Set width for each column
        table_widget.column(cid, width=width, stretch=True)
    table_widget.pack(side='left', fill='both', expand=True)    # Redraw table
    
    update_graph()

_VARS['window'].close()
