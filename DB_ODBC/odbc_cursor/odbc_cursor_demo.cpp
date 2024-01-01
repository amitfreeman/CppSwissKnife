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

/* modifying for postgres ODBC cursor and fetch */


int main () {
    int rc;
    std::string sql_select;
    result_t result_arr;
    char ans;

    std::cout<<"\n--- Starting, making DB connection --"<<std::endl;
    
    //create ODBC driver and connect using DSN = db-amit-auth
    OdbcDriver* d=OdbcDriver::connect("pgdb_aparopka");  //for read c
   OdbcDriver* dl=OdbcDriver::connect("pgdb_aparopka"); //for lock table

    //1. lock table with dl , run update in other session
    //  rc = dl->executeSql("lock table select_demo in exclusive mode;");
    //  if(!rc)
    //      return -1;
    
    //  std::cout<<"table select_demo locked, press key to continue..."<<std::endl;
    //  std::cin>>ans;


    //2. get read c view with d
   // rc = d->executeSql("set transaction isolation level repeatable read read only;");
    //rc = d->executeSql("delete from  \"SHAREPLEX_COMPARE_MARKER\";");
    rc = d->executeSql("set transaction isolation level repeatable read;", true); // read only;", true);
    if(!rc)
        return -1;

    //rc = d->executeSql("select * From \"DEMO_SRC\";");
    
    std::cout<<"read c on d conn taken, press key to continue..."<<std::endl;
    std::cin>>ans;

    //3. release lock dl
    // dl->commit();
    // std::cout<<"table select_demo unlocked, press key to continue..."<<std::endl;
    // std::cin>>ans;

    //4. view table data with d
    
    //sql_select = "select md5(id::varchar), id, name, city "
    sql_select = "select ctid::varchar, id, name, city "
            "from select_demo order by 2,1";
    std::cout<<"\n--- select SQL = "<<sql_select<<std::endl;

    int batchSize=10; //default
    std::cout<<"Enter number of rows to be fetched?"<<std::endl;
    std::cin>>batchSize;

    result_arr.nameLen = new long[batchSize];

    rc=d->executeSelectCursor(sql_select, result_arr, batchSize);
    std::cout<<"\n--- rc = "<<rc<<std::endl;
    if(!rc){
      std::cout<<"\n--- select failed! ---"<<std::endl;
      return -1;
    }

    /* Fetch result */
    int rowsFetched;
    rc = d->executeFetchArray(result_arr, batchSize, rowsFetched);
    std::cout << "\nctid,\t  id,\t  name,\t  city"<<std::endl;
    while(rc!=SQL_NO_DATA & rc!=FAIL){
      /* Print data */
      for (int i = 0; i < rowsFetched; i++)
      {
        std::cout <<result_arr.hash[i]<<"\t  "<< result_arr.id[i]<<"\t  "<<
                      result_arr.name[i]<<"\t  "<<result_arr.city[i]<<std::endl;
        
      }
      /*fetch next batch*/
      rc = d->executeFetchArray(result_arr, batchSize, rowsFetched);
    }
 
    //d->disconnect();
    delete d;
    delete dl;

    delete[] result_arr.nameLen ;

    std::cout<<"\n--- Finished successfully ---"<<std::endl;
    return 0;
}
