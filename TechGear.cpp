#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <set>
#include <limits>

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
        CartNode* newNode = new CartNode(item_name);
        newNode->next = head;
        head = newNode;
    }

    void view_cart() const {
        CartNode* current = head;
        while (current) {
            cout << "\t\t\t- " << current->item_name << "\n";
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

    void save_purchase_history(const string& username) {
        ofstream file(username + "history.txt",ios::trunc);
        if (file.is_open()) {
            for (const auto& record : users[username].purchase_history) {
                file << "\"" << record.item_name << "\" " << record.cost << " " << record.date << "\n";
            }
            file.close();
        } else {
            cerr << "\t\t\tError: Could not open file for user: " << username << endl;
        }
    }

    void load_purchase_history(const string& username) {
        ifstream file(username + "history.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string item_name;
                double cost;
                string date;

                getline(iss, item_name, '\"');
                getline(iss, item_name, '\"');
                iss >> cost;
                iss.ignore();
                getline(iss, date);

                users[username].purchase_history.emplace_back(item_name, cost, date);
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
        for (const auto& pair : users) {
            save_purchase_history(pair.first);
        }
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
        if (users[username].password == password) {
            return true;
        }
        return false;
    }

    bool admin_login(const string& username, const string& password) {
        return username == admin_username && password == admin_password;
    }

    bool add_to_cart(const string& username, const string& item) {
        if (users.find(username) == users.end()) {
            return false;
        }
        users[username].cart.add_item(item);
        return true;
    }

    void view_cart(const string& username) {
        cout <<"\t\t\t" << username << "'s Cart:\n";
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

    const unordered_map<string, User>& get_users() const {
        return users;
    }

    //AAYUSIN PA TONG TATLONG FUNCTION

    void add_to_purchase_history(const string& username, const string& item_name, double cost, const string& date) {
        if (users.find(username) != users.end()) {
            users[username].purchase_history.emplace_back(item_name, cost, date);
            save_purchase_history(username);
        } else {
            cerr << "\t\t\t\tError: User " << username << " not found." << endl;
        }
    }

    void view_purchase_history(const string& username) {
        if (users.find(username) == users.end()) {
            cout << "\t\t\t\t\tUser not found.\n";
            return;
        }

        users[username].purchase_history.clear();
        load_purchase_history(username);
        const auto& history = users.at(username).purchase_history;

        if (history.empty()) {
            cout << "\t\t\t" << username << " has no purchase history.\n";
            return;
        }

        double total_cost = 0.0;

        cout << "\t\t\t" << username << "'s Purchase History:\n";

        for (const auto& record : history) {
            cout << "\t\t\t- Item: " << record.item_name << ", Cost: Php" << record.cost << ", Date: " << record.date << "\n";
            total_cost += record.cost;
        }
    }

    void view_all_purchase_history() const {
        bool hasHistory = false;

        for (const auto& pair : users) {
            const string& username = pair.first;
            const auto& history = pair.second.purchase_history;

            if (!history.empty()) {
                cout << "\t\t\t" << username << "'s Purchase History: \n";
                for (const auto& record : history) {
                    cout << "\t\t\t\tItem: " << record.item_name << ", Cost: Php" << record.cost << ", Date: " << record.date << "\n";
                }
                hasHistory = true;
            }
        }

        if (!hasHistory) {
            cout << "\t\t\tNo purchase histories found for any user.\n";
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
            {"iPhone 15", {"iPhone 15", "Smartphone", {{"Brand", "Apple"}, {"Model", "iPhone 15"}, {"Storage", "256GB"}, {"Camera", "48MP + 12MP"}, {"Battery", "4000mAh"}, {"Display", "6.1-inch Super Retina XDR"}, {"Processor", "A16 Bionic"}, {"OS", "iOS 16"}, {"RAM", "6GB"}, {"Water Resistance", "IP68"}}, 49999.50, 15, 35000.0}},
            {"Galaxy S23", {"Galaxy S23", "Smartphone", {{"Brand", "Samsung"}, {"Model", "Galaxy S23"}, {"Storage", "256GB"}, {"Camera", "50MP + 12MP + 10MP"}, {"Battery", "4500mAh"}, {"Display", "6.2-inch Dynamic AMOLED 2X"}, {"Processor", "Exynos 2200/Snapdragon 8 Gen 1"}, {"OS", "Android 13"}, {"RAM", "8GB"}, {"Water Resistance", "IP68"}}, 44999.50, 20, 32500.0}},
            {"MacBook Pro", {"MacBook Pro", "Laptop", {{"Brand", "Apple"}, {"RAM", "16GB"}, {"Storage", "512GB SSD"}, {"Processor", "M1 Pro"}, {"Display", "14-inch Liquid Retina XDR"}, {"Battery Life", "Up to 17 hours"}, {"OS", "macOS"}, {"Weight", "3.5 lbs"}, {"Resolution", "3024 x 1964"}}, 99999.50, 10, 80000.0}},
            {"Surface Pro 9", {"Surface Pro 9", "Tablet", {{"Brand", "Microsoft"}, {"Model", "Surface Pro 9"}, {"Storage", "512GB SSD"}, {"RAM", "16GB"}, {"Display", "13-inch PixelSense"}, {"Battery Life", "Up to 15 hours"}, {"OS", "Windows 11"}, {"Weight", "1.96 lbs"}, {"Resolution", "2880 x 1920"}}, 64999.50, 8, 50000.0}},
            {"Sony WH-1000XM5", {"Sony WH-1000XM5", "Headphones", {{"Brand", "Sony"}, {"Model", "WH-1000XM5"}, {"Type", "Over-Ear"}, {"Battery Life", "30 hours"}, {"Noise Cancellation", "Yes"}, {"Connectivity", "Bluetooth 5.0"}, {"Weight", "254g"}, {"Frequency Response", "4Hz-40kHz"}}, 17499.50, 25, 12500.0}},
            {"Canon EOS R6", {"Canon EOS R6", "Camera", {{"Brand", "Canon"}, {"Model", "EOS R6"}, {"Resolution", "20MP"}, {"Video", "4K 60fps"}, {"Lens Mount", "RF Mount"}, {"ISO Range", "100-102400"}, {"Screen", "3.0-inch Vari-Angle Touchscreen"}, {"Connectivity", "Wi-Fi, Bluetooth"}}, 124999.50, 5, 100000.0}},
            {"PlayStation 5", {"PlayStation 5", "Gaming Console", {{"Brand", "Sony"}, {"Storage", "825GB SSD"}, {"Resolution", "4K UHD"}, {"Processor", "Custom AMD Ryzen Zen 2"}, {"Memory", "16GB GDDR6"}, {"Backward Compatibility", "Yes"}, {"Connectivity", "Wi-Fi, Bluetooth, USB, HDMI"}, {"Weight", "9.9 lbs"}}, 24999.50, 30, 20000.0}},
            {"Google Nest Wifi", {"Google Nest Wifi", "Router", {{"Brand", "Google"}, {"Model", "Nest Wifi"}, {"Speed", "2200Mbps"}, {"Coverage", "2200 sq. ft."}, {"Bands", "Dual-band"}, {"Security", "WPA3"}, {"Ports", "2 Gigabit Ethernet ports per unit"}, {"Weight", "380g"}}, 8499.50, 40, 6000.0}},
            {"LG OLED TV", {"LG OLED TV", "Television", {{"Brand", "LG"}, {"Size", "55 inches"}, {"Resolution", "4K"}, {"Display Technology", "OLED"}, {"Smart TV", "Yes"}, {"HDR", "Dolby Vision, HDR10"}, {"Refresh Rate", "120Hz"}, {"Ports", "4 HDMI, 3 USB"}, {"Weight", "18.9 kg"}}, 74999.50, 12, 60000.0}},
            {"Logitech MX Keys", {"Logitech MX Keys", "Keyboard", {{"Brand", "Logitech"}, {"Type", "Mechanical"}, {"Connectivity", "Wireless"}, {"Battery Life", "Up to 10 days"}, {"Backlight", "Yes"}, {"Compatibility", "Windows, macOS"}, {"Weight", "810g"}, {"Dimensions", "430.2 x 131.63 x 20.5 mm"}}, 4999.50, 50, 3500.0}},
            {"Razer DeathAdder V2", {"Razer DeathAdder V2", "Mouse", {{"Brand", "Razer"}, {"Type", "Gaming"}, {"DPI", "20000"}, {"Buttons", "8 Programmable"}, {"Connectivity", "Wired"}, {"Sensor", "Optical"}, {"Weight", "82g"}, {"Cable Length", "2.1m"}}, 3999.50, 60, 2500.0}},
            {"Samsung T7", {"Samsung T7", "External Hard Drive", {{"Brand", "Samsung"}, {"Capacity", "1TB"}, {"Type", "SSD"}, {"Interface", "USB 3.2 Gen 2"}, {"Read Speed", "1050MB/s"}, {"Write Speed", "1000MB/s"}, {"Weight", "58g"}, {"Dimensions", "85 x 57 x 8 mm"}}, 7999.50, 25, 6000.0}},
            {"HP LaserJet Pro", {"HP LaserJet Pro", "Printer", {{"Brand", "HP"}, {"Model", "LaserJet Pro"}, {"Type", "All-in-One"}, {"Print Speed", "28 ppm"}, {"Connectivity", "Wi-Fi, USB, Ethernet"}, {"Functions", "Print, Scan, Copy, Fax"}, {"Weight", "11.4 kg"}, {"Dimensions", "420 x 390 x 323 mm"}}, 14999.50, 18, 11000.0}},
            {"JBL Charge 5", {"JBL Charge 5", "Speakers", {{"Brand", "JBL"}, {"Type", "Portable"}, {"Battery Life", "20 hours"}, {"Waterproof", "IP67"}, {"Power", "30W"}, {"Connectivity", "Bluetooth"}, {"Weight", "960g"}, {"Dimensions", "223 x 96.5 x 94 mm"}}, 8999.50, 35, 6500.0}},
            {"Amazon Echo", {"Amazon Echo", "Smart Home Hub", {{"Brand", "Amazon"}, {"Model", "Echo"}, {"Voice Assistant", "Alexa"}, {"Connectivity", "Wi-Fi, Bluetooth"}, {"Audio", "360-degree sound"}, {"Smart Home Compatibility", "Yes"}, {"Weight", "780g"}, {"Dimensions", "148 x 99 x 99 mm"}}, 4999.50, 10, 3500.0}},
            {"Dell XPS 13", {"Dell XPS 13", "Laptop", {{"Brand", "Dell"}, {"RAM", "16GB"}, {"Storage", "512GB SSD"}, {"Processor", "Intel Core i7-1185G7"}, {"Display", "13.4-inch FHD+"}, {"Battery Life", "Up to 14 hours"}, {"OS", "Windows 11"}, {"Weight", "1.2 kg"}, {"Resolution", "1920 x 1200"}}, 74999.50, 10, 60000.0}},
            {"Apple Watch Series 8", {"Apple Watch Series 8", "Smartwatch", {{"Brand", "Apple"}, {"Model", "Series 8"}, {"Storage", "32GB"}, {"Display", "1.9-inch Retina"}, {"Battery Life", "Up to 18 hours"}, {"OS", "watchOS 9"}, {"Connectivity", "Wi-Fi, Bluetooth, GPS, Cellular"}, {"Water Resistance", "50m"}}, 19999.50, 15, 15000.0}},
            {"Samsung Galaxy Tab S8", {"Samsung Galaxy Tab S8", "Tablet", {{"Brand", "Samsung"}, {"Storage", "128GB"}, {"RAM", "8GB"}, {"Display", "11-inch TFT"}, {"Battery", "8000mAh"}, {"Processor", "Snapdragon 8 Gen 1"}, {"OS", "Android 12"}, {"Resolution", "2560 x 1600"}, {"Weight", "503g"}}, 34999.50, 20, 25000.0}},
            {"GoPro HERO10", {"GoPro HERO10", "Action Camera", {{"Brand", "GoPro"}, {"Resolution", "23MP"}, {"Video", "5.3K60, 4K120"}, {"Waterproof", "10m"}, {"Battery", "1720mAh"}, {"Connectivity", "Wi-Fi, Bluetooth"}, {"Weight", "153g"}, {"Dimensions", "71 x 55 x 33.6 mm"}}, 24999.50, 15, 17500.0}},
            {"Bose QuietComfort 45", {"Bose QuietComfort 45", "Headphones", {{"Brand", "Bose"}, {"Type", "Over-Ear"}, {"Battery Life", "24 hours"}, {"Noise Cancellation", "Yes"}, {"Connectivity", "Bluetooth 5.1, USB-C"}, {"Weight", "240g"}, {"Frequency Response", "20Hz-20kHz"}}, 16499.50, 20, 12500.0}}
        };
    }

    void view_inventory() {
        cout << "               ==============================================================================" << endl;
        cout << "                                               I N V E N T O R Y" << endl;
	cout << "               ==============================================================================" << endl << endl;
        for (const auto& pair : inventory) {
            const auto& item = pair.second;
            cout << "\t\t\tName\t\t\t: " << item.name << endl;
            cout << "\t\t\tCategory\t\t: " << item.category << endl;
            for (const auto& spec : item.specs) {
                cout << "\t\t\t"<< spec.first << "\t\t\t: " << spec.second << endl;
            }
            cout << "\t\t\tPrice\t\t\t: Php" << item.price << endl;
            cout << "\t\t\tQuantity\t\t: " << item.quantity << endl;
		cout << "               ==============================================================================" << endl;
        }
    }

    void update_inventory(const string& item_name, int quantity) {
        inventory[item_name].quantity -= quantity;
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
        double total_profit = 0.0;
        for (const auto& item : inventory) {
            double profit_per_item = item.second.price - item.second.cost_price;
            double total_item_profit = profit_per_item * (item.second.quantity - 10);
            total_profit += total_item_profit;
        }
        return total_profit;
    }

    void display_total_profit() const {
        double total_profit = calculate_total_profit();
        cout << "\t\t\tTotal Profit: Php" << total_profit << endl;
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
            cout << "\t\t\tName\t\t: " << item.name << endl;
            cout << "\t\t\tCategory\t: " << item.category << endl;
            for (const auto& spec : item.specs) {
                cout << "\t\t\t"<<spec.first << "\t: " << spec.second << endl;
            }
            cout << "\t\t\tPrice\t\t: Php" << item.price << endl;
            cout << "\t\t\tQuantity\t: " << item.quantity << endl;
            cout << "               ==============================================================================" << endl<< endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\t\t\t\t\tNo items found with " << spec_key << " = " << spec_value << endl;
    }
}

    void search_by_name(const string& name) {
        bool found = false;
        for (const auto& pair : inventory) {
            const auto& item = pair.second;
            if (item.name == name) {
                cout << "\t\t\tName\t: " << item.name << endl;
                cout << "\t\t\tCategory\t\t: " << item.category << endl;
                for (const auto& spec : item.specs) {
                    cout << "\t\t\t"<<spec.first << "\t: " << spec.second << endl;
                }
                cout << "\t\t\tPrice\t\t: Php" << item.price << endl;
                cout << "\t\t\tQuantity\t: " << item.quantity << endl;
                cout << "               ==============================================================================" << endl << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "\t\t\t\t\tNo items found with name = " << name << endl;
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
		cout << "               ==============================================================================" << endl;
		cout << "                                                Enter Mode: ";
            while (!(cin >> choice) || choice < 1 || choice > 7) {
                cout << "Invalid choice, please enter a valid option(1-7): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
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
                    cin.ignore();
                    cout << "\t\t\tEnter item name\t\t\t\t\t: ";
                    getline(cin, name);
                    cout << "\t\t\tEnter category\t\t\t\t\t: ";
                    cin >> category;
                    cout << "\t\t\tEnter price\t\t\t\t\t: ";
                    cin >> price;
                    cout << "\t\t\tEnter quantity\t\t\t\t\t: ";
                    cin >> quantity;
                    cout << "\t\t\tEnter cost price\t\t\t\t: ";
                    cin >> cost_price;
                    cout << "\t\t\tEnter specs (key value pairs, 'done' to finish. Double Enter for new specs entry:>)\t:\n";
                    while (true) {
                        cin.ignore();
                        cout << "\t\t\t\tEnter Specs Key\t\t\t\t: ";
                        getline(cin, spec_key);
                        if (spec_key == "done") break;
                        cout << "\t\t\t\tEnter Specs Value\t\t\t: ";
                        getline(cin, spec_value);
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
                    cin.ignore();
                    cout << "\t\t\tEnter item name\t\t: ";
                    getline(cin, item_name);
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
                    cin.ignore();
                    cout << "\t\t\tEnter item name\t\t: ";
                    getline(cin, item_name);
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
                    cout << "               ==============================================================================" << endl;
			cout << "                                         D E L E T E   I T E M " << endl;
			cout << "               ==============================================================================" << endl << endl;
                    cin.ignore();
                    cout << "\t\t\tEnter item name\t\t\t: ";
                    getline(cin, item_name);

                    if (inventoryManager.delete_item(item_name)) {
                        cout << "\t\t\t\t\tItem deleted successfully.\n";
                    } else {
                        cout << "\t\t\t\t\tItem not found.\n";
                    }
                    break;

                }
                case 6:
                    cout << "               ==============================================================================" << endl;
                    cout << "                                                P R O F I T" << endl;
			        cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tTotal profit: Php " << inventoryManager.calculate_total_profit() << "\n";
                    userManager.view_all_purchase_history();
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
            cout << "               ==============================================================================" << endl;
		cout << "                                               I N V E N T O R Y" << endl;
		cout << "               ==============================================================================" << endl << endl;
            inventoryManager.view_inventory();
            cout << "\n\t\t\tDo you want an item to the cart? (y/n)\t\t: ";
            char add_to_cart_choice;
            cin >> add_to_cart_choice;

            if (add_to_cart_choice == 'y' || add_to_cart_choice == 'Y') {
                cin.ignore();
                string item_name;
                cout << "\t\t\tEnter item name\t\t\t\t\t: ";
                getline(cin, item_name);

                if (inventoryManager.get_item(item_name)) {
                    userManager.add_to_cart(username, item_name);
                    cout << "\t\t\t\t\tItem added to Cart." << endl << endl;
                } else {
                    cout << "\t\t\t\tItem does not exist, Item not found." << endl << endl;
                }
                cout << "\t\t\tDo you want to purchase another item or go to the user menu? (p/u): ";
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
            cout << "               ==============================================================================" << endl;
                cout << "                                               C A R T" << endl;
		cout << "               ==============================================================================" << endl << endl;
            userManager.view_cart(username);
            double total_cost = userManager.get_cart(username).total_cost(inventoryManager.get_inventory());
            cout << "\t\t\tTotal Cost\t\t: Php" << total_cost << "\n";

            cout << "\t\t\tDo you want to checkout or return to the menu? (c/m)\t: ";
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

                            time_t now = time(0);
                            char* dt = ctime(&now);
                            string date(dt);
                            if (!date.empty() && date.back() == '\n') {
                                date.pop_back();
                            }

                            userManager.add_to_purchase_history(username, current->item_name, item->price, date);
                        }
                        current = current->next;
                    }

                    userManager.deduct_money(username, total_cost);
                    userManager.clear_cart(username);

                    time_t now = time(0);
                    char* dt = ctime(&now);
                    string date(dt);
                    if (!date.empty() && date.back() == '\n') {
                        date.pop_back(); 
                    }
                    cout << "\t\t\tCheckout successful. Total cost: Php" << total_cost << "\n";
                } else {
                    cout << "\t\t\tInsufficient funds. Please add more money to your account.\n";
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
	        cout << "               ==============================================================================" << endl;
	        cout << "                                                Enter Choice: ";
            while (!(cin >> choice) || choice < 1 || choice > 7) {
                cout << "\t\t\tInvalid choice, please enter a valid option(1-6): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
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
                int searchChoice;
                cout << "               ==============================================================================" << endl;
                cout << "                                               S E A R C H" << endl;
		cout << "               ==============================================================================" << endl << endl;
                cout << "\t\t\t[1]\t\tSearch by Name" << endl;
                cout << "\t\t\t[2]\t\tSearch by Specifications" << endl;
		cout << "               ==============================================================================" << endl;
                cout << "                                             Enter Choice: ";
                while (!(cin >> searchChoice) || searchChoice < 1 || searchChoice > 2) {
                    cout << "\t\t\t\tInvalid choice, please enter a valid option(1-2): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                if (searchChoice == 1) {
                    string name;
                    cin.ignore();
                    cout << "\t\t\tEnter the name of the item\t\t: ";
                    getline(cin, name);
                    inventoryManager.search_by_name(name);
                } 
		else if (searchChoice == 2) {
                    string spec_key, spec_value;
                    cin.ignore();
                    cout << "\t\t\tEnter specification key\t\t: ";
                    getline(cin, spec_key);
                    cout << "\t\t\tEnter specification value\t\t: ";
                    getline(cin, spec_value);
                    inventoryManager.search_by_spec(spec_key, spec_value);	
                } 
		else if (searchChoice == 3) {
			user_menu(username);
		}
		else {
                    cout << "\t\t\t\t\tInvalid choice. Please try again." << endl << endl;
                }
                break;
            }
                case 4: {
                    double amount;
			cout << "               ==============================================================================" << endl;
               		cout << "                                               D E P O S I T" << endl;
			cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter amount to add\t\t: ";
                    cin >> amount;
                    userManager.add_money(username, amount);
                    cout << endl << "\t\t\t\tPhp" << amount << " added to your account." << endl << endl;
                    break;
                }
                case 5: {
                    cout << "\n               ==============================================================================" << endl;
                    cout << "                                          A C C O U N T   D E T A I L S" << endl;
                    cout << "               ==============================================================================" << endl;
                    cout << "\n\t\t\tUsername\t\t\t: " << username << endl;
                    cout << "\t\t\tMoney\t\t\t\t: Php" << userManager.get_user_money(username) << endl;
                    cout << "\t\t\tPurchase History\t\t:" << endl;
                    userManager.view_purchase_history(username);
                    while (true) {
                        char choice;
                        cout << endl << "\t\t\tEnter M to return to Menu\t\t: ";
                        cin >> choice;
                        if (choice == 'm' || choice == 'M') {
                            user_menu(username);
                            system("cls");
                        }
                    }
                    break;
                }
                case 6: {
                    cout << "\t\t\t\t\tLogged out Successfully.\n";
                    main_menu();
			system("cls");
                }
                default:
                    cout << "\t\t\t\t\tInvalid choice. Please try again.\n";
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
            cout << "               ==============================================================================" << endl;
	        cout << "                                               [1] Admin Log-in " << endl;
	        cout << "                                               [2] User Log-in " << endl;
	        cout << "                                               [3] Register " << endl;
		cout << "                                               [4] Exit " << endl;
	     cout << "               ==============================================================================" << endl;
	        cout << endl << endl;
	        cout << "                                                Enter Mode: ";
            while (!(cin >> choice) || choice < 1 || choice > 4) {
                cout << "\t\t\t\tInvalid choice, please enter a valid option(1-4): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
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
                    exit(0);
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
