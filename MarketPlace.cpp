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
    string category;
    unordered_map<string, string> specs;
    double price;
    int quantity;
    double cost_price;

    Item() : name(""), category(""), price(0.0), quantity(0), cost_price(0.0) {}

    Item(string n, string c, unordered_map<string, string> s, double p, int q, double cp)
        : name(n), category(c), specs(s), price(p), quantity(q), cost_price(cp) {}
};

struct CartNode {
    string item_name;
    CartNode* next;

    CartNode(string name) : item_name(name), next(nullptr) {}
};

class Cart {
private:
    CartNode* head;

public:
    Cart() : head(nullptr) {}

    ~Cart() {
        clear_cart();
    }

    void add_item(const string& item_name) {
        CartNode* new_node = new CartNode(item_name);
        new_node->next = head;
        head = new_node;
    }

    void view_cart() const {
        CartNode* current = head;
        while (current) {
            cout << "- " << current->item_name << "\n";
            current = current->next;
        }
    }

    double total_cost(const unordered_map<string, Item>& inventory) const {
        double total = 0.0;
        CartNode* current = head;
        while (current) {
            total += inventory.at(current->item_name).price;
            current = current->next;
        }
        return total;
    }

    void clear_cart() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    CartNode* get_head() const {
        return head;
    }
};

struct User {
    string username;
    string password;
    Cart cart;
    double money;
    vector<string> purchase_history;

    User() : username(""), password(""), money(0.0) {}
};

class UserManager {
private:
    unordered_map<string, User> users;
    const string user_file = "users.txt";
    const string admin_username = "admin";
    const string admin_password = "admin";

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
        User newUser;
        newUser.username = username;
        newUser.password = password;
        newUser.money = 0.0;

        users[username] = newUser;
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
        users[username].cart.add_item(item);
        return true;
    }

    void view_cart(const string& username) {
        cout << username << "'s Cart:\n";
        users[username].cart.view_cart();
    }

    double get_user_money(const string& username) {
        return users[username].money;
    }

    Cart& get_cart(const string& username) {
        return users[username].cart;
    }

    CartNode* get_cart_head(const string& username) const {
        return users.at(username).cart.get_head();
    }

    void add_money(const string& username, double amount) {
        users[username].money += amount;
        save_users();
    }

    void deduct_money(const string& username, double amount) {
        users[username].money -= amount;
        save_users();
    }

    void clear_cart(const string& username) {
        users[username].cart.clear_cart();
    }

    void add_to_purchase_history(const string& username, const string& purchase_record) {
        users[username].purchase_history.push_back(purchase_record);
    }

    const vector<string>& get_purchase_history(const string& username) const {
        return users.at(username).purchase_history;
    }
};

class InventoryManager {
private:
    unordered_map<string, Item> inventory;
    double total_profit;

public:
    InventoryManager() : total_profit(0.0) {
        load_inventory();
    }

    void load_inventory() {
        inventory = {
            {"Laptop", {"Laptop", "Electronics", {{"Brand", "Dell"}, {"RAM", "16GB"}, {"Storage", "512GB SSD"}}, 999.99, 10, 800.0}},
            {"Smartphone", {"Smartphone", "Electronics", {{"Brand", "Apple"}, {"Model", "iPhone 13"}, {"Storage", "128GB"}}, 799.99, 15, 600.0}},
            {"Smartwatch", {"Smartwatch", "Wearables", {{"Brand", "Samsung"}, {"Model", "Galaxy Watch"}, {"Battery Life", "48 hours"}}, 199.99, 20, 120.0}},
            {"Tablet", {"Tablet", "Electronics", {{"Brand", "Microsoft"}, {"Model", "Surface Pro"}, {"Storage", "256GB"}}, 899.99, 8, 700.0}},
            {"Headphones", {"Headphones", "Audio", {{"Brand", "Bose"}, {"Model", "QuietComfort"}, {"Type", "Over-Ear"}}, 299.99, 25, 200.0}},
            {"Camera", {"Camera", "Photography", {{"Brand", "Canon"}, {"Model", "EOS R5"}, {"Resolution", "45MP"}}, 3899.99, 5, 3500.0}},
            {"Gaming Console", {"Gaming Console", "Entertainment", {{"Brand", "Sony"}, {"Model", "PlayStation 5"}, {"Storage", "1TB"}}, 499.99, 30, 400.0}},
            {"Router", {"Router", "Networking", {{"Brand", "Netgear"}, {"Model", "Nighthawk"}, {"Speed", "1Gbps"}}, 129.99, 40, 100.0}},
            {"Monitor", {"Monitor", "Electronics", {{"Brand", "LG"}, {"Size", "27 inches"}, {"Resolution", "4K"}}, 349.99, 12, 250.0}},
            {"Keyboard", {"Keyboard", "Peripherals", {{"Brand", "Logitech"}, {"Type", "Mechanical"}, {"Connectivity", "Wireless"}}, 79.99, 50, 60.0}},
            {"Mouse", {"Mouse", "Peripherals", {{"Brand", "Razer"}, {"Type", "Gaming"}, {"DPI", "16000"}}, 59.99, 60, 40.0}},
            {"External Hard Drive", {"External Hard Drive", "Storage", {{"Brand", "Western Digital"}, {"Capacity", "2TB"}, {"Type", "SSD"}}, 129.99, 25, 80.0}},
            {"Printer", {"Printer", "Peripherals", {{"Brand", "HP"}, {"Model", "OfficeJet Pro"}, {"Type", "All-in-One"}}, 199.99, 18, 150.0}},
            {"Speakers", {"Speakers", "Audio", {{"Brand", "JBL"}, {"Type", "Portable"}, {"Battery Life", "20 hours"}}, 99.99, 35, 70.0}},
            {"Smart Home Hub", {"Smart Home Hub", "Smart Home", {{"Brand", "Google"}, {"Model", "Nest Hub"}, {"Voice Assistant", "Google Assistant"}}, 129.99, 10, 90.0}}
        };
    }

