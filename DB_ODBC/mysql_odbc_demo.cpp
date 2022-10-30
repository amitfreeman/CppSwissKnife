/* This program demonstrate simple ODBC connection 
 *  - to mysql DB, - perform Select & update operation on demo table
 *  - bind variables & result
 *
 *  Pre-requisite: 
 *   - unixODBC & mysql odbc (myodbc8w) driver installation
 *   - /etc/odbc.ini file with valid working DSN
 *   - create demo table in DB (create table demo (id int(5), name varchar(50), primary key (id));)
 *  
 *  Program:
 *   - Instantiate ODBC driver and connect to DB
 *   - Fetch maximum primary key - ID present in table demo
 *   - Insert new row with max ID + 1, and text+number as name 
 */

#include <iostream>
#include "OdbcDriver.h"


int main () {
    int rc;
    std::string sql_select, max_id_str;
    int max_id;

    std::cout<<"\n--- Starting, making DB connection --"<<std::endl;
    
    //create ODBC driver and connect using DSN = db-amit-auth
    OdbcDriver* d=OdbcDriver::connect("db-amit-auth");

    // get max ID present in demo table
    std::cout<<"\n--- Select max id (pk) from demo table --"<<std::endl;
    sql_select = "select max(id) from demo";
    rc=d->executeSelect(sql_select, max_id_str);
    std::cout<<"\n--- rc = "<<rc<<std::endl;
    if(!rc){
      std::cout<<"\n--- select failed! ---"<<std::endl;
      return -1;
    }

    max_id=std::stoi(max_id_str);
 
    // insert new row with max id +1
    std::cout<<"\n--- Inserting row into demo table with new id = "<<++max_id<<" --"<<std::endl;   
    rc=d->executeInsert(max_id);
    std::cout<<"\n--- rc = "<<rc<<std::endl;
    if(!rc){
      std::cout<<"\n--- insert failed! ---"<<std::endl;
      return -1;
    }

    d->disconnect();

    std::cout<<"\n--- Finished successfully ---"<<std::endl;
    return 0;
}
