import tkinter as tk
import sys
import os
#import matplotlib
#matplotlib.use('Agg')
#from tkinter import ttk

#os.system("Xvfb :0 -screen 0 720x720x16 &")
#os.environ['DISPLAY'] = ":0.0"

#if os.environ.get('DISPLAY','') == '':
#    print('no display found. Using :0.0')
#    os.environ.__setitem__('DISPLAY', ':0.0')

window = tk.Tk()
window.geometry("900x700")
#window.attributes("-fullscreen", True)
window.title("Flowchart Graphics")

canvas = tk.Canvas(window, width=900, height=500)
canvas.pack()

label = tk.Label(window, text="Year 1", font=('Comic Sans MS', 23))
label.pack()

c = -100
with open('output.txt') as f:
    lines = f.readlines()
    for i in lines:
        if(i == 'Quarter\n'):
            label = tk.Label(window, text=("Quarter"), font=('Comic Sans MS', 23))
            label.pack()
            c = c + 120
        if(i == i):
            rectangle = canvas.create_rectangle(0, c, 200, 200, fill='blue')

window.mainloop()

#file = open("output.txt", "r")
#print(file.read())


#export DISPLAY=:0.0
#sudo apt install xvfb
#sudo apt install wsl