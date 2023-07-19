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

def display_portfolio(root,fields_dict,elts,labs):    
    for field,value in fields_dict.items():
        if field == "Capital":
            txt = field+": "+str(value)
        else:
            txt = f"{field}: {value} shares"
        
        if field not in elts.keys():
            elts[field] = tk.Frame(root)  
            labs[field] = tk.Label(elts[field], width=50, text=txt, anchor='w')
            elts[field].pack(side = tk.BOTTOM, fill = tk.X, padx = 5 , pady = 5)  
            labs[field].pack(side = tk.LEFT)
        else:
            labs[field]["text"] = txt
        if value < 1e-6 and field != "Capital":
            #If no securities are owned remove that row
            elts[field].destroy()
    return

def get_message(c):
    while True:
        msg = c.client.read_until(str.encode("\r\n"), timeout=c.msg_timeout)
        msg = msg.decode()        
        if c.ended.is_set():
            break
        #Received order, now update portfolio
        try:
            elts = msg.split(":")
            #Ticker is in the portfolio, now to update it
            c.portfolio[f"{elts[0]}: {elts[1]}"] = (elts[0],elts[1],elts[2],elts[3])
            sign = 1
            if elts[1] == "ASK":
                sign = -1
            c.securities["Capital"] = c.securities["Capital"] + sign * float(elts[2]) * float(elts[3])
            c.securities[elts[0]] = c.securities[elts[0]] + sign* float(elts[3])
            print(c.portfolio)
            display_portfolio(c.window, c.securities,c.elts, c.labs) 
        except Exception as e:
            pass
        time.sleep(1)
        

class ClientWindow:
    def __init__(self, title = "Open EEX", port = 6671, capital = 100, eps = 1e-6):
        self.capital = capital
        self.port = port
        self.portfolio = {"Capital": self.capital}
        self.securities = {"Capital": self.capital}
        self.elts = {}
        self.labs = {}
        self.msg_timeout = 0.1
        self.fields = ("Ticker", "Bid or Ask", "Price per share", "Volume")
        self.client = self.create_connection()
        self.eps = 1e-6

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
        display_portfolio(self.window, self.portfolio,self.elts, self.labs)
        
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
        sign = 1
        if entries["Bid or Ask"].get().lower() == "bid":
            odr_str = "BID"
            sign = 1
        else:
            odr_str = "ASK"
            sign = -1
        
        price_per_share = entries["Price per share"].get()
        vol = entries["Volume"].get()
            
        
        send_str = f"{ticker}:{odr_str}:{price_per_share}:{vol}\r\n"

        #If you do a bid, you must have that security
        if odr_str == "BID":
            #You don't have that security at all
            if ticker not in self.securities.keys():
                print("You don't own this security - not Allowed") 
                return
            
            if float(vol) > float(self.securities[ticker]) + self.eps:
                print("You bid more shares than you have - not Allowed")
                print(self.securities[ticker])
                return
        else:       
            if self.portfolio["Capital"] + sign*float(price_per_share) * float(vol) < 0:
                print("You don't have enough capital - not Allowed")
                return
        self.client.write(send_str.encode("ascii"))
        
        #Sent order, deduct this from portfolio
        self.securities["Capital"] = self.securities["Capital"] + float(price_per_share) * float(vol)*sign
        if ticker not in self.securities.keys():
            self.securities[ticker] = 0
        self.securities[ticker] = self.securities[ticker] - sign * float(vol)
        
        display_portfolio(self.window, self.securities,self.elts, self.labs)
        
if __name__ == "__main__":
    c = ClientWindow()
        