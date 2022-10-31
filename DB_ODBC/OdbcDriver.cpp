#include "OdbcDriver.h"

/* make this class singleton */
OdbcDriver* OdbcDriver::instance = nullptr;

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
                     "SQLAllocHandle(SQL_LOGIN_TIMEOUT)",
                     hdbc, SQL_HANDLE_DBC))
        exit(-1);

    retcode = SQLSetConnectAttr(hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)3, 0);
    if (!check_error(retcode,
                     "SQLAllocHandle(SQL_ATTR_CONNECTION_TIMEOUT)",
                     hdbc, SQL_HANDLE_DBC))
        exit(-1);

    // Set Auto Commit
    retcode = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT,
                                (SQLPOINTER)FALSE, 0);
    if (!check_error(retcode,
                     "SQLAllocHandle(SQL_ATTR_AUTOCOMMIT)",
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

int OdbcDriver::executeInsert(int& id)
{

    SQLRETURN retcode;

    // Allocate Statement Handle
    if(!allocateStmtHandle()){
        std::cout<<"\n    Error: ODBC could not allocate statement handle!"<<std::endl;
        return FAIL;
    }

    /* Prepare C vars for input */
    int buf_len=50;
    std::string name_str = "Text_" + std::to_string(id);
    char name[buf_len];
    strcpy(name, name_str.c_str());
    SQLLEN cb_name = strlen(name);
    int i;

    /* bind params  */
    SQLUSMALLINT bindIdx = 1;

    retcode = SQLBindParameter(hstmt, bindIdx++, SQL_PARAM_INPUT,
                               SQL_C_SLONG,  /* value type */
                               SQL_INTEGER, /* param type */
                               0,           /* column size */
                               0,           /* dec digits */
                               &id,         /* param value ptr */
                               sizeof(id),  /* buffer len */
                               0 /* StrLen_or_IndPtr */);

    if (!check_error(retcode, "SQLBindParameter()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    retcode = SQLBindParameter(hstmt, bindIdx++, SQL_PARAM_INPUT,
                               SQL_C_CHAR, /* value type */
                               SQL_VARCHAR,   /* param type */
                               buf_len,         /* column size */
                               0,          /* dec digits */
                               name,       /* param value ptr */
                               buf_len,         /* buffer len */
                               &cb_name /* StrLen_or_IndPtr */);

    if (!check_error(retcode,"SQLBindParameter()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Prepare Statement
    retcode = SQLPrepare(hstmt, (SQLCHAR *)"insert into demo values ( ?, ? )", SQL_NTS);

    if (!check_error(retcode, "SQLPrepare(insert)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Execute statement
    retcode = SQLExecute(hstmt);

    if (!check_error(retcode, "SQLExecute()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;
    else
        std::cout << "    Info: Execution status = Success" << std::endl;

    retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

    if (!check_error(retcode, "SQLEndTran(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Free Statement handles
    freeStmtHandle();

    return SUCCESS;
}

int OdbcDriver::executeSelect(const std::string& sql_stmt, std::string& o_result)
{
    SQLRETURN retcode;
    SQLLEN output_len = 0;
    char buffer[50]={'\0'};
    int buf_len=50;
    SQLLEN o_len;
    //int max_id;

    // Allocate Statement Handle
    if (!allocateStmtHandle())
    {
        std::cout << "\n    Error: ODBC could not allocate statement handle!" << std::endl;
        return FAIL;
    }

    retcode = SQLExecDirect(hstmt, (SQLCHAR *)sql_stmt.c_str(), SQL_NTS);
    if (!check_error(retcode, "SQLEndTran(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    /* code to fetch directly int value */
    /*SQLBindCol(hstmt, 1, SQL_C_SLONG,
               &max_id, /* output value */
               //0, /* buffer len */
               // &o_len  /* output length */);

    /* code to fetch string result, generic */
    SQLBindCol(hstmt, 1, SQL_C_CHAR,
               buffer,            /* output value */
               (SQLLEN) buf_len, /* buffer len */
               &o_len             /* output length */);

    SQLFetch(hstmt);

    //std::cout<<"    Info: Result fetched = "<<max_id<<std::endl;
    o_result = std::string(buffer);
    std::cout<<"    Info: Result fetched = "<<o_result<<std::endl;

    // Free Statement handles
    freeStmtHandle();

    return SUCCESS;
}

/* make OdbcDriver singleton class */
OdbcDriver* OdbcDriver::connect(const std::string& idsn){
    if(!instance)
        instance = new OdbcDriver(idsn);

    return instance;
}

int OdbcDriver::disconnect(){
    if(instance)
        delete instance;

    instance = nullptr;
    return 0;
}