#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <cstring>

#define SUCCESS 1
#define FAIL 0

#define TRUE 1
#define FALSE 0

class OdbcDriver{

    SQLHENV  henv;    // Environment handle
    SQLHDBC  hdbc;     // Connection handle
    SQLHSTMT hstmt;    // Statement handle
    std::string dsn;

    int check_error(SQLRETURN retcode, 
               std::string fn, 
               SQLHANDLE handle, 
               SQLSMALLINT type);
    
    int init();

    int freeStmtHandle();

    int allocateStmtHandle();

public:

   OdbcDriver(const std::string& idsn);

   int executeInsert(int& id);

   int executeSelect(std::string& sql_stmt, std::string& o_result);

   ~OdbcDriver();
};