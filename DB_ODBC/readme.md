This program demonstrate simple ODBC connection 
   - to mysql DB, - perform Select & update operation on demo table
   - bind variables & result
 
   Pre-requisite: 
    - unixODBC & mysql odbc (myodbc8w) driver installation
    - /etc/odbc.ini file with valid working DSN
    - create demo table in DB (create table demo (id int(5), name varchar(50), primary key (id));)
   
   Program:
    - Instantiate ODBC driver and connect to DB
    - Fetch maximum primary key - ID present in table demo
    - Insert new row with max ID + 1, and text+number as name 
 
