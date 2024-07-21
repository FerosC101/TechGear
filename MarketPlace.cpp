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

struct PurchaseRecord {
    string item_name;
    double cost;
    string date;

    PurchaseRecord(const string& item, double c, const string& d) : item_name(item), cost(c), date(d) {}
};
struct User {
    string username;
    string password;
    Cart cart;
    double money;
    vector<PurchaseRecord> purchase_history;

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

    void add_to_purchase_history(const string& username, const string& item_name, double cost, const string& date) {
        users[username].purchase_history.emplace_back(item_name, cost, date);
    }

    void search_item(const string& name, string& catagory);

    void view_purchase_history(const string& username) const {
        const auto& history = users.at(username).purchase_history;
        cout << "\t\t\t" << username << "'s Purchase History:\n";
        for (const auto& record : history) {
            cout << "Item: " << record.item_name << ", Cost: Php" << record.cost << ", Date: " << record.date << "\n";
        }
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
            cout << "\t\t\tName\t\t: " << item.name << endl;
            cout << "\t\t\tCategory\t: " << item.category << endl;
            for (const auto& spec : item.specs) {
                cout << spec.first << ": " << spec.second << endl;
            }
            cout << "\t\t\tPrice: $" << item.price << endl;
            cout << "\t\t\tQuantity: " << item.quantity << endl;
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

    bool delete_item(const string& item_name) {
        return inventory.erase(item_name) > 0;
    }

    Item* get_item(const string& item_name) {
        if (inventory.find(item_name) == inventory.end()) {
            return nullptr;
        }
        return &inventory[item_name];
    }

    double calculate_total_profit() const {
        double total = 0.0;
        for (const auto& pair : inventory) {
            const auto& item = pair.second;
            total += (item.price - item.cost_price) * (item.quantity);
        }
        return total;
    }

    void add_profit(double amount) {
        total_profit += amount;
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

    bool item_exists(const string& item_name) const {
        return inventory.find(item_name) != inventory.end();
    }

    void search_by_spec(const string& spec_key, const string& spec_value) {
        bool found = false;
        for (const auto& pair : inventory) {
            const auto& item = pair.second;
            if (item.specs.find(spec_key) != item.specs.end() && item.specs.at(spec_key) == spec_value) {
                cout << "\t\t\tName: " << item.name << endl;
                cout << "\t\t\tCategory: " << item.category << endl;
                for (const auto& spec : item.specs) {
                    cout << spec.first << ": " << spec.second << endl;
                }
                cout << "\t\t\tPrice: $" << item.price << endl;
                cout << "\t\t\tQuantity: " << item.quantity << endl;
                cout << "                           =====================================================" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "\t\t\t\t\tNo items found with " << spec_key << " = " << spec_value << endl;
        }
    }
};

class Techgear {
private:
    UserManager userManager;
    InventoryManager inventoryManager;

public:
    void admin_menu() {
        int choice;
        while(true) {
            cout << "               ==============================================================================" << endl;
            cout << "                                              A D M I N   M E N U" << endl;
			cout << "               ==============================================================================" << endl << endl;
            cout << "\t\t\t[1]\tView Inventory" << endl;
            cout << "\t\t\t[2]\tAdd Item" << endl;
            cout << "\t\t\t[3]\tEdit Price" << endl;
            cout << "\t\t\t[4]\tEdit Quantity" << endl;
            cout << "\t\t\t[5]\tDelete Item" << endl;
            cout << "\t\t\t[6]\tCalculate Total Profit" << endl;
            cout << "\t\t\t[7]\tExit\n" << endl;
			cout << "                           =====================================================" << endl;
			cout << "                                                Enter Mode: ";
            cin >> choice;
            system("cls");

            switch(choice) {
                case 1:
                    cout << "               ==============================================================================" << endl;
                    cout << "                                               I N V E N T O R Y" << endl;
			        cout << "               ==============================================================================" << endl << endl;
                    inventoryManager.view_inventory();
                    break;
                case 2: {
                    string name, category, spec_key, spec_value;
                    unordered_map<string, string> specs;
                    double price, cost_price;
                    int quantity;
                    cout << "               ==============================================================================" << endl;
                    cout << "                                                  A D D   I T E M S" << endl;
			        cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter item name\t\t\t\t: ";
                    cin >> name;
                    cout << "\t\t\tEnter category\t\t\t\t: ";
                    cin >> category;
                    cout << "\t\t\tEnter price\t\t\t\t: ";
                    cin >> price;
                    cout << "\t\t\tEnter quantity\t\t\t\t: ";
                    cin >> quantity;
                    cout << "\t\t\tEnter cost price\t\t\t\t: ";
                    cin >> cost_price;
                    cout << "\t\t\tEnter specs (key value pairs, 'done' to finish)\t:\n";
                    while (true) {
                        cin >> spec_key;
                        if (spec_key == "done") break;
                        cin >> spec_value;
                        specs[spec_key] = spec_value;
                    }

                    Item newItem(name, category, specs, price, quantity, cost_price);
                    if (inventoryManager.add_item(newItem)) {
                        cout << "\t\t\t\t\tItem added successfully.\n";
                    } else {
                        cout << "\t\t\t\t\tItem already exists.\n";
                    }
                    break;
                }
                case 3: {
                    string item_name;
                    double new_price;
                    cout << "               ==============================================================================" << endl;
                    cout << "                                                E D I T   P R I C E" << endl;
			        cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter item name\t\t: ";
                    cin >> item_name;
                    cout << "\t\t\tEnter new price\t\t: ";
                    cin >> new_price;
                    if (inventoryManager.edit_price(item_name, new_price)) {
                        cout << "\t\t\t\tPrice updated successfully.\n";
                    } else {
                        cout << "\t\t\t\t\tItem not found.\n";
                    }
                    break;
                }
                case 4: {
                    string item_name;
                    int new_quantity;
                    cout << "               ==============================================================================" << endl;
                        cout << "                                         E D I T   Q U A N T I T Y " << endl;
			        cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter item name\t\t: ";
                    cin >> item_name;
                    cout << "\t\t\tEnter new quantity\t\t: ";
                    cin >> new_quantity;
                    if (inventoryManager.edit_quantity(item_name, new_quantity)) {
                        cout << "\t\t\t\tQuantity updated successfully.\n";
                    } else {
                        cout << "\t\t\t\t\tItem not found.\n";
                    }
                    break;
                }
                case 5: {
                    string item_name;
                    cout << "Enter item name: ";
                    cin >> item_name;

                    if (inventoryManager.delete_item(item_name)) {
                        cout << "Item deleted successfully.\n";
                    } else {
                        cout << "Item not found.\n";
                    }
                    break;

                }
                case 6:
                    cout << "               ==============================================================================" << endl;
                    cout << "                                                P R O F I T" << endl;
			        cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tTotal profit: $" << inventoryManager.calculate_total_profit() << "\n";
                    break;
                case 7:
                    return;
                default:
                    cout << "\t\t\t\t\tInvalid choice. Please try again.\n";
            }
        }
    }

    

    void view_inventory_and_add_to_cart(const string& username) {
        while (true) {
            cout << "\nInventory: \n";
            inventoryManager.view_inventory();
            cout << "\nDo you want an item to the cart? (y/n): ";
            char add_to_cart_choice;
            cin >> add_to_cart_choice;

            if (add_to_cart_choice == 'y' || add_to_cart_choice == 'Y') {
                string item_name;
                cout << "Enter item name: ";
                cin >> item_name;

                if (inventoryManager.get_item(item_name)) {
                    userManager.add_to_cart(username, item_name);
                    cout << "Item added to Cart.\n";
                } else {
                    cout << "Item does not exist, Item not found.\n";
                }
                cout << "Do you want to purchase another item or go to the user menu? (p/u): ";
                char next_choice;
                cin >> next_choice;

                if (next_choice == 'u' || next_choice == 'U') {
                    break;
                }
            } else {
                break;
            }
        }
    }

    void view_cart_and_checkout(const string& username) {
        while (true) {
            cout << "\nYour Cart:\n";
            userManager.view_cart(username);
            double total_cost = userManager.get_cart(username).total_cost(inventoryManager.get_inventory());
            cout << "Total Cost: $" << total_cost << "\n";

            cout << "Do you want to checkout or return to the menu? (c/m): ";
            char checkout_choice;
            cin >> checkout_choice;

            if (checkout_choice == 'c' || checkout_choice == 'C') {
                double user_money = userManager.get_user_money(username);

                if (user_money >= total_cost) {
                    CartNode* current = userManager.get_cart_head(username);
                    while (current) {
                        Item* item = inventoryManager.get_item(current->item_name);
                        if (item) {
                            item->quantity--;
                            double profit = item->price - item->cost_price;
                            inventoryManager.add_profit(profit);
                        }
                        current = current->next;
                    }

                    userManager.deduct_money(username, total_cost);
                    userManager.clear_cart(username);

                    // PARA SA TIJME
                    time_t now = time(0);
                    char* dt = ctime(&now);

                    current = userManager.get_cart_head(username);
                    while (current) {
                        const Item* item = inventoryManager.get_item(current->item_name);
                        if (item) {
                            double item_cost = item->price;
                            userManager.add_to_purchase_history(username, item->name, item_cost, string(dt));
                        } 
                        current = current->next;
                    }
                    userManager.add_to_purchase_history(username, "Total cost: $" + to_string(total_cost), total_cost, string(dt));
                    userManager.clear_cart(username);
                    cout << "Checkout successful. Total cost: $" << total_cost << "\n";
                } else {
                    cout << "Insufficient funds. Please add more money to your account.\n";
                }
                break;
            } else {
                break;
            }
        }
    }

    void user_menu(const string& username) {
        int choice;
        while (true) {
            cout << "               ==============================================================================" << endl;
	        cout << "                                              U S E R   M E N U" << endl;
	        cout << "               ==============================================================================" << endl << endl;
            cout << "\t\t\t[1]\tShop Now" << endl;
	        cout << "\t\t\t[2]\tView Cart" << endl;
	        cout << "\t\t\t[3]\tSearch" << endl;
	        cout << "\t\t\t[4]\tAdd Money to Account" << endl;
            cout << "\t\t\t[5]\tAccount Details" << endl;
	        cout << "\t\t\t[6]\tExit\n" << endl;
	        cout << "                           =====================================================" << endl;
	        cout << "                                                Enter Choice: ";
            cin >> choice;
            system("cls");

            switch (choice) {
                case 1:
                    view_inventory_and_add_to_cart(username);
                    break;
                case 2: {
                    view_cart_and_checkout(username);
                    break;
                }
                case 3: {
                    string spec_key, spec_value;
                    cout << "Enter specification key: ";
                    cin >> spec_key;
                    cout << "Enter specification value: ";
                    cin >> spec_value;
                    inventoryManager.search_by_spec(spec_key, spec_value);
                    break;
                }
                case 4: {
                    double amount;
                    cout << "Enter amount to add: ";
                    cin >> amount;
                    userManager.add_money(username, amount);
                    cout << "$" << amount << " added to your account.\n";
                    break;
                }
                case 5: {
                    cout << "\n               ==============================================================================" << endl;
                    cout << "                                          A c c o u n t  D e t a i l s" << endl;
                    cout << "               ==============================================================================" << endl;
                    cout << "\n\t\t\tUsername: " << username << endl;
                    cout << "\t\t\tMoney: $" << userManager.get_user_money(username) << endl;
                    cout << "\t\t\tPurchase History:" << endl;
                    userManager.view_purchase_history(username);
                    while (true) {
                        char choice;
                        cout << "\t\t\tEnter M to return to Menu: ";
                        cin >> choice;
                        if (choice == 'm' || choice == 'M') {
                            user_menu(username);
                            system("cls");
                        }
                    }
                    break;
                }
                case 6: {
                    cout << "Logged out Successfully.\n";
                    main_menu();
                }
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    void introduction(){
	    cout << endl;
	    cout << "               ==============================================================================" << endl;
	    cout << "                                              T e c h G e a r" << endl;
	    cout << "               ==============================================================================" << endl;
	    cout << endl;
	    cout << "                         ====================             ==============         " << endl;
	    cout << "                        /====================           ================        " << endl;
	    cout << "                        ////////=====///////          /======//////=====       " << endl;
	    cout << "                               /=====                 /=====      /////             " << endl;
	    cout << "                               /=====                 /=====                    " << endl;
	    cout << "                               /=====                 /=====      =======       " << endl;
	    cout << "                               /=====                 /=====     /=======       " << endl;
	    cout << "                               /=====                 /======     //=====       " << endl;
	    cout << "                               /=====          ====    /================/   ==== " << endl;
	    cout << "                               /=====         /====     /==============/   /==== " << endl;
	    cout << "                               //////         ////       //////////////     ////" << endl;
	    cout << endl;
	    cout << "               ==============================================================================" << endl;
	    cout << "                           CREATED BY : FAJUTNAO  |  NUNEZ  |  PAULOS  |  VILLAR" << endl;
	    cout << "               ==============================================================================" << endl << endl << endl;
    }
    void main_menu() {
        int choice;
        string username, password;

        while (true) {
            cout << "                           =====================================================" << endl;
	        cout << "                                               [1] Admin Log-in " << endl;
	        cout << "                                               [2] User Log-in " << endl;
	        cout << "                                               [3] Register " << endl;
	        cout << "                                               [4] Exit " << endl;
	        cout << "                           =====================================================" << endl;
	        cout << endl << endl;
	        cout << "                                                Enter Mode: ";
            cin >> choice;
            system("cls");

            switch (choice) {
                case 1:
                    cout << "               ==============================================================================" << endl;
	                cout << "                                          A D M I N   L O G - I N" << endl;
	                cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter admin username\t\t: ";
                    cin >> username;
                    cout << "\t\t\tEnter admin password\t\t: ";
                    cin >> password;
                    if (userManager.admin_login(username, password)) {
                        admin_menu();
                    } else {
                        cout << "\t\t\t\t\tInvalid admin credentials.\n";
                    }
                    break;
                case 2:
                    cout << "               ==============================================================================" << endl;
	                cout << "                                          U S E R   L O G - I N" << endl;
	                cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter username\t\t: ";
                    cin >> username;
                    cout << "\t\t\tEnter password\t\t: ";
                    cin >> password;
                    if (userManager.login_user(username, password)) {
                        user_menu(username);
                    } else {
                        cout << "\t\t\t\t\tInvalid username or password.\n";
                    }
                    break;
                case 3:
                    cout << "               ==============================================================================" << endl;
	                cout << "                                                R E G I S T E R" << endl;
	                cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter username\t\t: ";
                    cin >> username;
                    cout << "\t\t\tEnter password\t\t: ";
                    cin >> password;
                    if (userManager.register_user(username, password)) {
                        cout << "\t\t\t\tRegistration successful. You can now log in.\n";
                    } else {
                        cout << "\t\t\t\tUsername already exists. Please choose a different username.\n";
                    }
                    break;
                case 4:
                    return;
                default:
                    cout << "\t\t\t\t\tInvalid choice. Please try again.\n";
            }
        }
    }

};

int main() {
    Techgear techgear;
    techgear.introduction();
    techgear.main_menu();
    return 0;
}