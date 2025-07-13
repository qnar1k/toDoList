#include "Task.h"
#include "Menu.h"
#include "DatabaseManager.h"

#include <iostream>
#include <vector>

using namespace std;

int main() {
    DatabaseManager db;

    // Connect to SQL Server using Windows Authentication
    if (!db.connect(L"Driver={ODBC Driver 17 for SQL Server};Server=DESKTOP-I04H8Q6\\SQLEXPRESS;Database=ToDoDB;Trusted_Connection=Yes;")) {
        cerr << "Failed to connect to the database.\n";
        return 1;
    }

    vector<Task> tasks;
    if (!db.loadTasks(tasks)) {
        cerr << "Failed to load tasks from the database.\n";
        return 1;
    }

    int choice;
    do {
        Menu::displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            string title;
            cout << "Enter task description: ";
            cin.ignore();
            getline(cin, title);
            Task newTask(0, title, false);
            if (db.addTask(newTask)) {
                cout << "Task added to database.\n";
                tasks.clear(); // Reload from DB to get new ID
                db.loadTasks(tasks);
            }
            break;
        }
        case 2: {
            int id;
            cout << "Enter task ID to mark as complete: ";
            cin >> id;
            if (db.markTaskComplete(id)) {
                cout << "Task marked as completed in DB.\n";
                tasks.clear(); // Refresh view
                db.loadTasks(tasks);
            }
            else {
                cout << "Task not found.\n";
            }
            break;
        }
        case 3:
            Menu::viewTasks(tasks);
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != 4);

    db.disconnect();
    return 0;
}
