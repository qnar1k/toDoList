#pragma once
#ifndef MENU_H
#define MENU_H

#include "Task.h"
#include <vector>

class Menu {
public:
    static void displayMenu();
    static void addTask(vector<Task>& tasks);
    static void markTaskComplete(vector<Task>& tasks);
    static void viewTasks(const vector<Task>& tasks);
};

#endif
