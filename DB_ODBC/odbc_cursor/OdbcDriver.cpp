#include "OdbcDriver.h"

/* make this class singleton */
//OdbcDriver* OdbcDriver::instance = nullptr;

int OdbcDriver::init()
{
    SQLHENV henv = SQL_NULL_HENV;    // Environment handle
    SQLHDBC hdbc = SQL_NULL_HDBC;    // Connection handle
    SQLHSTMT hstmt = SQL_NULL_HSTMT; // Statement handle
    return 0;
}

OdbcDriver::OdbcDriver(const std::string& idsn)
{
    init();
    SQLRETURN retcode;

    // Allocate environment
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (!check_error(retcode,
                     "SQLAllocHandle(SQL_HANDLE_ENV)",
                     henv, SQL_HANDLE_ENV))
        exit(-1);

    // Set ODBC Verion 3
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
                            (SQLPOINTER *)SQL_OV_ODBC3, 0);
    if (!check_error(retcode,
                     "SQLAllocHandle(SQL_ATTR_ODBC_VERSION)",
                     henv, SQL_HANDLE_ENV))
        exit(-1);

    // Allocate Connection
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (!check_error(retcode,
                     "SQLAllocHandle(SQL_HANDLE_DBC)",
                     hdbc, SQL_HANDLE_DBC))
        exit(-1);

    // Set Login Timeout
    retcode = SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)180, 0);
    if (!check_error(retcode,
                     "SQLSetConnectAttr(SQL_LOGIN_TIMEOUT)",
                     hdbc, SQL_HANDLE_DBC))
        exit(-1);

    retcode = SQLSetConnectAttr(hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)3, 0);
    if (!check_error(retcode,
                     "SQLSetConnectAttr(SQL_ATTR_CONNECTION_TIMEOUT)",
                     hdbc, SQL_HANDLE_DBC))
        exit(-1);

    // Set Auto Commit
    retcode = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT,
                                (SQLPOINTER)FALSE, 0);
    if (!check_error(retcode,
                     "SQLSetConnectAttr(SQL_ATTR_AUTOCOMMIT)",
                     hdbc, SQL_HANDLE_DBC))
        exit(-1);

    // Connect to DSN
    dsn = "DSN="+ idsn + ";";
    retcode = SQLDriverConnect(hdbc, NULL, (SQLCHAR *)dsn.c_str(),
                               SQL_NTS, (SQLCHAR *)NULL, 0, NULL, SQL_DRIVER_COMPLETE);
    if (!check_error(retcode,
                     "SQLDriverConnect()",
                     hdbc, SQL_HANDLE_DBC))
        exit(-1);

        retcode = SQLSetConnectAttr(hdbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TXN_READ_COMMITTED, 
                                     SQL_IS_INTEGER);
        if (!check_error(retcode,
                         "SQLSetConnectAttr(SQL_ATTR_TXN_ISOLATION)",
                         hdbc, SQL_HANDLE_DBC))
            exit(-1);


    /* Transaction isolation level */
    SQLSMALLINT ind = 0;
    SQLUINTEGER param  = 0;

    SQLGetInfo( hdbc, (SQLUSMALLINT)SQL_DEFAULT_TXN_ISOLATION, (SQLPOINTER)&param,
                (SQLSMALLINT)sizeof(param), &ind );

    std::cout<<" - SQL_DEFAULT_TXN_ISOLATION="<<param<<std::endl;

    SQLGetInfo(hdbc, (SQLUSMALLINT) SQL_TXN_ISOLATION_OPTION, (SQLPOINTER)&param,
               (SQLSMALLINT)sizeof(param), &ind);

    std::cout<<" - SQL_TXN_ISOLATION_OPTION="<<param<<std::endl;

    SQLGetInfo(hdbc, (SQLUSMALLINT)SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)&param,
               (SQLSMALLINT)sizeof(param), &ind);

    std::cout << " Before - SQL_ATTR_TXN_ISOLATION=" << param << std::endl;

    std::cout << "    Info: OdbcDriver() constructor, connection established to DB!" << std::endl;
}

OdbcDriver::~OdbcDriver()
{
    freeStmtHandle();

    // Connection
    if (hdbc != SQL_NULL_HDBC)
    {
        SQLDisconnect(hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        hdbc = SQL_NULL_HDBC;
    }

    // Environment
    if (henv != SQL_NULL_HENV)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        henv = SQL_NULL_HENV;
    }

    std::cout << "    Info: ~OdbcDriver() finished!" << std::endl;
}

