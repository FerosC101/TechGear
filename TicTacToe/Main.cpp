#include <iostream>
using namespace std;

int main() {
    UserManagement UserManager;
    string username;
    string password;
    while (true) {
        cout << "1. Register" << endl;
        cout << "2. Log in" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (user_manager.register_user(username, password)) {
                cout << "Registration successful!" << endl;
            } else {
                cout << "Username already exists." << endl;
            }
        } else if (choice == 2) {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (user_manager.login(username, password)) {
                cout << "Login successful!" << endl;
                while (true) {
                    cout << "1. Start a new game" << endl;
                    cout << "2. View records" << endl;
                    cout << "3. Log out" << endl;
                    cout << "Enter choice: ";
                    cin >> choice;
                    if (choice == 1) {
                        TicTacToeGame game;
                        game.play_game(user_manager, username);
                    } else if (choice == 2) {
                        user_manager.get_record(username);
                    } else if (choice == 3) {
                        break;
                    }
                }
            } else {
                cout << "Invalid username or password." << endl;
            }
        } else if (choice == 3) {
            break;
        }
    }
    return 0;
}
