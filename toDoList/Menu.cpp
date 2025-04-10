#include "Menu.h"
#include <iostream>

void Menu::displayMenu() {
    cout << "\n=== To-Do List Menu ===\n";
    cout << "1. Add Task\n";
    cout << "2. Mark Task Complete\n";
    cout << "3. View Tasks\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

void Menu::addTask(vector<Task>& tasks) {
    string title;
    cout << "Enter task description: ";
    cin.ignore();
    getline(cin, title);

    int id = tasks.empty() ? 1 : tasks.back().id + 1;
    tasks.emplace_back(id, title, false);
    cout << "Task added successfully!\n";
}

void Menu::markTaskComplete(vector<Task>& tasks) {
    int id;
    cout << "Enter task ID to mark as complete: ";
    cin >> id;

    for (auto& task : tasks) {
        if (task.id == id) {
            task.isCompleted = true;
            cout << "Task marked as completed.\n";
            return;
        }
    }
    cout << "Task not found.\n";
}

void Menu::viewTasks(const vector<Task>& tasks) {
    if (tasks.empty()) {
        cout << "No tasks available.\n";
        return;
    }

    cout << "\nYour Tasks:\n";
    for (const auto& task : tasks) {
        task.display();
    }
}
