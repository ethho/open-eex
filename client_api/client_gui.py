import tkinter as tk
import telnetlib
import socket
import time
import os
import sys
import threading

def makeform(root, fields):
    entries = {}
    for field in fields:
        row = tk.Frame(root)
        lab = tk.Label(row, width=22, text=field+": ", anchor='w')
        ent = tk.Entry(row)
        ent.insert(0,"0")
        row.pack(side = tk.TOP, fill = tk.X, padx = 5 , pady = 5)
        lab.pack(side = tk.LEFT)
        ent.pack(side = tk.RIGHT, expand = tk.YES, fill = tk.X)
        entries[field] = ent
    return entries

def display_portfolio(root,fields_dict):
    entries = {}
    
    for field,value in fields_dict.items():
        row = tk.Frame(root)
        lab = tk.Label(row, width=22, text=field+": "+str(value), anchor='w')
        row.pack(side = tk.BOTTOM, fill = tk.X, padx = 5 , pady = 5)
        lab.pack(side = tk.LEFT)
    return entries

def get_message(c):
    while True:
        msg = c.client.read_until(str.encode("\r\n"), timeout=c.msg_timeout)
        msg = msg.decode()
        print(msg)
        if c.ended.is_set():
            break
        time.sleep(1)

class ClientWindow:
    def __init__(self, title = "Open EEX", port = 6670, capital = 100):
        self.capital = capital
        self.port = port
        self.portfolio = {"Capital": self.capital}
        self.msg_timeout = 0.1
        self.fields = ("Ticker", "Bid or Ask", "Price per share", "Volume")
        self.client = self.create_connection()

        self.window = tk.Tk()
        self.window_title = title
        self.window.resizable(width = False, height = False)
        self.window.title(self.window_title)
        self.window.geometry('400x400')
        self.window.configure(background = "grey")
        ents = makeform(self.window, self.fields)
        self.window.bind('<Return>', (lambda event, e = ents: tk.fetch(e)))
        
        b1 = tk.Button(self.window, text = 'Submit Order',
            command=(lambda e = ents: self.process_order(e)))
        b1.pack(side = tk.LEFT, padx = 5, pady = 5)
        b2 = tk.Button(self.window, text = 'Exit',
            command=(lambda e = ents: self.disconnect()))
        b2.pack(side = tk.LEFT, padx = 5, pady = 5)
        _ = display_portfolio(self.window, self.portfolio)
        
        self.msg_thread = threading.Thread(target=get_message, args=[self])
        self.msg_thread.start()
        self.ended = threading.Event()
        
        self.window.mainloop()
                
    def create_connection(self):
        tries = 3
        while tries > 0:
            try:
                self.client = telnetlib.Telnet("localhost", str(self.port))
                self.client.set_debuglevel(100)
                break
            except Exception as e:
                print(e)
                tries -= 1
                time.sleep(0.1)
        return self.client

    def disconnect(self):
        print("Over")
        self.ended.set()
        self.msg_thread.join()
        self.client.close()
        self.window.quit()
    
    def process_order(self, entries):
        ticker = entries["Ticker"].get()
        if entries["Bid or Ask"].get().lower() == "bid":
            odr_str = "BID"
        else:
            odr_str = "ASK"
        
        price_per_share = entries["Price per share"].get()
        vol = entries["Volume"].get()
        
        send_str = f"{ticker}:{odr_str}:{price_per_share}:{vol}\r\n"
        
        print(send_str)
        self.client.write(send_str.encode("ascii"))
        
if __name__ == "__main__":
    c = ClientWindow()
        