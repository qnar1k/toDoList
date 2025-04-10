#pragma once
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Task.h"
#include <vector>
#include <string>

class FileManager {
public:
    static void saveTasks(const vector<Task>& tasks, const string& filename);
    static void loadTasks(vector<Task>& tasks, const string& filename);
};

#endif
