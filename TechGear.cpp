#include <iostream>
#include <vector>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <unordered_map>

using namespace std;
using namespace sql;

struct Item {
    string name;
    string category;
    unordered_map<string, string> specs;
    double price;
    int quantity;
};

struct User {
    string username;
    string password;
    vector<string> cart;
    double money;
};

class UserManager {
private:
    Driver* driver;
    Connection* con;
    string admin_username = "BryAiNiVi";
    string admin_password = "BryAiNiVi";

    void connect_to_db() {
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
        con->setSchema("store_db");
    }

public:
    UserManager() {
        connect_to_db();
    }

    ~UserManager() {
        delete con;
    }

    bool register_user(const string& username, const string& password) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("INSERT INTO users(username, password, money) VALUES (?, ?, ?)");
            pstmt->setString(1, username);
            pstmt->setString(2, password);
            pstmt->setDouble(3, 0.0);
            pstmt->execute();
            delete pstmt;
            return true;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
            return false;
        }
    }

    bool login_user(const string& username, const string& password) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("SELECT password FROM users WHERE username = ?");
            pstmt->setString(1, username);
            ResultSet* res = pstmt->executeQuery();
            if (res->next() && res->getString("password") == password) {
                delete res;
                delete pstmt;
                return true;
            }
            delete res;
            delete pstmt;
            return false;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
            return false;
        }
    }

    bool admin_login(const string& username, const string& password) {
        return username == admin_username && password == admin_password;
    }

    bool add_to_cart(const string& username, const string& item) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("INSERT INTO cart(username, item) VALUES (?, ?)");
            pstmt->setString(1, username);
            pstmt->setString(2, item);
            pstmt->execute();
            delete pstmt;
            return true;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
            return false;
        }
    }

    void view_cart(const string& username) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("SELECT item FROM cart WHERE username = ?");
            pstmt->setString(1, username);
            ResultSet* res = pstmt->executeQuery();
            cout << username << "'s Cart:\n";
            while (res->next()) {
                cout << "- " << res->getString("item") << "\n";
            }
            delete res;
            delete pstmt;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void view_account_details(const string& username) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("SELECT username, money FROM users WHERE username = ?");
            pstmt->setString(1, username);
            ResultSet* res = pstmt->executeQuery();
            if (res->next()) {
                cout << "Username: " << res->getString("username") << "\n";
                cout << "Money: " << res->getDouble("money") << "\n";
            }
            delete res;
            delete pstmt;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    bool purchase_items(const string& username, unordered_map<string, Item>& inventory) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("SELECT item FROM cart WHERE username = ?");
            pstmt->setString(1, username);
            ResultSet* res = pstmt->executeQuery();

            double total_cost = 0.0;
            while (res->next()) {
                string item_name = res->getString("item");
                if (inventory.find(item_name) != inventory.end()) {
                    total_cost += inventory[item_name].price;
                }
            }

            delete res;
            delete pstmt;

            pstmt = con->prepareStatement("SELECT money FROM users WHERE username = ?");
            pstmt->setString(1, username);
            res = pstmt->executeQuery();

            if (res->next() && res->getDouble("money") >= total_cost) {
                double new_balance = res->getDouble("money") - total_cost;
                delete res;
                delete pstmt;

                pstmt = con->prepareStatement("UPDATE users SET money = ? WHERE username = ?");
                pstmt->setDouble(1, new_balance);
                pstmt->setString(2, username);
                pstmt->execute();

                for (const auto& item_name : inventory) {
                    if (inventory[item_name.first].quantity > 0) {
                        inventory[item_name.first].quantity--;
                        pstmt = con->prepareStatement("UPDATE inventory SET quantity = ? WHERE name = ?");
                        pstmt->setInt(1, inventory[item_name.first].quantity);
                        pstmt->setString(2, item_name.first);
                        pstmt->execute();
                    }
                }

                pstmt = con->prepareStatement("DELETE FROM cart WHERE username = ?");
                pstmt->setString(1, username);
                pstmt->execute();

                delete pstmt;
                return true;
            }

            delete res;
            delete pstmt;
            return false;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
            return false;
        }
    }
};

class InventoryManager {
private:
    Driver* driver;
    Connection* con;

    void connect_to_db() {
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
        con->setSchema("store_db");
    }

public:
    InventoryManager() {
        connect_to_db();
    }

    ~InventoryManager() {
        delete con;
    }

