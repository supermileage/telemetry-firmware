from sseclient import SSEClient
from tkinter import *
import matplotlib
import math
import json
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import _thread

class main_window:

    def __init__(self, window):
        # This is our main Tk() object, here are all the setup steps (instance variables)
        self.window = window
        self.background = None
        self.x = []
        self.y = []

        self.start()    

    # This is our main window, with all our widgets present
    def start(self):
        # ---------------------- WIDGET SETUP ----------------------
        # Figure for our plot
        self.fig = Figure(figsize = (11, 6))
        self.temp_plot = self.fig.add_subplot(111)
        # Draw our plot elements
        self.temp_plot.set_xlabel("x")
        self.temp_plot.set_ylabel("y")
        self.temp_plot.set_ylim([-20, 20])
        self.fig_canvas = FigureCanvasTkAgg(self.fig, master=self.window)
   
        self.fig_canvas.get_tk_widget().pack()
        self.update()
        def grab_data():
            for msg in messages:
                print(msg)
                try:
                    nums = json.loads(msg.data)["data"].split(" ")
                    self.x.append(float(nums[0]))
                    self.y.append(float(nums[1]))
                except:
                    pass
        
        _thread.start_new_thread(grab_data, ())

    # This function updates our values; has a callback that is registered every update_timing_num
    def update(self):
        self.plot()     
        # Set up our callback
        self._after = self.window.after(200, self.update)        

    # This is our plot function
    def plot(self):
        # self.fig.clear()
        self.temp_plot.plot(self.x, self.y, color='blue')
        self.fig_canvas.draw()

# Create our root window
f = open("access-link.txt", "r")
print(f.read())
messages = SSEClient(f.read())
f.close()
window = Tk()
# Pass the root window into our main_window configuration
main_window(window)
# Run the main loop
window.mainloop()