#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

const string USER_FILE = "users.txt";
string hashPassword(const string& password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + (unsigned char)c; // hash * 33 + c
    }
    return to_string(hash);
}


map<string, string> loadUsers() {
    map<string, string> users;
    ifstream inFile(USER_FILE);

    if (!inFile) {
        return users; 
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string username, hashedPassword;
        getline(ss, username, ',');
        getline(ss, hashedPassword, ',');

        users[username] = hashedPassword;
    }

    inFile.close();
    return users;
}

void saveUser(const string& username, const string& hashedPassword) {
    ofstream outFile(USER_FILE, ios::app);
    outFile << username << "," << hashedPassword << "\n";
    outFile.close();
}

void registerUser() {
    cout << "\n--- Register ---\n";

    string username, password;
    cout << "Choose a username: ";
    cin >> username;
    cout << "Choose a password: ";
    cin >> password;
    if (username.empty() || password.empty()) {
        cout << "Error: username and password cannot be empty.\n";
        return;
    }

    if (password.length() < 4) {
        cout << "Error: password must be at least 4 characters long.\n";
        return;
    }

    map<string, string> users = loadUsers();

    if (users.find(username) != users.end()) {
        cout << "Error: that username is already taken. Try a different one.\n";
        return;
    }

    string hashedPassword = hashPassword(password);
    saveUser(username, hashedPassword);

    cout << "Success! Account created for '" << username << "'. You can now log in.\n";
}

void loginUser() {
    cout << "\n--- Login ---\n";

    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    map<string, string> users = loadUsers();

    if (users.find(username) == users.end()) {
        cout << "Error: no account found with that username.\n";
        return;
    }

    string hashedPassword = hashPassword(password);

    if (users[username] == hashedPassword) {
        cout << "Success! Welcome back, " << username << ".\n";
    } else {
        cout << "Error: incorrect password.\n";
    }
}

int main() {
    int choice;

    while (true) {
        cout << "\n===== Login and Registration System =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Choose an option (1-3): ";
        cin >> choice;

        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            loginUser();
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid option, please choose 1, 2, or 3.\n";
        }
    }

    return 0;
}