    unordered_map<string, Item> load_inventory() {
        unordered_map<string, Item> inventory;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM inventory");

            while (res->next()) {
                string name = res->getString("name");
                string category = res->getString("category");
                double price = res->getDouble("price");
                int quantity = res->getInt("quantity");
                unordered_map<string, string> specs;

                PreparedStatement* pstmt = con->prepareStatement("SELECT spec_key, spec_value FROM specs WHERE item_name = ?");
                pstmt->setString(1, name);
                ResultSet* specs_res = pstmt->executeQuery();

                while (specs_res->next()) {
                    specs[specs_res->getString("spec_key")] = specs_res->getString("spec_value");
                }

                delete specs_res;
                delete pstmt;

                inventory[name] = {name, category, specs, price, quantity};
            }

            delete res;
            delete stmt;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
        }
        return inventory;
    }

    void view_inventory() {
        unordered_map<string, Item> inventory = load_inventory();
        for (const auto& pair : inventory) {
            const Item& item = pair.second;
            cout << "Name: " << item.name << "\n";
            cout << "Category: " << item.category << "\n";
            for (const auto& spec : item.specs) {
                cout << spec.first << ": " << spec.second << "\n";
            }
            cout << "Price: " << item.price << "\n";
            cout << "Quantity: " << item.quantity << "\n\n";
        }
    }

    void add_item(const string& name, const Item& item) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("INSERT INTO inventory(name, category, price, quantity) VALUES (?, ?, ?, ?)");
            pstmt->setString(1, name);
            pstmt->setString(2, item.category);
            pstmt->setDouble(3, item.price);
            pstmt->setInt(4, item.quantity);
            pstmt->execute();

            for (const auto& spec : item.specs) {
                pstmt = con->prepareStatement("INSERT INTO specs(item_name, spec_key, spec_value) VALUES (?, ?, ?)");
                pstmt->setString(1, name);
                pstmt->setString(2, spec.first);
                pstmt->setString(3, spec.second);
                pstmt->execute();
            }

            delete pstmt;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void edit_price(const string& name, double new_price) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("UPDATE inventory SET price = ? WHERE name = ?");
            pstmt->setDouble(1, new_price);
            pstmt->setString(2, name);
            pstmt->execute();
            delete pstmt;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void edit_quantity(const string& name, int new_quantity) {
        try {
            PreparedStatement* pstmt = con->prepareStatement("UPDATE inventory SET quantity = ? WHERE name = ?");
            pstmt->setInt(1, new_quantity);
            pstmt->setString(2, name);
            pstmt->execute();
            delete pstmt;
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
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

        if (user_choice == 1) {
            InventoryManager inventoryManager;
            inventoryManager.view_inventory();
        } else if (user_choice == 2) {
            string item;
            cout << "Enter item name: ";
            cin >> item;
            userManager.add_to_cart(username, item);
        } else if (user_choice == 3) {
            userManager.view_cart(username);
        } else if (user_choice == 4) {
            userManager.view_account_details(username);
        } else if (user_choice == 5) {
            if (userManager.purchase_items(username, inventory)) {
                cout << "Purchase successful!" << endl;
            } else {
                cout << "Insufficient funds or item out of stock." << endl;
            }
        } else if (user_choice == 6) {
            break;
        } else {
            cout << "Invalid choice!" << endl;
        }
    }
}

void admin_menu(UserManager& userManager, const string& username, unordered_map<string, Item>& inventory) {
    InventoryManager inventoryManager;
    while (true) {
        cout << "1. Check Profits" << endl;
        cout << "2. Add Items" << endl;
        cout << "3. Edit Price" << endl;
        cout << "4. Edit Quantity" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        int admin_choice;
        cin >> admin_choice;

        if (admin_choice == 1) {
            // Implementation of profit checking
        } else if (admin_choice == 2) {
            string name, category;
            double price;
            int quantity;
            unordered_map<string, string> specs;
            cout << "Enter item name: ";
            cin >> name;
            cout << "Enter category: ";
            cin >> category;
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter quantity: ";
            cin >> quantity;
            // Add specifications
            inventoryManager.add_item(name, {name, category, specs, price, quantity});
        } else if (admin_choice == 3) {
            string name;
            double new_price;
            cout << "Enter item name: ";
            cin >> name;
            cout << "Enter new price: ";
            cin >> new_price;
            inventoryManager.edit_price(name, new_price);
        } else if (admin_choice == 4) {
            string name;
            int new_quantity;
            cout << "Enter item name: ";
            cin >> name;
            cout << "Enter new quantity: ";
            cin >> new_quantity;
            inventoryManager.edit_quantity(name, new_quantity);
        } else if (admin_choice == 5) {
            break;
        } else {
            cout << "Invalid choice!" << endl;
        }
    }
}

int main() {
    UserManager userManager;
    InventoryManager inventoryManager;
    unordered_map<string, Item> inventory;
    string username;
    string password;

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
        } else if (choice == 3) {
            cout << "Enter Admin Username: ";
            cin >> username;
            cout << "Enter Admin Password: ";
            cin >> password;
            if (userManager.admin_login(username, password)) {
                cout << "Admin Login successful!" << endl;
                admin_menu(userManager, username, inventory);
            } else {
                cout << "Invalid admin username or password!" << endl;
            }
        } else if (choice == 4) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}

/*
CREATE DATABASE store_db;
USE store_db;

CREATE TABLE users (
    username VARCHAR(50) PRIMARY KEY,
    password VARCHAR(50),
    money DOUBLE
);

CREATE TABLE inventory (
    name VARCHAR(50) PRIMARY KEY,
    category VARCHAR(50),
    price DOUBLE,
    quantity INT
);

CREATE TABLE specs (
    item_name VARCHAR(50),
    spec_key VARCHAR(50),
    spec_value VARCHAR(50),
    FOREIGN KEY (item_name) REFERENCES inventory(name)
);

CREATE TABLE cart (
    username VARCHAR(50),
    item VARCHAR(50),
    FOREIGN KEY (username) REFERENCES users(username),
    FOREIGN KEY (item) REFERENCES inventory(name)
);
*/
