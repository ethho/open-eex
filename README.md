# open-eex
An open-source electronic market server. We have included both the aspects to the server as well as a GUI for the clients.

The server accepts orders from clients in the format $TICKER:$VAR:$PRICE:$VOLUME, where $VAR is either BID or ASK. The server then adds these orders into a queue of orders, and is equipped with a scheduler, that assigns these tasks to threads that are waiting for tasks. The server is equipped with a matching engine as well, and order matching is implemented using 2 heaps for each security. One heap corresponds to all the buy orders, and the other heap corresponds to all the sell orders. The comparitor for the heaps ensures price-time priority. We compare the tops of the two heaps to perform order matching. 

To run the server:
cmake -B build
cd build
make
./main

Clients may connect through any means which involves establishing a socket with the server. The easiest method we found was telnet. Clients may also use the GUI present in the client_api directory, which abstracts away all the details of the server connection. The GUI includes a form to submit orders as well as display of the portfolio of the client. 
To run the client GUI, use 
cd client_api
python client_gui.py
