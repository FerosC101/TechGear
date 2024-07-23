#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <openssl/sha.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>

using namespace std;
using namespace sql;

string hash_password(const string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);

    stringstream ss;
    for (auto byte : hash) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

class User {
public:
    string username;
    string password;
    double money;
    //Hinahanap niya nasaan si Cart at Purchase History
    Cart cart;
    vector<PurchaseRecord> purchase_history;
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
    if (file) {
        for (const auto& pair : users) {
            file << pair.second.username << " "
                 << pair.second.password << " "
                 << pair.second.money << "\n";
        }
    } else {
        cerr << "Error: Could not open user file for writing.\n";
    }
}

    void save_purchase_history(const string& username) {
    ofstream file(username + "history.txt", ios::trunc);
    if (file) {
        for (const auto& record : users[username].purchase_history) {
            file << record.item_name << " " << record.cost << " " << record.date << endl;
        }
        cout << "\t\t\t\tPurchase history saved successfully for user: " << username << endl;
    } else {
        cerr << "\t\t\t\tError: Could not open file for user: " << username << endl;
    }
}

    void load_purchase_history(const string& username) {
    ifstream file(username + "history.txt");
    if (file) {
        users[username].purchase_history.clear();
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string item_name, date;
            double cost;
            if (iss >> item_name >> cost >> date) {
                users[username].purchase_history.emplace_back(item_name, cost, date);
            }
        }
    } else {
        cout << "\t\t\t\tCould not open file for user: " << username << "\n";
    }
}
        
public:
    UserManager() {
        load_users();
        for (auto& user_pair : users) {
            load_purchase_history(user_pair.first);
        }
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
    newUser.password = hash_password(password);
    newUser.money = 0.0;

    users[username] = newUser;
    save_users();
    return true;
    }

    bool login_user(const string& username, const string& password) {
        if (users.find(username) == users.end()) {
        return false;
    }
    return users[username].password == hash_password(password);
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
        cout << "\t\t\t\t\t\t"<< username << "'s Cart:\n";
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

    void add_to_purchase_history(const string& username, const string& item_name, double cost, const string& date) {
        if (users.find(username) != users.end()) {
            cout << "\t\t\tAdding to purchase history: " << item_name << ", " << cost << ", " << date << endl;
            users[username].purchase_history.emplace_back(item_name, cost, date);
            save_purchase_history(username);
        } else {
            cerr << "\t\t\tError: User " << username << " not found." << endl;
        }
    }

    void view_purchase_history(const string& username) {
        if (users.find(username) == users.end()) {
            cout << "User not found.\n";
            return;
        }
        
        load_purchase_history(username);
        const auto& history = users.at(username).purchase_history;

        if (history.empty()) {
            cout << "\t\t\t" << username << " has no purchase history.\n";
            return;
        }

        cout << "\t\t\t" << username << "'s Purchase History:\n";
        for (const auto& record : history) {
            cout << "Item: " << record.item_name << ", Cost: Php" << record.cost << ", Date: " << record.date << "\n";
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
                    cout << "Item: " << record.item_name << ", Cost: Php" << record.cost << ", Date: " << record.date << "\n";
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
public:
    void view_inventory(Connection *conn) {
    try {
        unique_ptr<Statement> stmt(conn->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM your_table_name"));
        while (res->next()) {
            cout << "\t\t\tItem: " << res->getString("item_name") 
                 << ", Quantity: " << res->getInt("quantity") 
                 << ", Price: " << res->getDouble("price") << endl;
        }
    } catch (SQLException &e) {
        cerr << "\t\t\tSQL Error: " << e.what() << " (SQLSTATE: " << e.getSQLState() << ")\n";
        }
    }

    void add_item(Connection *conn, const string &item_name, int quantity, double price) {
        try {
            unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(
                "\t\t\tINSERT INTO your_table_name (item_name, quantity, price) VALUES (?, ?, ?)"));
            pstmt->setString(1, item_name);
            pstmt->setInt(2, quantity);
            pstmt->setDouble(3, price);
            pstmt->execute();
            cout << "\t\t\t\t\tItem added successfully." << endl;
        } catch (SQLException &e) {
            cerr << "\t\t\t\t\tError: " << e.what() << endl;
        }
    }

    void edit_price(Connection *conn, const string &item_name, double new_price) {
        try {
            unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(
                "\t\t\tUPDATE your_table_name SET price = ? WHERE item_name = ?"));
            pstmt->setDouble(1, new_price);
            pstmt->setString(2, item_name);
            pstmt->executeUpdate();
            cout << "\t\t\t\t\tPrice updated successfully." << endl;
        } catch (SQLException &e) {
            cerr << "\t\t\t\t\tError: " << e.what() << endl;
        }
    }

    void edit_quantity(Connection *conn, const string &item_name, int new_quantity) {
        try {
            unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(
                "\t\t\tUPDATE your_table_name SET quantity = ? WHERE item_name = ?"));
            pstmt->setInt(1, new_quantity);
            pstmt->setString(2, item_name);
            pstmt->executeUpdate();
            cout << "\t\t\t\t\tQuantity updated successfully." << endl;
        } catch (SQLException &e) {
            cerr << "\t\t\t\t\tError: " << e.what() << endl;
        }
    }

    void delete_item(Connection *conn, const string &item_name) {
        try {
            unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(
                "\t\t\tDELETE FROM your_table_name WHERE item_name = ?"));
            pstmt->setString(1, item_name);
            pstmt->execute();
            cout << "\t\t\t\t\tItem deleted successfully." << endl;
        } catch (SQLException &e) {
            cerr << "\t\t\t\t\tError: " << e.what() << endl;
        }
    }

    void calculate_total_profit(Connection *conn) {
        try {
            unique_ptr<Statement> stmt(conn->createStatement());
            unique_ptr<ResultSet> res(stmt->executeQuery(
                "\t\t\tSELECT SUM(price * quantity) AS total_profit FROM your_table_name"));
            if (res->next()) {
                cout << "\t\t\t\tTotal Profit: " << res->getDouble("total_profit") << endl;
            }
        } catch (SQLException &e) {
            cerr << "\t\t\t\t\tError: " << e.what() << endl;
        }
    }
};

