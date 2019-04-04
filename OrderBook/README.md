# Simulate online stock transaction by implement a stock order book.

An order book contains queues. One is to buy and the other is to sell. Since the market is very dynamic, your data structure(s) to hold the orders vary from time to time. One way to implement the order book is using priority queue. A priority queue is a list which order is based on a priority. How to implement a priority queue is totally a design decision. For example, one may use vector, or pointer to implement this priority queue. You may just use a FIFO queue without priority to implement your order book, although this may not be a good idea (why?)

In our application, an order on top of the buying queue is the one with the highest bid, so the priority used on a buying queue is the bidding price of an order. While on the sell queue, the order on top would be the one with lowest ask price.

If two orders on same queue have the same price then we will use the timestamp to resolve. I.e., the order came earlier would be on top of the one who came in later.

Your system will execute the transaction as soon as the buy-sell matches. After execution of an order, one may remove the order from the queue. You may need to update queues and check if you can execute another transaction.

Write a C++ program that will output menu (and submenu if you so design) to ask user to enter order(s).

Your program will store the order information on the data structure that you designed and carry out transaction if there is match on price.

Your program allows user to query the system. For example, user can ask what is the highest ask price, lowest price; how many shares of top order, how many shares for top three orders and so on.

Your system can allow user to cancel an order from the order book. Also your system can allow user to modify an existing order. Note when an order changes, the position of that order on its queue may change.

Your program allows user to list all orders in a tabular format. Several information are associate with each order:

- Time,

- Name of company (you can use stock ticker or you can invent your own symbols)

- How many shares ask or bid

- Asking price or bid price.

Design your system by choosing your own data structure(s). Come up with the classes design.

Write your class(es). Test your class(es). Write your system. Test your system.

Catch all the screen shots. Write a document to explain the relationship among various components in your system and how they interact with one another.
 
Also write a user document which will explain how a user can use your system.