    void view_inventory() {
        for (const auto& pair : inventory) {
            const auto& item = pair.second;
            cout << "Name: " << item.name << endl;
            cout << "Category: " << item.category << endl;
            for (const auto& spec : item.specs) {
                cout << spec.first << ": " << spec.second << endl;
            }
            cout << "Price: $" << item.price << endl;
            cout << "Quantity: " << item.quantity << endl;
            cout << "--------------------------------" << endl;
        }
    }

    bool add_item(const Item& item) {
        if (inventory.find(item.name) != inventory.end()) {
            return false;
        }
        inventory[item.name] = item;
        return true;
    }

    bool edit_price(const string& item_name, double new_price) {
        if (inventory.find(item_name) == inventory.end()) {
            return false;
        }
        inventory[item_name].price = new_price;
        return true;
    }

    bool edit_quantity(const string& item_name, int new_quantity) {
        if (inventory.find(item_name) == inventory.end()) {
            return false;
        }
        inventory[item_name].quantity = new_quantity;
        return true;
    }

    void decrease_item_quantity(const string& item_name) {
        if (inventory.find(item_name) != inventory.end()) {
            inventory[item_name].quantity--;
        }
    }

    double calculate_total_profit() const {
        double total = 0.0;
        for (const auto& pair : inventory) {
            const auto& item = pair.second;
            total += (item.price - item.cost_price) * (item.quantity);
        }
        return total;
    }

    void update_profit(double amount) {
        total_profit += amount;
    }

    double get_total_profit() const {
        return total_profit;
    }

    const unordered_map<string, Item>& get_inventory() const {
        return inventory;
    }
};

