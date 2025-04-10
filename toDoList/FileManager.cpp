#include "FileManager.h"
#include <fstream>
#include <iostream>

void FileManager::saveTasks(const vector<Task>& tasks, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }
    for (const auto& task : tasks) {
        file << task.id << "|" << task.title << "|" << task.isCompleted << "\n";
    }
    file.close();
}

void FileManager::loadTasks(vector<Task>& tasks, const string& filename) {
    ifstream file(filename);
    if (!file) return; // No file exists yet

    int id;
    string title;
    bool isCompleted;
    string line;

    while (getline(file, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');

        id = stoi(line.substr(0, pos1));
        title = line.substr(pos1 + 1, pos2 - pos1 - 1);
        isCompleted = stoi(line.substr(pos2 + 1));

        tasks.emplace_back(id, title, isCompleted);
    }
    file.close();
}
