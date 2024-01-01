#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <cstring>

#define SUCCESS 1
#define FAIL 0

#define TRUE 1
#define FALSE 0

#define MAX_DATA 1000
#define CHAR_LEN 50

#define TABLE_NAME "BATCH_DEMO"



class OdbcDriver{

    SQLHENV  henv;    // Environment handle
    SQLHDBC  hdbc;     // Connection handle
    SQLHSTMT hstmt;    // Statement handle
    std::string dsn;
    //static OdbcDriver* instance;
    SQLULEN numOfRows=0;

    int check_error(SQLRETURN retcode, 
               std::string fn, 
               SQLHANDLE handle, 
               SQLSMALLINT type);
    
    int init();
  
    OdbcDriver(const std::string& idsn);

    OdbcDriver(const OdbcDriver& obj) = delete;
   
    OdbcDriver operator=(const OdbcDriver& obj) = delete;

    int freeStmtHandle();

    int allocateStmtHandle();

public:

   static OdbcDriver* connect(const std::string& idsn);

   int disconnect();

   int executeSql(const std::string &sql);

   void commit();

   int executeInsertBatch(int& array_size, std::string table_name, int merge_insert=0); 
                                              //0 - default, 1 - for merge update, 2 - for merge delete

   int executeUpdateBatch(int& array_size);

   int executeDeleteBatch(int& array_size);

   int executeUpdateMerge(int& array_size);

   int executeDeleteMerge(int& array_size);

   ~OdbcDriver();
};