#include "Task.h"
#include <iostream>

Task::Task() : id(0), title(""), isCompleted(false) {}

Task::Task(int id, const string& title, bool isCompleted)
    : id(id), title(title), isCompleted(isCompleted) {}

void Task::display() const {
    cout << "[" << (isCompleted ? "1" : "0") << "] " << id << ": " << title << endl;
}