void admin_menu(UserManager& userManager, InventoryManager& inventoryManager) {
    while (true) {
        cout << "\nAdmin Menu:" << endl;
        cout << "1. View Inventory" << endl;
        cout << "2. Add Item to Inventory" << endl;
        cout << "3. Edit Item Price" << endl;
        cout << "4. Edit Item Quantity" << endl;
        cout << "5. View Profits" << endl;
        cout << "6. Log out" << endl;
        cout << "Enter choice: ";
        int admin_choice;
        cin >> admin_choice;

        if (admin_choice == 1) {
            inventoryManager.view_inventory();
        } else if (admin_choice == 2) {
            string name, category;
            double price, cost_price;
            int quantity;
            unordered_map<string, string> specs;

            cout << "Enter name: ";
            cin >> name;
            cout << "Enter category: ";
            cin >> category;
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter cost price: ";
            cin >> cost_price;
            cout << "Enter quantity: ";
            cin >> quantity;

            Item newItem(name, category, specs, price, quantity, cost_price);
            if (inventoryManager.add_item(newItem)) {
                cout << "Item added successfully!" << endl;
            } else {
                cout << "Failed to add item. Item may already exist." << endl;
            }
        } else if (admin_choice == 3) {
            string item_name;
            double new_price;
            cout << "Enter item name: ";
            cin >> item_name;
            cout << "Enter new price: ";
            cin >> new_price;

            if (inventoryManager.edit_price(item_name, new_price)) {
                cout << "Price updated successfully!" << endl;
            } else {
                cout << "Item not found. Price update failed." << endl;
            }
        } else if (admin_choice == 4) {
            string item_name;
            int new_quantity;
            cout << "Enter item name: ";
            cin >> item_name;
            cout << "Enter new quantity: ";
            cin >> new_quantity;

            if (inventoryManager.edit_quantity(item_name, new_quantity)) {
                cout << "Quantity updated successfully!" << endl;
            } else {
                cout << "Item not found. Quantity update failed." << endl;
            }
        } else if (admin_choice == 5) {
            cout << "Total Profits: $" << inventoryManager.get_total_profit() << endl;
        } else if (admin_choice == 6) {
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void user_menu(UserManager& userManager, InventoryManager& inventoryManager, const string& username) {
    while (true) {
        cout << "\nUser Menu:" << endl;
        cout << "1. View Items" << endl;
        cout << "2. Add to Cart" << endl;
        cout << "3. View Cart" << endl;
        cout << "4. View Account Details" << endl;
        cout << "5. Purchase Items" << endl;
        cout << "6. Top Up Money" << endl;
        cout << "7. View Purchase History" << endl;
        cout << "8. Log out" << endl;
        cout << "Enter choice: ";
        int user_choice;
        cin >> user_choice;

        if (user_choice == 1) {
            inventoryManager.view_inventory();
        } else if (user_choice == 2) {
            string item_name;
            cout << "Enter item name to add to cart: ";
            cin >> item_name;
            if (inventoryManager.get_inventory().find(item_name) == inventoryManager.get_inventory().end()) {
                cout << "Item not found!" << endl;
            } else {
                userManager.add_to_cart(username, item_name);
                cout << "Item added to cart!" << endl;
            }
        } else if (user_choice == 3) {
            userManager.view_cart(username);
        } else if (user_choice == 4) {
            cout << "Account Details:" << endl;
            cout << "Username: " << username << endl;
            cout << "Money: $" << userManager.get_user_money(username) << endl;
        } else if (user_choice == 5) {
            double total_cost = userManager.get_cart(username).total_cost(inventoryManager.get_inventory());
            double user_money = userManager.get_user_money(username);

            if (total_cost > user_money) {
                cout << "Insufficient funds!" << endl;
            } else {
                CartNode* current = userManager.get_cart_head(username);
                while (current != nullptr) {
                    inventoryManager.decrease_item_quantity(current->item_name);
                    current = current->next;
                }

                userManager.deduct_money(username, total_cost);
                userManager.clear_cart(username);

                time_t now = time(0);
                tm* ltm = localtime(&now);
                stringstream purchase_record;
                purchase_record << "[" << 1900 + ltm->tm_year << "-" 
                                << 1 + ltm->tm_mon << "-" << ltm->tm_mday << "] ";
                purchase_record << "Total Cost: $" << total_cost;
                userManager.add_to_purchase_history(username, purchase_record.str());

                inventoryManager.update_profit(total_cost - (total_cost * 0.1));
                cout << "Purchase successful!" << endl;
            }
        } else if (user_choice == 6) {
            double amount;
            cout << "Enter amount to top up: $";
            cin >> amount;
            userManager.add_money(username, amount);
            cout << "Top-up successful!" << endl;
        } else if (user_choice == 7) {
            cout << "Purchase History:" << endl;
            for (const auto& purchase : userManager.get_purchase_history(username)) {
                cout << purchase << endl;
            }
        } else if (user_choice == 8) {
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}


void display_menu() {
    cout << "Welcome to the Online Store!" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Admin Login" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter choice: ";
}

int main() {
    UserManager userManager;
    InventoryManager inventoryManager;

    while (true) {
        display_menu();
        int choice;
        cin >> choice;

        if (choice == 1) {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (userManager.login_user(username, password)) {
                cout << "Login successful!" << endl;
                user_menu(userManager, inventoryManager, username);
            } else {
                cout << "Login failed. Incorrect username or password." << endl;
            }
        } else if (choice == 2) {
            string username, password;
            cout << "Enter new username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (userManager.register_user(username, password)) {
                cout << "Registration successful!" << endl;
            } else {
                cout << "Registration failed. Username already exists." << endl;
            }
        } else if (choice == 3) {
            string username, password;
            cout << "Enter admin username: ";
            cin >> username;
            cout << "Enter admin password: ";
            cin >> password;

            if (userManager.admin_login(username, password)) {
                cout << "Admin login successful!" << endl;
                admin_menu(userManager, inventoryManager);
            } else {
                cout << "Admin login failed. Incorrect username or password." << endl;
            }
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    cout << "Thank you for using the Online Store!" << endl;
    return 0;
}
