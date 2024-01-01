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

/* make OdbcDriver singleton class */
OdbcDriver* OdbcDriver::connect(const std::string& idsn){
    // if(!instance)
    //     instance = new OdbcDriver(idsn); //discarding for now

    OdbcDriver* instance = new OdbcDriver(idsn);

    return instance;
}

void OdbcDriver::commit()
{
    //std::cout<<"commiting transaction"<<std::endl;
    SQLRETURN retcode;
    retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

    if (!check_error(retcode, "SQLEndTran(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
                     std::cout<<"   Error: Failed to commit trx"<<std::endl;
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

int OdbcDriver::executeSql(const std::string& sql)
{
    SQLRETURN retcode;

    // Allocate Statement Handle
    if (!allocateStmtHandle())
    {
        std::cout << "\n   Error: ODBC could not allocate statement handle in executeSql!" << std::endl;
        return FAIL;
    }

    retcode =  SQLExecDirect(hstmt, (SQLCHAR *)sql.c_str(), SQL_NTS);
    if (!check_error(retcode, "SQLExecDirect(sql)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    std::cout<<"  ...executed sql: "<<sql<<" successfully"<<std::endl;

    commit();
    freeStmtHandle();

    return SUCCESS;
}

/* ---- Batch insert update delete */

int OdbcDriver::executeInsertBatch(int& array_size, std::string table_name,int merge_insert){
    SQLRETURN retcode;

    // Allocate Statement Handle
    if(!allocateStmtHandle()){
        std::cout<<"\n    Error: ODBC could not allocate statement handle!"<<std::endl;
        return FAIL;
    }

    SQLULEN cur_row = 0;
    SQLLEN row_cnt = 0;
    char* nrow_ptr = 0;

    nrow_ptr += array_size;
    SQLUSMALLINT* row_status = (SQLUSMALLINT*)malloc(array_size * sizeof(SQLUSMALLINT ));
    SQLSetStmtAttr( hstmt, SQL_ATTR_PARAM_BIND_TYPE, (void *)(SQLINTEGER)SQL_PARAM_BIND_BY_COLUMN, 0);
    SQLSetStmtAttr( hstmt, SQL_ATTR_PARAMSET_SIZE, (void *)nrow_ptr, 0);
    SQLSetStmtAttr( hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, (void *)&cur_row, 0);
    SQLSetStmtAttr( hstmt, SQL_ATTR_PARAM_STATUS_PTR, (void *)row_status, 0 );

    /* Prepare C vars for input */
    int id[array_size];
    int buf_len=CHAR_LEN;
    char name[array_size][buf_len];
    SQLLEN cb_name[array_size];

    for (int i = 0; i < array_size; i++)
    {
        id[i] = i + 1;

        memset(name[i], 0, buf_len);
        std::string name_str;
        if(merge_insert==1)
            name_str = "Updated merge";
        else
            name_str = "Text_" + std::to_string(id[i]);

        strcpy(name[i], name_str.c_str());
        cb_name[i] = strlen(name[i]);
    }

    /* bind params  */
    SQLUSMALLINT bindIdx = 1;

    retcode = SQLBindParameter(hstmt, bindIdx++, SQL_PARAM_INPUT,
                               SQL_C_SLONG,  /* value type */
                               SQL_INTEGER, /* param type */
                               0,           /* column size */
                               0,           /* dec digits */
                               id,         /* param value ptr */
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
                               cb_name /* StrLen_or_IndPtr */);

    if (!check_error(retcode,"SQLBindParameter()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Prepare Statement
    std::string insert_sql = "insert into ";
    insert_sql += table_name;
    insert_sql += " values ( ?, ? )";

    std::cout<<"    sql: "<<insert_sql<<std::endl;
    retcode = SQLPrepare(hstmt, (SQLCHAR *)insert_sql.c_str(), SQL_NTS);

    if (!check_error(retcode, "SQLPrepare(insert)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Execute statement
    retcode = SQLExecute(hstmt);

    if (!check_error(retcode, "SQLExecute()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

    if (!check_error(retcode, "SQLEndTran(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Free Statement handles
    freeStmtHandle();
    free(row_status);

    return SUCCESS;
}

int OdbcDriver::executeUpdateBatch(int& array_size){

    SQLULEN cur_row = 0;
    SQLLEN row_cnt = 0;
    char *nrow_ptr = 0;
    SQLRETURN retcode;

    // Allocate Statement Handle
    if (!allocateStmtHandle())
    {
        std::cout << "\n    Error: ODBC could not allocate statement handle!" << std::endl;
        return FAIL;
    }

    nrow_ptr += array_size;
    SQLUSMALLINT *row_status = (SQLUSMALLINT *)malloc(array_size * sizeof(SQLUSMALLINT));
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_BIND_TYPE, (void *)(SQLINTEGER)SQL_PARAM_BIND_BY_COLUMN, 0);
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMSET_SIZE, (void *)nrow_ptr, 0);
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, (void *)&cur_row, 0);
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_STATUS_PTR, (void *)row_status, 0);

    int id[array_size];
    SQLLEN cb_id[array_size];
    int buf_len=CHAR_LEN;
    char name[array_size][buf_len];
    SQLLEN cb_name[array_size];
    int i;

    for (i = 0; i < array_size; i++)
    {
        id[i] = i + 1;
        cb_id[i] = 0;

        memset(name[i], 0, buf_len);
        strcpy(name[i], "updated batch");
        cb_name[i] = strlen(name[i]);
    }

    SQLUSMALLINT bindIdx = 1;

    /* bind param  */

    retcode = SQLBindParameter(hstmt, bindIdx++, SQL_PARAM_INPUT,
                               SQL_C_CHAR, /* value type */
                               SQL_CHAR,   /* param type */
                               buf_len,         /* column size */
                               0,          /* dec digits */
                               name,       /* param value ptr */
                               buf_len,         /* buffer len */
                               cb_name /* StrLen_or_IndPtr */);
    if (!check_error(retcode, "SQLBindParameter()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    retcode = SQLBindParameter(hstmt, bindIdx++, SQL_PARAM_INPUT,
                               SQL_C_LONG,  /* value type */
                               SQL_INTEGER, /* param type */
                               0,           /* column size */
                               0,           /* dec digits */
                               id,          /* param value ptr */
                               sizeof(id),  /* buffer len */
                               cb_id /* StrLen_or_IndPtr */);
    if (!check_error(retcode, "SQLBindParameter()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Prepare Statement
    std::string update_sql = "update BATCH_DEMO set NAME = ? where ID = ?;";
    std::cout<<"    sql: "<<update_sql<<std::endl;
    retcode = SQLPrepare(hstmt, (SQLCHAR *)update_sql.c_str(), SQL_NTS);
    if (!check_error(retcode, "SQLExecute()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Execute statement
    retcode = SQLExecute(hstmt);

    if (!check_error(retcode, "SQLExecute()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

    if (!check_error(retcode, "SQLEndTran(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Free Statement handles
    freeStmtHandle();
    free(row_status);

    return SUCCESS;
}

int OdbcDriver::executeDeleteBatch(int& array_size){

    SQLULEN cur_row = 0;
    SQLLEN row_cnt = 0;
    char *nrow_ptr = 0;
    SQLRETURN retcode;

    // Allocate Statement Handle
    if (!allocateStmtHandle())
    {
        std::cout << "\n    Error: ODBC could not allocate statement handle!" << std::endl;
        return FAIL;
    }

    nrow_ptr += array_size;
    SQLUSMALLINT *row_status = (SQLUSMALLINT *)malloc(array_size * sizeof(SQLUSMALLINT));
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_BIND_TYPE, (void *)(SQLINTEGER)SQL_PARAM_BIND_BY_COLUMN, 0);
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMSET_SIZE, (void *)nrow_ptr, 0);
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, (void *)&cur_row, 0);
    SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_STATUS_PTR, (void *)row_status, 0);

    int id[array_size];
    SQLLEN cb_id[array_size];
    int i;

    for (i = 0; i < array_size; i++)
    {
        id[i] = i + 1;
        cb_id[i] = 0;
    }

    SQLUSMALLINT bindIdx = 1;

    /* bind param  */
    retcode = SQLBindParameter(hstmt, bindIdx++, SQL_PARAM_INPUT,
                               SQL_C_LONG,  /* value type */
                               SQL_INTEGER, /* param type */
                               0,           /* column size */
                               0,           /* dec digits */
                               id,          /* param value ptr */
                               sizeof(id),  /* buffer len */
                               cb_id /* StrLen_or_IndPtr */);
    if (!check_error(retcode, "SQLBindParameter()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Prepare Statement
    std::string delete_sql = "delete from BATCH_DEMO where ID = ?";
    std::cout<<"    sql: "<<delete_sql<<std::endl;
    retcode = SQLPrepare(hstmt, (SQLCHAR *)delete_sql.c_str(), SQL_NTS);
    if (!check_error(retcode, "SQLExecute()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Execute statement
    retcode = SQLExecute(hstmt);

    if (!check_error(retcode, "SQLExecute()",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

    if (!check_error(retcode, "SQLEndTran(SQL_COMMIT)",
                     hstmt, SQL_HANDLE_STMT))
        return FAIL;

    // Free Statement handles
    freeStmtHandle();
    free(row_status);

    return SUCCESS;
}
/* -------------------  */

// /* ---------- merge update and delete ----------*/
int OdbcDriver::executeUpdateMerge(int& array_size){
    int rc=SUCCESS;
    
    srand(time(0));
    int num=rand()%100;
    std::string staging_u_table = std::string(TABLE_NAME) + std::to_string(num) + "_u";

    std::string sql = "create table if not exists ";
    sql += staging_u_table;
    sql += " (ID int primary key, NAME varchar(50));";
    rc = executeSql(sql);
    if(!rc)
        return FAIL;

    rc = executeInsertBatch(array_size, staging_u_table, 1);

    // Prepare Statement
    std::string merge_update = "merge into BATCH_DEMO t using ";
    merge_update += staging_u_table +" s ";
    merge_update += "on t.id = s.id ";
    merge_update += " when matched then ";
    merge_update += " update set t.name = s.name";

    rc = executeSql(merge_update.c_str());

    if (!rc)
    {
        std::cout<<" update merge failed: "<<merge_update<<std::endl;
        return FAIL;
    }

    sql = "drop table "+staging_u_table;
    executeSql(sql.c_str());

    return SUCCESS;
}

int OdbcDriver::executeDeleteMerge(int& array_size){
    int rc=SUCCESS;
    
    srand(time(0));
    int num=rand()%100;
    std::string staging_d_table = std::string(TABLE_NAME) + std::to_string(num) + "_d";

    std::string sql = "create table if not exists ";
    sql += staging_d_table;
    sql += " (ID int primary key, NAME varchar(50));";
    rc = executeSql(sql);
    if(!rc)
        return FAIL;

    rc = executeInsertBatch(array_size, staging_d_table, 1);

    // Prepare Statement
    std::string merge_update = "merge into BATCH_DEMO t using ";
    merge_update += staging_d_table +" s ";
    merge_update += "on t.id = s.id ";
    merge_update += " when matched then delete;";

    rc = executeSql(merge_update.c_str());

    if (!rc)
    {
        std::cout<<" delete merge failed: "<<merge_update<<std::endl;
        return FAIL;
    }

    sql = "drop table "+staging_d_table;
    executeSql(sql.c_str());

    return SUCCESS;
}

/* --------------------------------  */