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
    static OdbcDriver* instance;

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

    ~OdbcDriver();

public:

   static OdbcDriver* connect(const std::string& idsn);

   int executeInsert(int& id);

   int executeSelect(const std::string& sql_stmt, std::string& o_result);

   int disconnect();

};