int OdbcDriver::check_error(SQLRETURN retcode,
                            std::string fn,
                            SQLHANDLE handle,
                            SQLSMALLINT type)
{
    SQLINTEGER i = 0;
    SQLINTEGER NativeError;
    SQLCHAR SQLState[7];
    SQLCHAR MessageText[256];
    SQLSMALLINT TextLength;
    SQLRETURN ret;

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
    {
        std::cout << "   Error: The OdbcDriver reported the following error in " << fn << "\n";
        do
        {
            //std::cout << "\nCalling SQLGetDiagRec" << std::endl;
            ret = SQLGetDiagRec(type, handle, ++i, SQLState, &NativeError,
                                MessageText, sizeof(MessageText), &TextLength);
            //std::cout << "ret: " << ret << std::endl;
            if (SQL_SUCCEEDED(ret))
            {
                std::cout << SQLState << ":" << i << ":" << NativeError << ":" << MessageText << std::endl;
            }
            else if (ret == SQL_ERROR)
                std::cout << "\n     Error: SQLGetDiagRec returns SQL_ERROR" << std::endl;
            else if (ret == SQL_INVALID_HANDLE)
                std::cout << "\n     Error: SQLGetDiagRec returns SQL_INVALID_HANDLE" << std::endl;
            /*else if (ret == SQL_NO_DATA)
                std::cout << "\nSQLGetDiagRec returns SQL_NO_DATA" << std::endl;*/
        } while (ret == SQL_SUCCESS);

        return FAIL;
    }

    return SUCCESS;
}

