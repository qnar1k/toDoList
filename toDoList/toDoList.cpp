#include "Task.h"
#include "FileManager.h"
#include "Menu.h"

#include <iostream>
#include <vector>

using namespace std;

const string FILENAME = "tasks.txt";

int main() {
    vector<Task> tasks;
    FileManager::loadTasks(tasks, FILENAME);

    int choice;
    do {
        Menu::displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: Menu::addTask(tasks); break;
            case 2: Menu::markTaskComplete(tasks); break;
            case 3: Menu::viewTasks(tasks); break;
            case 4: cout << "Saving tasks and exiting...\n"; break;
            default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 4);

    FileManager::saveTasks(tasks, FILENAME);
    return 0;
}
