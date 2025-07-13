#include "DatabaseManager.h"
#include <iostream>
#include <sqltypes.h>
#include <vector>
#include <codecvt>
#include <locale>

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect(const std::wstring& connStr) {
    if (connected) return true;

    SQLRETURN ret;
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (!SQL_SUCCEEDED(ret)) return false;

    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    if (!SQL_SUCCEEDED(ret)) return false;

    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    if (!SQL_SUCCEEDED(ret)) return false;

    SQLWCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;

    ret = SQLDriverConnectW(hDbc, NULL,
        (SQLWCHAR*)connStr.c_str(), SQL_NTS,
        outConnStr, 1024, &outConnStrLen, SQL_DRIVER_COMPLETE);

    if (SQL_SUCCEEDED(ret)) {
        connected = true;
        std::wcout << L"Connected to SQL Server successfully.\n";
        return true;
    }
    else {
        showSQLError(hDbc, SQL_HANDLE_DBC);
        return false;
    }
}

void DatabaseManager::disconnect() {
    if (connected) {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        connected = false;
    }
}

SQLHDBC DatabaseManager::getConnectionHandle() {
    return hDbc;
}

void DatabaseManager::showSQLError(SQLHANDLE handle, SQLSMALLINT handleType) {
    SQLWCHAR sqlState[1024], message[1024];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;
    if (SQLGetDiagRecW(handleType, handle, 1, sqlState, &nativeError, message, 1024, &textLength) == SQL_SUCCESS) {
        std::wcerr << L"ODBC Error: " << message << L" (SQLSTATE: " << sqlState << L")\n";
    }
}

bool DatabaseManager::addTask(const Task& task) {
    SQLHSTMT stmt;
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    std::wstring wtitle(task.title.begin(), task.title.end());

    ret = SQLPrepareW(stmt, (SQLWCHAR*)L"INSERT INTO tasks (title, isCompleted) VALUES (?, ?)", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)wtitle.c_str(), 0, NULL);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, (SQLPOINTER)&task.isCompleted, 0, NULL);

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        showSQLError(stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return true;
}

bool DatabaseManager::markTaskComplete(int taskId) {
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    SQLPrepareW(stmt, (SQLWCHAR*)L"UPDATE tasks SET isCompleted = 1 WHERE id = ?", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &taskId, 0, NULL);

    SQLRETURN ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        showSQLError(stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return true;
}

bool DatabaseManager::loadTasks(std::vector<Task>& tasks) {
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    SQLRETURN ret = SQLExecDirectW(stmt, (SQLWCHAR*)L"SELECT id, title, isCompleted FROM tasks", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        showSQLError(stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return false;
    }

    int id;
    wchar_t titleW[256];
    bool isCompleted;

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        SQLGetData(stmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(stmt, 2, SQL_C_WCHAR, titleW, sizeof(titleW), NULL);
        SQLGetData(stmt, 3, SQL_C_BIT, &isCompleted, 0, NULL);

        std::wstring wtitle(titleW);
        std::string title(wtitle.begin(), wtitle.end());

        tasks.emplace_back(id, title, isCompleted);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return true;
}

bool DatabaseManager::deleteTask(int taskId) {
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    SQLPrepareW(stmt, (SQLWCHAR*)L"DELETE FROM tasks WHERE id = ?", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &taskId, 0, NULL);

    SQLRETURN ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        showSQLError(stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return true;
}

bool DatabaseManager::deleteUser(const std::string& username) {
    std::cout << "deleteUser not implemented. Ignored.\n";
    return false;
}
