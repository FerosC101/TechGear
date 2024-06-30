#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Item {
    string name;
    double price;
    int quantity;
};
struct User {
    string username;
    string password;
    vector<string>cart;
    double money;
};

class UserManager {
private:
    unordered_map<string, User> users;
    const string user_file = "users.txt";
    const string admin_username = "BryAiNiVi";
    const string admin_password = "BryAiNiVi";

    void load_users() {
        ifstream file(user_file);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                User user;
                iss >> user.username >> user.password >> user.money;
                users[user.username] = user;

            }
            file.close();
        }
    }

    void save_users() {
        ofstream file(user_file);
        if (file.is_open()) {
            for (const auto& pair : users) {
                file << pair.second.username << " "
                     << pair.second.password << " "
                     << pair.second.money << "\n";
            }
            file.close();
        }
    }

public:
    UserManager() {
        load_users();
    }

    ~UserManager() {
        save_users();
    }

    bool register_user(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            return false;
        }
        users[username] = {username, password};
        save_users();
        return true;
    }

    bool login_user(const string& username, const string& password) {
        if (users.find(username) == users.end()) {
            return false;
        }
        return users[username].password == password;
    }

    bool admin_login(const string& username, const string& password) {
        return username == admin_username && password == admin_password;
    }
    
    bool add_to_cart(const string& username, const string& item) {
        users[username].cart.push_back(item);
        save_users();
    }

    void view_cart(const string& username) {
        cout << username << "Cart" << ":\n";
        for (const auto& item : users[username].cart) {
            cout << "- " << item << "\n";
        }
    }
};

void user_menu(UserManager& userManager, const string& username, unordered_map<string, Item>& inventory) {
    while (true) {
        cout << "1. View Items" << endl;
        cout << "2. Add to Cart" << endl;
        cout << "3. View Cart" << endl;
        cout << "4. View Account Details" << endl;
        cout << "5. Purchase Items" << endl;
        cout << "6. Log out" << endl;
        cout << "Enter choice: ";
        int user_choice;
        cin >> user_choice;
    }
}

int main() {
    UserManager userManager;
    string username;
    string password;
    unordered_map<string, Item> inventory = {};

    while (true) {
        cout << "1. Register" << endl;
        cout << "2. Log in" << endl;
        cout << "3. Admin Login" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;
            if (userManager.register_user(username, password)) {
                cout << "Registration successful!" << endl;
            } else {
                cout << "Username already exists!" << endl;
            }
        } else if (choice == 2) {
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;
            if (userManager.login_user(username, password)) {
                cout << "Login successful!" << endl;
                user_menu(userManager, username, inventory);
            } else {
                cout << "Invalid username or password!" << endl;
            }
        }
    }
}