class Techgear {
private:
    UserManager userManager;
    InventoryManager inventoryManager;

    void clear_screen() {
        cout << string(100, '\n');
    }

public:
    void admin_menu() {
        int choice;
        mysql::MySQL_Driver *driver;
        unique_ptr<Connection> conn(nullptr);

        while (true) {
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

            clear_screen();

            try {
                driver = mysql::get_mysql_driver_instance();
                conn.reset(driver->connect("tcp://your_host:3306", "your_username", "your_password"));
                conn->setSchema("your_database");

                switch (choice) {
                    case 1:
                        cout << "               ==============================================================================" << endl;
                        cout << "                                               I N V E N T O R Y" << endl;
                        cout << "               ==============================================================================" << endl << endl;
                        inventoryManager.view_inventory(conn.get());
                        break;
                    case 2: {
                        string item_name;
                        int quantity;
                        double price;
                        cout << "\t\t\tEnter item name: ";
                        cin.ignore();
                        getline(cin, item_name);
                        cout << "\t\t\tEnter quantity: ";
                        while (!(cin >> quantity) || quantity < 0) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "\t\t\t\tInvalid quantity. Please enter a positive integer: ";
                        }
                        cout << "\t\t\tEnter price: ";
                        while (!(cin >> price) || price < 0.0) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "\t\t\t\tInvalid price. Please enter a positive number: ";
                        }
                        inventoryManager.add_item(conn.get(), item_name, quantity, price);
                        break;
                    }
                    case 3: {
                        string item_name;
                        double new_price;
                        cout << "\t\t\tEnter item name: ";
                        cin.ignore();
                        getline(cin, item_name);
                        cout << "\t\t\tEnter new price: ";
                        while (!(cin >> new_price) || new_price < 0.0) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "\t\t\t\tInvalid price. Please enter a positive number: ";
                        }
                        inventoryManager.edit_price(conn.get(), item_name, new_price);
                        break;
                    }
                    case 4: {
                        string item_name;
                        int new_quantity;
                        cout << "\t\t\tEnter item name: ";
                        cin.ignore();
                        getline(cin, item_name);
                        cout << "\t\t\tEnter new quantity: ";
                        while (!(cin >> new_quantity) || new_quantity < 0) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "\t\t\t\t\tInvalid quantity. Please enter a positive integer: ";
                        }
                        inventoryManager.edit_quantity(conn.get(), item_name, new_quantity);
                        break;
                    }
                    case 5: {
                        string item_name;
                        cout << "\t\t\tEnter item name to delete: ";
                        cin.ignore();
                        getline(cin, item_name);
                        inventoryManager.delete_item(conn.get(), item_name);
                        break;
                    }
                    case 6:
                        inventoryManager.calculate_total_profit(conn.get());
                        break;
                    case 7:
                        return;
                    default:
                        cout << "\t\t\t\t\tInvalid choice. Please try again.\n";
                        break;
                }
            } catch (SQLException &e) {
                cerr << "\t\t\t\tSQL Error: " << e.what() << " (SQLSTATE: " << e.getSQLState() << ")\n";
            }
        }
    }
};
    void introduction() {
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

            clear_screen();

            switch (choice) {
                case 1:
                    cout << "               ==============================================================================" << endl;
                    cout << "                                          A D M I N   L O G - I N" << endl;
                    cout << "               ==============================================================================" << endl << endl;
                    cout << "\t\t\tEnter admin username\t\t: ";
                    cin >> username;
                    cout << "\t\t\tEnter admin password\t\t: ";
                    cin >> password;
                    // Ensure userManager is properly defined and initialized
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
                    // Ensure userManager is properly defined and initialized
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
                    // Ensure userManager is properly defined and initialized
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