int OdbcDriver::allocateStmtHandle()
{
    // allocate Statement handles
    SQLRETURN retcode;
    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if (!check_error(retcode, "SQLAllocHandle(SQL_HANDLE_STMT)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    return SUCCESS;
}

int OdbcDriver::freeStmtHandle()
{
    // Free Statement handles
    if (hstmt != SQL_NULL_HSTMT)
    {
        SQLFreeStmt(hstmt, SQL_CLOSE);
        SQLFreeStmt(hstmt, SQL_UNBIND);
        SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        hstmt=SQL_NULL_HSTMT;
    }

    return SUCCESS;
}

int OdbcDriver::executeSql(const std::string& sql, bool print)
{
    SQLRETURN retcode;

     /* Transaction isolation level */
    if (print)
    {
        SQLSMALLINT ind = 0;
        SQLUINTEGER param = 0;

        commit();

        //SQL_TXN_SERIALIZABLE not working
        //SQL_TXN_REPEATABLE_READ not working
        //SQL_TXN_READ_COMMITTED not working
        retcode = SQLSetConnectAttr(hdbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)0x00000004, 
                                     SQL_IS_INTEGER);
        if (!check_error(retcode,
                         "SQLSetConnectAttr(SQL_ATTR_TXN_ISOLATION)",
                         hdbc, SQL_HANDLE_DBC))
            exit(-1);

        SQLGetInfo(hdbc, (SQLUSMALLINT)SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)&param,
                   (SQLSMALLINT)sizeof(param), &ind);

        std::cout << " NOW - SQL_ATTR_TXN_ISOLATION=" << param << std::endl;
    }

    // Allocate Statement Handle
    if (!allocateStmtHandle())
    {
        std::cout << "\n   Error: ODBC could not allocate statement handle in executeSql!" << std::endl;
        return FAIL;
    }

    retcode =  SQLExecDirect(hstmt, (SQLCHAR *)sql.c_str(), SQL_NTS);
    std::cout<<"  ...executed "<<sql<<"successfully"<<std::endl;
    if (!check_error(retcode, "SQLExecDirect(sql)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;


    freeStmtHandle();

    return SUCCESS;
}

int OdbcDriver::executeSelectCursor(const std::string& sql_stmt,
                                     result_t& resultArray,
                                     int batchSize)
{
    SQLRETURN retcode;
    SQLLEN output_len = 0;
    int buf_len=50;
    int hash_len=100;
    SQLLEN o_len;
    //int max_id;

    //Allocate Statement Handle
    if (!allocateStmtHandle())
    {
        std::cout << "\n    Error: ODBC could not allocate statement handle!" << std::endl;
        return FAIL;
    }

    /*retcode= SQLPrepare( hstmt, (SQLCHAR *)sql_stmt.c_str(), SQL_NTS); 
    if (!check_error(retcode, "SQLPrepare(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    retcode = SQLExecute( hstmt );*/

    // Set the following statement attributes for making cursor:
    // SQL_ATTR_CURSOR_TYPE:            Keyset-driven
    // SQL_ATTR_ROW_BIND_TYPE:          SQL_BIND_BY_COLUMN
    // SQL_ATTR_ROW_ARRAY_SIZE:         100 //fails for 650+ in postgres, PGAPI_ExtendedFetch
    // SQL_ATTR_ROW_STATUS_PTR:         Points to RowStatusArray
    // SQL_ATTR_CONCURRENCY:            Sets Concurrency
    //                                  (because default is READ ONLY)
    // SQL_ATTR_ROWS_FETCHED_PTR:       to get actual numbers of rows fetched
    SQLUSMALLINT RowStatusArray[MAX_DATA];
    retcode = SQLSetStmtAttr(hstmt, SQL_ATTR_CURSOR_TYPE,
                             (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, 0);
    retcode = SQLSetStmtAttr(hstmt, SQL_ATTR_ROW_BIND_TYPE,
                             SQL_BIND_BY_COLUMN, 0);
    retcode = SQLSetStmtAttr(hstmt, SQL_ATTR_ROW_ARRAY_SIZE,
                             (SQLPOINTER)batchSize, 0);  //fails beyond 650
    retcode = SQLSetStmtAttr(hstmt, SQL_ATTR_ROW_STATUS_PTR,
                             RowStatusArray, 0);
    retcode = SQLSetStmtAttr(hstmt, SQL_ATTR_CONCURRENCY,
                             (SQLPOINTER)SQL_CONCUR_LOCK ,0); // not in MS example
    retcode = SQLSetStmtAttr(hstmt, SQL_ATTR_ROWS_FETCHED_PTR,
                               (SQLPOINTER)&numOfRows, 0);


    //prepare ODBC statement //SQLExecute(hstmt); --doesn't work
    retcode =  SQLExecDirect(hstmt, (SQLCHAR *)sql_stmt.c_str(), SQL_NTS);
    if (!check_error(retcode, "SQLExecDirect(sql_stmt)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;


    /*  // Bind arrays to */
    SQLBindCol(hstmt, 1, SQL_C_CHAR,
               resultArray.hash,            /* output value */
               (SQLLEN) hash_len, /* buffer len */
               resultArray.hashLen             /* output length */);

    SQLBindCol(hstmt, 2, SQL_C_SLONG,
               &resultArray.id, /* output value */
                0, /* buffer len */
                resultArray.idLen  /* output length */);

    SQLBindCol(hstmt, 3, SQL_C_CHAR,
               resultArray.name,            /* output value */
               (SQLLEN) buf_len, /* buffer len */
               resultArray.nameLen             /* output length */);

    SQLBindCol(hstmt, 4, SQL_C_CHAR,
               resultArray.city,            /* output value */
               (SQLLEN) buf_len, /* buffer len */
               resultArray.cityLen            /* output length */);


    //freeStmtHandle();

    std::cout<<"\n statement formed :("<< __FUNCTION__<<", L"<< __LINE__<<")"<<std::endl;
    return SUCCESS;
}

int 
OdbcDriver::executeFetchArray( result_t& resultArray, int batchSize, int &rowsFetched)
{
    SQLRETURN retcode;

    /* SQL_FETCH_NEXT
       SQL_FETCH_ABSOLUTE
       SQL_FETCH_LAST
       SQL_FETCH_FIRST
       SQL_FETCH_RELATIVE
    */
    retcode = SQLFetchScroll(hstmt, SQL_FETCH_NEXT, batchSize);
    if (retcode == SQL_NO_DATA){
        std::cout<<"  --SQL_NO_DATA\n";
        rowsFetched = (int) numOfRows;
        std::cout<<" retcode =  "<<retcode<<
                "\t rowsFetched: "<<rowsFetched<<std::endl;
        return SQL_NO_DATA;
    }
    else if (!check_error(retcode, "SQLFetchScroll()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    rowsFetched = (int) numOfRows;
    std::cout<<" retcode =  "<<retcode<<
                "\t rowsFetched: "<<rowsFetched<<std::endl;
    return SUCCESS;
}

/* make OdbcDriver singleton class */
OdbcDriver* OdbcDriver::connect(const std::string& idsn){
    // if(!instance)
    //     instance = new OdbcDriver(idsn); //discarding for now

    OdbcDriver* instance = new OdbcDriver(idsn);

    return instance;
}

void OdbcDriver::commit()
{
    std::cout<<"commiting transaction"<<std::endl;
    SQLRETURN retcode;
    retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

    if (!check_error(retcode, "SQLEndTran(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
                     std::cout<<"   Error: Failed to commit trx"<<std::endl;
}

// int OdbcDriver::disconnect(){
//     if(instance)
//         delete instance;

//     instance = nullptr;
//     return 0;
// }