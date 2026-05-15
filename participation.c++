#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;

// Task structure
struct Task {
    int id;
    string description;
    string dueDate;
    bool completed;
};

// Global task list
vector<Task> tasks;
int nextTaskId = 1;

// Function prototypes
void displayMenu();
void addTask();
void viewTasks();
void markTaskComplete();
void deleteTask();
void setReminder();
void addNote();
void saveData();
void loadData();
void displayNotes();

int main() {
    loadData();
    
    cout << "=====================================" << endl;
    cout << "    Welcome to Personal Assistant    " << endl;
    cout << "=====================================" << endl;
    
    int choice;
    while (true) {
        displayMenu();
        cout << "Enter your choice (1-9): ";
        cin >> choice;
        
        if (choice == 1) addTask();
        else if (choice == 2) viewTasks();
        else if (choice == 3) markTaskComplete();
        else if (choice == 4) deleteTask();
        else if (choice == 5) setReminder();
        else if (choice == 6) addNote();
        else if (choice == 7) displayNotes();
        else if (choice == 8) saveData();
        else if (choice == 9) {
            saveData();
            cout << "Thank you for using Personal Assistant! Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice! Please try again." << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
}

void displayMenu() {
    cout << "\n========== MAIN MENU ==========" << endl;
    cout << "1. Add Task" << endl;
    cout << "2. View All Tasks" << endl;
    cout << "3. Mark Task Complete" << endl;
    cout << "4. Delete Task" << endl;
    cout << "5. Set Reminder" << endl;
    cout << "6. Add Note" << endl;
    cout << "7. View Notes" << endl;
    cout << "8. Save Data" << endl;
    cout << "9. Exit" << endl;
}

void addTask() {
    Task newTask;
    newTask.id = nextTaskId++;
    cout << "Enter task description: ";
    cin.ignore();
    getline(cin, newTask.description);
    cout << "Enter due date (DD/MM/YYYY): ";
    cin >> newTask.dueDate;
    newTask.completed = false;
    
    tasks.push_back(newTask);
    cout << "Task added successfully! ID: " << newTask.id << endl;
}

void viewTasks() {
    if (tasks.empty()) {
        cout << "No tasks found!" << endl;
        return;
    }
    
    cout << "\n========== YOUR TASKS ==========" << endl;
    for (const auto& task : tasks) {
        cout << "ID: " << task.id << " | ";
        cout << task.description << " | ";
        cout << "Due: " << task.dueDate << " | ";
        cout << (task.completed ? "[COMPLETED]" : "[PENDING]") << endl;
    }
}

void markTaskComplete() {
    viewTasks();
    if (tasks.empty()) return;
    
    int id;
    cout << "Enter task ID to mark complete: ";
    cin >> id;
    
    for (auto& task : tasks) {
        if (task.id == id) {
            task.completed = true;
            cout << "Task marked as complete!" << endl;
            return;
        }
    }
    cout << "Task not found!" << endl;
}

void deleteTask() {
    viewTasks();
    if (tasks.empty()) return;
    
    int id;
    cout << "Enter task ID to delete: ";
    cin >> id;
    
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->id == id) {
            tasks.erase(it);
            cout << "Task deleted successfully!" << endl;
            return;
        }
    }
    cout << "Task not found!" << endl;
}

void setReminder() {
    time_t now = time(0);
    char* dt = ctime(&now);
    cout << "Reminder set! Current date/time: " << dt;
    cout << "Don't forget to check your tasks!" << endl;
}

void addNote() {
    string note;
    cout << "Enter your note: ";
    cin.ignore();
    getline(cin, note);
    
    ofstream noteFile("notes.txt", ios::app);
    if (noteFile.is_open()) {
        noteFile << note << endl;
        noteFile.close();
        cout << "Note saved successfully!" << endl;
    }
}

void displayNotes() {
    ifstream noteFile("notes.txt");
    string line;
    
    cout << "\n========== YOUR NOTES ==========" << endl;
    if (noteFile.is_open()) {
        while (getline(noteFile, line)) {
            cout << "- " << line << endl;
        }
        noteFile.close();
    } else {
        cout << "No notes found!" << endl;
    }
}

void saveData() {
    ofstream file("tasks.txt");
    if (file.is_open()) {
        for (const auto& task : tasks) {
            file << task.id << "|" << task.description << "|" 
                 << task.dueDate << "|" << task.completed << endl;
        }
        file.close();
        cout << "Data saved successfully!" << endl;
    }
}

void loadData() {
    ifstream file("tasks.txt");
    string line;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            Task task;
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);
            
            task.id = stoi(line.substr(0, pos1));
            task.description = line.substr(pos1 + 1, pos2 - pos1 - 1);
            task.dueDate = line.substr(pos2 + 1, pos3 - pos2 - 1);
            task.completed = (line.substr(pos3 + 1) == "1");
            
            tasks.push_back(task);
            if (task.id >= nextTaskId) nextTaskId = task.id + 1;
        }
        file.close();
    }
}