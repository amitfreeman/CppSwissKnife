/* This program demonstrate simple ODBC connection 
 *  - to snowflake DB
 *  - perform batch insert, update and delete operation on demo table
 *  - perform update and delete operation using merge table
 *
 *  Pre-requisite: 
 *   - unixODBC & snowflake odbc driver installation
 *   - /etc/odbc.ini file with valid working DSN
 *  
 */

#include "OdbcDriver.h"
#include <unistd.h>
#include <chrono>


int main () {
    int rc;
    int batchSize;
    int mergeSize;
    std::string sql;
    double elapsed_time_ms;

    //create ODBC driver and connect using DSN name
    OdbcDriver* db=OdbcDriver::connect("sp_snowflake_amit");

    /* Create table */
    sql = "create table if not exists BATCH_DEMO (ID int primary key, NAME varchar(50));";
    rc = db->executeSql(sql);

    if(!rc)
        exit(-1);

    std::cout<<"\nPlease enter number of rows to be operated on: ";
    std::cin>>mergeSize;
    std::cout<<"\n";
    if(mergeSize>500000)
        mergeSize=500000;
    else if(mergeSize<0)
        mergeSize=50;

    //capping batch update and delete on 50, because time taken is huge
    batchSize= (mergeSize<=50) ? mergeSize:50; 

    /* ----- insert records in batch mode ----- */
    auto t_start = std::chrono::high_resolution_clock::now();
    rc = db->executeInsertBatch(batchSize, std::string(TABLE_NAME));
    auto t_end = std::chrono::high_resolution_clock::now();

    if(!rc)
        exit(-1);

    elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout<<"*Time taken for "<<batchSize<<" inserts = "<<elapsed_time_ms/1000<<" sec"<<std::endl;

    /* ----- Update records in batch mode ----- */
    t_start = std::chrono::high_resolution_clock::now();
    rc = db->executeUpdateBatch(batchSize);
    t_end = std::chrono::high_resolution_clock::now();

    if(!rc)
        exit(-1);
   
    elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout<<"*Time taken for "<<batchSize<<" batch updates = "<<elapsed_time_ms/1000<<" sec"<<std::endl;

    /* ----- Delete records in batch mode ----- */
    t_start = std::chrono::high_resolution_clock::now();
    rc = db->executeDeleteBatch(batchSize);
    t_end = std::chrono::high_resolution_clock::now();

    if(!rc)
        exit(-1);
   
    elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout<<"*Time taken for "<<batchSize<<" batch deletes = "<<elapsed_time_ms/1000<<" sec"<<std::endl;

    /* -------- start again ------------ */
    sql = "truncate table BATCH_DEMO;";
    rc = db->executeSql(sql);

    if(!rc)
        exit(-1);

    /* ----- insert records in batch mode ----- */
    t_start = std::chrono::high_resolution_clock::now();
    rc = db->executeInsertBatch(mergeSize, std::string(TABLE_NAME));
    t_end = std::chrono::high_resolution_clock::now();

    if(!rc)
        exit(-1);

    elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout<<"*Time taken for "<<mergeSize<<" inserts = "<<elapsed_time_ms/1000<<" sec"<<std::endl;

    /* ----- Update records using merge ----- */
    t_start = std::chrono::high_resolution_clock::now();
    rc = db->executeUpdateMerge(mergeSize);
    t_end = std::chrono::high_resolution_clock::now();

    if(!rc)
        exit(-1);
   
    elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout<<"*Time taken for "<<mergeSize<<" update using merge = "<<elapsed_time_ms/1000<<" sec"<<std::endl;

    /* ----- delete records using merge ----- */
    t_start = std::chrono::high_resolution_clock::now();
    rc = db->executeDeleteMerge(mergeSize);
    t_end = std::chrono::high_resolution_clock::now();

    if(!rc)
        exit(-1);
   
    elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cout<<"*Time taken for "<<mergeSize<<" delete using merge = "<<elapsed_time_ms/1000<<" sec"<<std::endl;

 
    delete db;
    std::cout<<"\n--- Finished successfully ---"<<std::endl;
    return 0;
}
