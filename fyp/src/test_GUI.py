import tkinter as tk

LARGE_FONT = ("Verdana", 12)

class FYP(tk.Tk):

    def __init__(self, *args, **kwargs):

        tk.Tk.__init__(self, *args, **kwargs)
        container = tk.Frame(self)

        container.pack(side="bottom", fill="both", expand = True)

        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        menubar = tk.Menu(container)
        filemenu = tk.Menu(menubar, tearoff=0)
        filemenu.add_command(label="exit", command=quit)
        menubar.add_cascade(label="File", menu=filemenu)

        tk.Tk.config(self, menu=menubar)
        
        self.frames = {}

        for f in (PageA, PageB):
            frame = f(container, self)
            self.frames[f] = frame
            frame.grid(row=0, column=0, sticky="nsew")
        self.show_frame(PageA)

    def show_frame(self, cont):

        frame = self.frames[cont]
        frame.tkraise()

class PageA(tk.Frame):
    
    def __init__(self, parent, controller):
        tk.Frame.__init__(self,parent)
        label = tk.Label(self, text="Control and Monitor", font=LARGE_FONT)
        label.grid(column = 0 , row = 1 )
        buttom = tk.Button(self, text="Train", command=lambda:controller.show_frame(PageB))
        buttom.grid(column = 0 , row = 0)

class PageB(tk.Frame):
    
    def __init__(self, parent, controller):
        tk.Frame.__init__(self,parent)
        label = tk.Label(self, text="Train", font=LARGE_FONT)
        label.grid(column = 0 , row = 1)
        buttom = tk.Button(self, text="Control", command=lambda:controller.show_frame(PageA))
        buttom.grid(column = 0 , row = 0)

app = FYP()
app.geometry("1280x720")
app.mainloop()