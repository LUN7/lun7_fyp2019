from tkinter import *
from tkinter import Menu

class Application(Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.create_widgets()
        self.testlabel()
        self.grid()
        self.master.geometry("1280x720")
        self.menu = menu(Window)
        

    def testlabel(self):
        self.test = Label(self)
        self.test["text"] = "hello"
        self.test.grid(column=0, row=0)

    def create_widgets(self):
        self.hi_there = Button(self)
        self.hi_there["text"] = "Hello World\n(click me)"
        self.hi_there["command"] = self.say_hi
        self.hi_there.grid(column=1, row=1)

        self.quit = Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit["text"] = "click here to leave"        
        self.quit.grid(column=2, row=2)

    def say_hi(self):
        print("hi there, everyone!")

    def open():

root = Tk()
app = Application(master=root)
app.mainloop()
