#pragma once
#ifndef TASK_H
#define TASK_H

#include <string>
using namespace std;

class Task {
public:
    int id;          // Unique task ID
    string title;    // Task description
    bool isCompleted; // Task status (true = completed)

    // Constructors
    Task();
    Task(int id, const string& title, bool isCompleted = false);

    // Display the task
    void display() const;
};

#endif
