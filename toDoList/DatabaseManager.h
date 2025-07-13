#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include "Task.h"  // assuming you have a Task class with id, title, isCompleted

class DatabaseManager {
private:
    SQLHENV hEnv = nullptr;
    SQLHDBC hDbc = nullptr;
    bool connected = false;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect(const std::wstring& connStr);
    void disconnect();
    void showSQLError(SQLHANDLE handle, SQLSMALLINT handleType);
    SQLHDBC getConnectionHandle();

    // Task-specific operations
    bool addTask(const Task& task);
    bool markTaskComplete(int taskId);
    bool loadTasks(std::vector<Task>& tasks);
    bool deleteTask(int taskId); // Optional
    bool deleteUser(const std::string& username); // Placeholder (not used in this project)
};
