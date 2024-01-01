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
#define HASH_LEN 100

typedef struct result_s
{
    int id[MAX_DATA];
    char name[MAX_DATA][CHAR_LEN];
    char city[MAX_DATA][CHAR_LEN];
    char hash[MAX_DATA][HASH_LEN];

    long idLen[MAX_DATA];
    long* nameLen; //= new long[MAX_DATA];
    //SQLLEN nameLen[2][MAX_DATA];
    SQLLEN cityLen[MAX_DATA];
    SQLLEN hashLen[MAX_DATA];

    /*long idLen;
    long nameLen;
    SQLLEN cityLen;
    SQLLEN hashLen;*/
}result_t;

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

   int executeSelectCursor(const std::string& sql_stmt, result_t& resultArray, int batchSize);

   int executeFetchArray( result_t& resultArray, int batchSize, int &rowsFetched);

   int disconnect();

   int executeSql(const std::string &sql, bool print=false);

   void commit();

   ~OdbcDriver();
};