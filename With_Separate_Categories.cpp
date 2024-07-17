#include <iostream>
#include <string>
#include <vector>
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

    CartNode* get_head() const {
        return head;
    }

    void clear_cart() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
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
    double total_profit;

public:
    InventoryManager() : total_profit(0.0) {
        load_inventory();
    }

    void load_inventory() {
        laptops = {
            {"Laptop1", {"Laptop1", "Laptops", {{"Brand", "Dell"}, {"RAM", "16GB"}, {"Storage", "512GB SSD"}}, 999.99, 10, 800.0}},
            {"Laptop2", {"Laptop2", "Laptops", {{"Brand", "HP"}, {"RAM", "8GB"}, {"Storage", "256GB SSD"}}, 599.99, 10, 450.0}},
            {"Laptop3", {"Laptop3", "Laptops", {{"Brand", "Apple"}, {"RAM", "16GB"}, {"Storage", "1TB SSD"}}, 1999.99, 10, 1600.0}},
            {"Laptop4", {"Laptop4", "Laptops", {{"Brand", "Lenovo"}, {"RAM", "8GB"}, {"Storage", "512GB SSD"}}, 799.99, 10, 650.0}},
            {"Laptop5", {"Laptop5", "Laptops", {{"Brand", "Asus"}, {"RAM", "32GB"}, {"Storage", "1TB SSD"}}, 1499.99, 10, 1200.0}},
            {"Laptop6", {"Laptop6", "Laptops", {{"Brand", "Acer"}, {"RAM", "4GB"}, {"Storage", "128GB SSD"}}, 399.99, 10, 300.0}}
        };

        phones = {
            {"Phone1", {"Phone1", "Phones", {{"Brand", "Apple"}, {"RAM", "16GB"}, {"Storage", "128GB"}}, 799.99, 15, 600.0}},
            {"Phone2", {"Phone2", "Phones", {{"Brand", "Samsung"}, {"RAM", "8GB"}, {"Storage", "256GB"}}, 699.99, 15, 500.0}},
            {"Phone3", {"Phone3", "Phones", {{"Brand", "Google"}, {"RAM", "6GB"}, {"Storage", "128GB"}}, 499.99, 15, 350.0}},
            {"Phone4", {"Phone4", "Phones", {{"Brand", "OnePlus"}, {"RAM", "12GB"}, {"Storage", "256GB"}}, 599.99, 15, 450.0}},
            {"Phone5", {"Phone5", "Phones", {{"Brand", "Sony"}, {"RAM", "8GB"}, {"Storage", "128GB"}}, 649.99, 15, 500.0}},
            {"Phone6", {"Phone6", "Phones", {{"Brand", "LG"}, {"RAM", "4GB"}, {"Storage", "64GB"}}, 399.99, 15, 300.0}}
        };

        others = {
            {"Smartwatch", {"Smartwatch", "Wearables", {{"Brand", "Samsung"}, {"Model", "Galaxy Watch"}, {"Battery Life", "48 hours"}}, 199.99, 20, 120.0}},
            {"Tablet", {"Tablet", "Electronics", {{"Brand", "Microsoft"}, {"Model", "Surface Pro"}, {"Storage", "256GB"}}, 899.99, 8, 700.0}},
            {"Headphones", {"Headphones", "Audio", {{"Brand", "Bose"}, {"Model", "QuietComfort"}, {"Type", "Over-Ear"}}, 299.99, 25, 200.0}},
            {"Camera", {"Camera", "Photography", {{"Brand", "Canon"}, {"Model", "EOS R5"}, {"Resolution", "45MP"}}, 3899.99, 5, 3500.0}},
            {"Gaming Console", {"Gaming Console", "Entertainment", {{"Brand", "Sony"}, {"Model", "PlayStation 5"}, {"Storage", "1TB"}}, 499.99, 30, 400.0}},
            {"Router", {"Router", "Networking", {{"Brand", "Netgear"}, {"Model", "Nighthawk"}, {"Speed", "1Gbps"}}, 129.99, 40, 100.0}}
        };
    }

    void view_inventory() {
        int choice;
        cout << "\n--- Select Inventory Category ---\n";
        cout << "1. Laptops\n";
        cout << "2. Phones\n";
        cout << "3. Others\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                view_category(laptops);
                break;
            case 2:
                view_category(phones);
                break;
            case 3:
                view_category(others);
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    void view_category(const unordered_map<string, Item>& category) {
        for (const auto& pair : category) {
            const Item& item = pair.second;
            cout << "Name: " << item.name << "\n";
            cout << "Category: " << item.category << "\n";
            cout << "Specifications:\n";
            for (const auto& spec : item.specs) {
                cout << "  - " << spec.first << ": " << spec.second << "\n";
            }
            cout << "Price: $" << item.price << "\n";
            cout << "Quantity: " << item.quantity << "\n";
            cout << "Cost Price: $" << item.cost_price << "\n";
            cout << "----------------------\n";
        }
    }

    bool is_in_stock(const string& item_name) {
        return laptops.find(item_name) != laptops.end() || phones.find(item_name) != phones.end() || others.find(item_name) != others.end();
    }

    Item& get_item(const string& item_name) {
        if (laptops.find(item_name) != laptops.end()) {
            return laptops[item_name];
        }
        if (phones.find(item_name) != phones.end()) {
            return phones[item_name];
        }
        if (others.find(item_name) != others.end()) {
            return others[item_name];
        }
        throw invalid_argument("Item not found in inventory.");
    }

    void update_stock(const string& item_name, int quantity) {
        if (laptops.find(item_name) != laptops.end()) {
            laptops[item_name].quantity -= quantity;
        } else if (phones.find(item_name) != phones.end()) {
            phones[item_name].quantity -= quantity;
        } else if (others.find(item_name) != others.end()) {
            others[item_name].quantity -= quantity;
        }
    }

    void calculate_profit(const string& item_name, int quantity) {
        Item& item = get_item(item_name);
        total_profit += (item.price - item.cost_price) * quantity;
    }

    double get_total_profit() const {
        return total_profit;
    }

    double calculate_cart_total(const Cart& cart) {
        double totalCost = 0.0;
        CartNode* current = cart.get_head();
        while (current) {
            totalCost += get_item(current->item_name).price;
            current = current->next;
        }
        return totalCost;
    }
};

