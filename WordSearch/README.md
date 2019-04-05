# Word Search

It is a system, written in C++, which will allow user to search data warehouse to retrieve documents based on user’s input. More specifically, you will write a program that generates an "inverted index" of all the words in a list of text files.

Before your system display a menu to allow user to select menu item from a menu, your system needs to create inverted index of these text files so to facilitate and speed up the search work. This menu contains at least two menu items. The first item will ask user to enter a word. After user entered this word, your system will search the inverted index data structure to find out which document(s) contain this word. If there is none, simply output “There is no XXX in our system”, where XXX is the word entered by the user. If one or more documents contain this word, your system will output all the corresponding document name and frequency count. For example, if user entered “finance” and in your system, Doc1.txt and Doc5.txt contain this word “finance” then your system will output the following assuming Doc1.txt ‘finance’ appeared 120 times and Doc5.txt 14times and other documents 0 times.

Doc1.txt , 120

Doc5.txt , 14

To simplify your system implementation you may treat lower case word and uppercase word the same. I.e., “Finance” and “finance”, and “FinanCe” can be treated as the same word.

Also note, the output from the search are sorted based on frequency count. I.e., we rank the document based on how many times the search word appeared in these documents. More frequency count means higher rank in this project.

The last menu item is “Quit”, if user choose this option, then your system output a farewell message and quit. Otherwise after serve one user’s query, your system will continue to display the menu and wait for user to input an option from the menu.

NOTE, because file reading and processing are time consuming tasks, you must preprocess the files once and cannot search them again after you display the menu. Doing so will slow down your search task.
 
