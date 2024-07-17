#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
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
    const string admin_username = "admin";
    const string admin_password = "admin";

public:
    UserManager() {}

    bool register_user(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            return false;
        }
        User newUser;
        newUser.username = username;
        newUser.password = password;
        newUser.money = 0.0;

        users[username] = newUser;
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
    }

    void deduct_money(const string& username, double amount) {
        users[username].money -= amount;
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
    unordered_map<string, Item> laptops;
    unordered_map<string, Item> phones;
    unordered_map<string, Item> others;

public:
    InventoryManager() {
        load_inventory();
    }

    void load_inventory() {
        laptops = {
            {"Laptop1", {"Laptop1", "Laptops", {{"Brand", "Dell"}, {"RAM", "16GB"}, {"Storage", "512GB SSD"}}, 999.99, 10, 800.0}},
            {"Laptop2", {"Laptop2", "Laptops", {{"Brand", "Apple"}, {"RAM", "16GB"}, {"Storage", "256GB SSD"}}, 1299.99, 5, 1000.0}},
            {"Laptop3", {"Laptop3", "Laptops", {{"Brand", "HP"}, {"RAM", "8GB"}, {"Storage", "1TB HDD"}}, 699.99, 8, 500.0}},
            {"Laptop4", {"Laptop4", "Laptops", {{"Brand", "Lenovo"}, {"RAM", "16GB"}, {"Storage", "1TB SSD"}}, 1199.99, 6, 900.0}},
            {"Laptop5", {"Laptop5", "Laptops", {{"Brand", "Asus"}, {"RAM", "32GB"}, {"Storage", "2TB SSD"}}, 1899.99, 3, 1500.0}},
            {"Laptop6", {"Laptop6", "Laptops", {{"Brand", "Acer"}, {"RAM", "8GB"}, {"Storage", "512GB SSD"}}, 799.99, 12, 600.0}}
        };

        phones = {
            {"Phone1", {"Phone1", "Phones", {{"Brand", "Apple"}, {"RAM", "6GB"}, {"Storage", "128GB"}}, 699.99, 15, 600.0}},
            {"Phone2", {"Phone2", "Phones", {{"Brand", "Samsung"}, {"RAM", "8GB"}, {"Storage", "256GB"}}, 899.99, 10, 700.0}},
            {"Phone3", {"Phone3", "Phones", {{"Brand", "Google"}, {"RAM", "4GB"}, {"Storage", "64GB"}}, 499.99, 20, 400.0}},
            {"Phone4", {"Phone4", "Phones", {{"Brand", "OnePlus"}, {"RAM", "12GB"}, {"Storage", "256GB"}}, 1099.99, 8, 900.0}},
            {"Phone5", {"Phone5", "Phones", {{"Brand", "Xiaomi"}, {"RAM", "8GB"}, {"Storage", "128GB"}}, 599.99, 18, 500.0}},
            {"Phone6", {"Phone6", "Phones", {{"Brand", "Sony"}, {"RAM", "6GB"}, {"Storage", "64GB"}}, 449.99, 25, 350.0}}
        };

        others = {
            {"Smartwatch", {"Smartwatch", "Others", {{"Brand", "Samsung"}, {"Model", "Galaxy Watch"}, {"Battery Life", "48 hours"}}, 199.99, 20, 120.0}},
            {"Tablet", {"Tablet", "Others", {{"Brand", "Microsoft"}, {"Model", "Surface Pro"}, {"Storage", "256GB"}}, 899.99, 8, 700.0}},
            {"Headphones", {"Headphones", "Others", {{"Brand", "Bose"}, {"Model", "QuietComfort"}, {"Type", "Over-Ear"}}, 299.99, 25, 200.0}},
            {"Camera", {"Camera", "Others", {{"Brand", "Canon"}, {"Model", "EOS R5"}, {"Resolution", "45MP"}}, 3899.99, 5, 3500.0}},
            {"Gaming Console", {"Gaming Console", "Others", {{"Brand", "Sony"}, {"Model", "PlayStation 5"}, {"Storage", "1TB"}}, 499.99, 30, 400.0}},
            {"Router", {"Router", "Others", {{"Brand", "Netgear"}, {"Model", "Nighthawk"}, {"Speed", "1Gbps"}}, 129.99, 40, 100.0}},
            {"Monitor", {"Monitor", "Others", {{"Brand", "LG"}, {"Size", "27 inches"}, {"Resolution", "4K"}}, 349.99, 12, 250.0}},
            {"Keyboard", {"Keyboard", "Others", {{"Brand", "Logitech"}, {"Type", "Mechanical"}, {"Connectivity", "Wireless"}}, 79.99, 50, 60.0}},
            {"Mouse", {"Mouse", "Others", {{"Brand", "Razer"}, {"Type", "Gaming"}, {"DPI", "16000"}}, 59.99, 60, 40.0}},
            {"External Hard Drive", {"External Hard Drive", "Others", {{"Brand", "Western Digital"}, {"Capacity", "2TB"}, {"Type", "SSD"}}, 129.99, 25, 80.0}},
            {"Printer", {"Printer", "Others", {{"Brand", "HP"}, {"Model", "OfficeJet Pro"}, {"Type", "All-in-One"}}, 199.99, 18, 150.0}},
            {"Speakers", {"Speakers", "Others", {{"Brand", "JBL"}, {"Type", "Portable"}, {"Battery Life", "20 hours"}}, 99.99, 35, 70.0}},
            {"Smart Home Hub", {"Smart Home Hub", "Others", {{"Brand", "Google"}, {"Model", "Nest Hub"}, {"Voice Assistant", "Google Assistant"}}, 129.99, 10, 90.0}}
        };
    }

    void view_inventory() {
        int choice;
        cout << "Select category to view:\n";
        cout << "1. Laptops\n";
        cout << "2. Phones\n";
        cout << "3. Others\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "--- Laptops ---\n";
                view_category(laptops);
                break;
            case 2:
                cout << "--- Phones ---\n";
                view_category(phones);
                break;
            case 3:
                cout << "--- Others ---\n";
                view_category(others);
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }

    void view_category(const unordered_map<string, Item>& category) {
        for (const auto& pair : category) {
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
        string category = item.category;
        if (category == "Laptops") {
            if (laptops.find(item.name) != laptops.end()) {
                return false;
            }
            laptops[item.name] = item;
        } else if (category == "Phones") {
            if (phones.find(item.name) != phones.end()) {
                return false;
            }
            phones[item.name] = item;
        } else {
            if (others.find(item.name) != others.end()) {
                return false;
            }
            others[item.name] = item;
        }
        return true;
    }

    bool edit_price(const string& item_name, double new_price) {
        if (laptops.find(item_name) != laptops.end()) {
            laptops[item_name].price = new_price;
            return true;
        } else if (phones.find(item_name) != phones.end()) {
            phones[item_name].price = new_price;
            return true;
        } else if (others.find(item_name) != others.end()) {
            others[item_name].price = new_price;
            return true;
        }
        return false;
    }

    bool edit_quantity(const string& item_name, int new_quantity) {
        if (laptops.find(item_name) != laptops.end()) {
            laptops[item_name].quantity = new_quantity;
            return true;
        } else if (phones.find(item_name) != phones.end()) {
            phones[item_name].quantity = new_quantity;
            return true;
        } else if (others.find(item_name) != others.end()) {
            others[item_name].quantity = new_quantity;
            return true;
        }
        return false;
    }

    void decrease_item_quantity(const string& item_name) {
        if (laptops.find(item_name) != laptops.end()) {
            laptops[item_name].quantity--;
        } else if (phones.find(item_name) != phones.end()) {
            phones[item_name].quantity--;
        } else if (others.find(item_name) != others.end()) {
            others[item_name].quantity--;
        }
    }

    double calculate_total_profit() const {
        double total = 0.0;
        total += calculate_category_profit(laptops);
        total += calculate_category_profit(phones);
        total += calculate_category_profit(others);
        return total;
    }

    double calculate_category_profit(const unordered_map<string, Item>& category) const {
        double category_total = 0.0;
        for (const auto& pair : category) {
            const auto& item = pair.second;
            double profit_per_item = item.price - item.cost_price;
            category_total += profit_per_item * (10 - item.quantity); // Assuming initial quantity was 10 for all items
        }
        return category_total;
    }

    bool item_exists(const string& item_name) const {
        return laptops.find(item_name) != laptops.end()
            || phones.find(item_name) != phones.end()
            || others.find(item_name) != others.end();
    }

    void search_by_spec(const string& spec_key, const string& spec_value) {
        bool found = false;
        found = search_in_category(laptops, spec_key, spec_value) || found;
        found = search_in_category(phones, spec_key, spec_value) || found;
        found = search_in_category(others, spec_key, spec_value) || found;

        if (!found) {
            cout << "No items found with " << spec_key << " = " << spec_value << endl;
        }
    }

    bool search_in_category(const unordered_map<string, Item>& category, const string& spec_key, const string& spec_value) {
        bool found = false;
        for (const auto& pair : category) {
            const auto& item = pair.second;
            if (item.specs.find(spec_key) != item.specs.end() && item.specs.at(spec_key) == spec_value) {
                cout << "Name: " << item.name << endl;
                cout << "Category: " << item.category << endl;
                for (const auto& spec : item.specs) {
                    cout << spec.first << ": " << spec.second << endl;
                }
                cout << "Price: $" << item.price << endl;
                cout << "Quantity: " << item.quantity << endl;
                cout << "--------------------------------" << endl;
                found = true;
            }
        }
        return found;
    }

    const unordered_map<string, Item>& get_inventory() const {
        return laptops; // Returning only laptops as an example; modify as needed
    }
};

class Techgear {
private:
    UserManager userManager;
    InventoryManager inventoryManager;

public:
    void admin_menu() {
        int choice;
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. View Inventory\n";
            cout << "2. Add Item\n";
            cout << "3. Edit Price\n";
            cout << "4. Edit Quantity\n";
            cout << "5. Calculate Total Profit\n";
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    inventoryManager.view_inventory();
                    break;
                case 2: {
                    string name, category, spec_key, spec_value;
                    unordered_map<string, string> specs;
                    double price, cost_price;
                    int quantity;
                    cout << "Enter item name: ";
                    cin >> name;
                    cout << "Enter category (Laptops, Phones, Others): ";
                    cin >> category;
                    cout << "Enter price: ";
                    cin >> price;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    cout << "Enter cost price: ";
                    cin >> cost_price;

                    cout << "Enter specs (key value pairs, 'done' to finish):\n";
                    while (true) {
                        cin >> spec_key;
                        if (spec_key == "done") break;
                        cin >> spec_value;
                        specs[spec_key] = spec_value;
                    }

                    Item newItem(name, category, specs, price, quantity, cost_price);
                    if (inventoryManager.add_item(newItem)) {
                        cout << "Item added successfully.\n";
                    } else {
                        cout << "Item already exists.\n";
                    }
                    break;
                }
                case 3: {
                    string item_name;
                    double new_price;
                    cout << "Enter item name: ";
                    cin >> item_name;
                    cout << "Enter new price: ";
                    cin >> new_price;
                    if (inventoryManager.edit_price(item_name, new_price)) {
                        cout << "Price updated successfully.\n";
                    } else {
                        cout << "Item not found.\n";
                    }
                    break;
                }
                case 4: {
                    string item_name;
                    int new_quantity;
                    cout << "Enter item name: ";
                    cin >> item_name;
                    cout << "Enter new quantity: ";
                    cin >> new_quantity;
                    if (inventoryManager.edit_quantity(item_name, new_quantity)) {
                        cout << "Quantity updated successfully.\n";
                    } else {
                        cout << "Item not found.\n";
                    }
                    break;
                }
                case 5:
                    cout << "Total profit: $" << inventoryManager.calculate_total_profit() << "\n";
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    void user_menu(const string& username) {
        int choice;
        while (true) {
            cout << "\n--- User Menu ---\n";
            cout << "1. View Inventory\n";
            cout << "2. Add Item to Cart\n";
            cout << "3. View Cart\n";
            cout << "4. Checkout\n";
            cout << "5. Search Specs\n";
            cout << "6. View Purchase History\n";
            cout << "7. Add Money to Account\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    inventoryManager.view_inventory();
                    break;
                case 2: {
                    string item_name;
                    cout << "Enter item name: ";
                    cin >> item_name;
                    if (inventoryManager.item_exists(item_name)) {
                        userManager.add_to_cart(username, item_name);
                        cout << "Item added to cart.\n";
                    } else {
                        cout << "Item not found.\n";
                    }
                    break;
                }
                case 3:
                    userManager.view_cart(username);
                    break;
                case 4: {
                    double total_cost = userManager.get_cart(username).total_cost(inventoryManager.get_inventory());
                    double user_money = userManager.get_user_money(username);

                    if (total_cost > user_money) {
                        cout << "Insufficient funds. Total cost: $" << total_cost << ", Available balance: $" << user_money << "\n";
                    } else {
                        userManager.deduct_money(username, total_cost);
                        CartNode* current = userManager.get_cart_head(username);
                        string purchase_record = "Purchased items: ";
                        while (current) {
                            inventoryManager.decrease_item_quantity(current->item_name);
                            purchase_record += current->item_name + ", ";
                            current = current->next;
                        }
                        purchase_record += "Total cost: $" + to_string(total_cost);
                        userManager.add_to_purchase_history(username, purchase_record);
                        userManager.clear_cart(username);
                        cout << "Checkout successful! Total cost: $" << total_cost << "\n";
                    }
                    break;
                }
                case 5: {
                    string spec_key, spec_value;
                    cout << "Enter specification key: ";
                    cin >> spec_key;
                    cout << "Enter specification value: ";
                    cin >> spec_value;
                    inventoryManager.search_by_spec(spec_key, spec_value);
                    break;
                }
                case 6: {
                    const vector<string>& history = userManager.get_purchase_history(username);
                    for (const auto& record : history) {
                        cout << record << "\n";
                    }
                    break;
                }
                case 7: {
                    double amount;
                    cout << "Enter amount to add: ";
                    cin >> amount;
                    userManager.add_money(username, amount);
                    cout << "$" << amount << " added to your account.\n";
                    break;
                }
                case 8:
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    void main_menu() {
        int choice;
        string username, password;

        while (true) {
            cout << "\n--- Main Menu ---\n";
            cout << "1. Admin Login\n";
            cout << "2. User Login\n";
            cout << "3. Register\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Enter admin username: ";
                    cin >> username;
                    cout << "Enter admin password: ";
                    cin >> password;
                    if (userManager.admin_login(username, password)) {
                        admin_menu();
                    } else {
                        cout << "Invalid admin credentials.\n";
                    }
                    break;
                case 2:
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    if (userManager.login_user(username, password)) {
                        user_menu(username);
                    } else {
                        cout << "Invalid username or password.\n";
                    }
                    break;
                case 3:
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    if (userManager.register_user(username, password)) {
                        cout << "Registration successful. You can now log in.\n";
                    } else {
                        cout << "Username already exists. Please choose a different username.\n";
                    }
                    break;
                case 4:
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    Techgear techgear;
    techgear.main_menu();
    return 0;
}