int main() {
    UserManager userManager;
    InventoryManager inventoryManager;

    srand(static_cast<unsigned int>(time(0)));

    int choice;
    string username, password, item_name;
    double amount;

    while (true) {
        cout << "\n--- Menu ---\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (userManager.register_user(username, password)) {
                    cout << "Registration successful!\n";
                } else {
                    cout << "Username already exists. Please choose a different username.\n";
                }
                break;

            case 2:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (userManager.login_user(username, password)) {
                    cout << "Login successful!\n";
                    while (true) {
                        cout << "\n--- User Menu ---\n";
                        cout << "1. View Inventory\n";
                        cout << "2. Add to Cart\n";
                        cout << "3. View Cart\n";
                        cout << "4. Add Money\n";
                        cout << "5. Checkout\n";
                        cout << "6. View Purchase History\n";
                        cout << "7. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        if (choice == 7) {
                            break;
                        }

                        switch (choice) {
                            case 1:
                                inventoryManager.view_inventory();
                                break;

                            case 2:
                                cout << "Enter item name to add to cart: ";
                                cin.ignore();
                                getline(cin, item_name);
                                if (inventoryManager.is_in_stock(item_name)) {
                                    userManager.add_to_cart(username, item_name);
                                    cout << "Item added to cart.\n";
                                } else {
                                    cout << "Item not found in inventory.\n";
                                }
                                break;

                            case 3:
                                userManager.view_cart(username);
                                break;

                            case 4:
                                cout << "Enter amount to add: ";
                                cin >> amount;
                                userManager.add_money(username, amount);
                                cout << "Money added successfully.\n";
                                break;

                            case 5: {
                                Cart& cart = userManager.get_cart(username);
                                double totalCost = inventoryManager.calculate_cart_total(cart);

                                if (totalCost <= userManager.get_user_money(username)) {
                                    userManager.deduct_money(username, totalCost);
                                    userManager.clear_cart(username);

                                    cout << "Checkout successful!\n";

                                    string purchase_record = "Purchase made: ";
                                    CartNode* current = userManager.get_cart_head(username);
                                    while (current) {
                                        purchase_record += current->item_name + " ";
                                        inventoryManager.update_stock(current->item_name, 1);
                                        inventoryManager.calculate_profit(current->item_name, 1);
                                        current = current->next;
                                    }
                                    userManager.add_to_purchase_history(username, purchase_record);
                                } else {
                                    cout << "Insufficient funds.\n";
                                }
                                break;
                            }

                            case 6:
                                for (const string& record : userManager.get_purchase_history(username)) {
                                    cout << record << "\n";
                                }
                                break;

                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                    }
                } else {
                    cout << "Invalid username or password.\n";
                }
                break;

            case 3:
                cout << "Enter admin username: ";
                cin >> username;
                cout << "Enter admin password: ";
                cin >> password;
                if (userManager.admin_login(username, password)) {
                    cout << "Admin login successful!\n";
                    cout << "Total profit: $" << inventoryManager.get_total_profit() << "\n";
                } else {
                    cout << "Invalid admin credentials.\n";
                }
                break;

            case 4:
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
