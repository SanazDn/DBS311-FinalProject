# DBS311-FinalProject
In this assignment, I created a simple Retail application using the C++ programming language and Oracle (PL/SQL).

DBS311.sql covers all materials learned on Oracle SQL SELECT statements.

DBS.cpp covers PL/SQL stored procedures using the C++ programming language and Oracle and If the user chooses option 1, ask the user to enter customer ID to login. 
To see if the customer with the entered ID exists, call the Oracle stored procedure find_customer(). IF the value of the output parameter in the procedure is 1, 
let the customer to continue. If the value of the output parameter found is 0, call the mainMenu() functions again and asks the customer to login again. 
Continue this process until the user chooses the option 0 to exit or enters a valid customer ID.
