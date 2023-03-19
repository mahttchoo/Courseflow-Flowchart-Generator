# Must type in sudo apt install x11-xserver-utils

import tkinter as tk
from tkinter import ttk

window = tk.Tk()
window.geometry("800x500")
#window.attributes("-fullscreen", True)
window.title("Flowchart Graphics")

label = tk.Label(window, text="Year 1", font=('Comic Sans MS', 23))
label.pack(padx=200,pady=100)

canvas = tk.Canvas(window, width=700, height=300)
canvas.pack(pady=20)
rectangle = canvas.create_rectangle(0, 0, 400, 400, fill='blue')

window.mainloop()
export DISPLAY=:0.0
# export DISPLAY=:0.0

#file = open("output.txt", "r")
#print(file.read())