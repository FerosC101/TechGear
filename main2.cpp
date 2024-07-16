#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Item {
public:
    string name;
    double price;
    string category;
    string description;

    Item(string name, double price, string category, string description) 
        : name(name), price(price), category(category), description(description) {}
};

class User {
public:
    double balance;

    User(double balance = 1e9) : balance(balance) {} // Infinite balance muna

    bool purchase(Item& item) {
        if (balance >= item.price) {
            balance -= item.price;
            cout << "Purchased " << item.name << " for $" << item.price << ".\n";
            return true;
        } else {
            cout << "Insufficient balance to purchase " << item.name << ".\n";
            return false;
        }
    }
};

class Techgear {
private:
    vector<Item> laptops;
    vector<Item> phones;
    vector<Item> others;
    User user;

public:
    Techgear() {
        laptops.push_back(Item("BrickBook Air", 1000, "Laptops", "Specs: 8 lbs, 128KB RAM, 2-hour battery life \nPerfect for weightlifting enthusiasts. Multitasking? Not so much."));
        laptops.push_back(Item("InvisiBook Lite", 1200, "Laptops", "Specs: 0hz processor, 0B RAM, non-existent \nPerfect for minimalists who need zero functionality."));
        laptops.push_back(Item("TNT Notebook", 900, "Laptops", "Specs: 900MHz processor, 3.5GB RAM, 1kg Gunpowder \nIt goes boom, literally."));
        laptops.push_back(Item("Potato", 900, "Laptops", "Specs: 10Hz processor, 10B RAM, potato starch \nCan still run minecraft, probably."));
        laptops.push_back(Item("Jigoku no Pasokon", 666, "Laptops", "Specs: 10000THz processor, 16ZB RAM, an evil prescence that watches you  \nWill steal your soul "));

        
        phones.push_back(Item("iBroke", 1700, "Phones", "Specs: Shatter-prone screen, 2-hour battery, 0.5MP camera \nDesigned to break your heart and your wallet."));
        phones.push_back(Item("BlunderPhone", 800, "Phones", "Specs: Randomly shuts down, Auto-delete messages, 3-minute battery \nGuaranteed to ruin your important calls."));
        phones.push_back(Item("Phonehub", 600, "Phones", "Specs: Free subscriptions to ******** and ******** \nThe only phone you need"));
        phones.push_back(Item("Sentai Morpher", 900, "Phones", "Specs: Advanced/Ancient alien technology, Spandex suit included \nIts morphin time"));
        phones.push_back(Item("Digivice Smartphone", 1000, "Phones", "Specs: 1PB storage, 1TB RAM, Random Digimon Partner \nWhat will you do tomorrow?"));
        phones.push_back(Item("Selpon ni Anjo", 0, "Phones", "Specs: Missing storage, Missing Ram, Empty sim tray \nGone Forever"));
        
        others.push_back(Item("ZapMouse", 300, "Others", "Specs: Shock feedback, Random disconnects, No scroll wheel \nEvery click is a shocking experience."));
        others.push_back(Item("MisType Keyboard", 200, "Others", "Specs: Stuck keys, Random letters, Loud clacking sound \n Misspell with every stroke."));
        others.push_back(Item("LagCharger", 150, "Others", "Specs: Slow charging, Overheats, Random disconnections \nCharge your phone... eventually."));
        others.push_back(Item("Eardrum Destroyer", 600, "Others", "Specs: 100dBA Sound, Extremely loud bass \nWill make you go deaf"));
        others.push_back(Item("Always-On Webcam", 250, "Others", "Specs: 100% of the time streaming, Zero privacy settings, High-res awkward moments \nGreat for those who want their life to be a reality show—forever!"));
        others.push_back(Item("Henshin Belt", 100, "Others", "Specs: Includes a random gimmick, Does a jingle everytime you use it \nHENSHIN!"));
       
    }

    void displayItems(const vector<Item>& items) {
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". " << items[i].name << " - $" << items[i].price << "\n";
            cout << items[i].description << "\n";
        }
    }

    void buyItem(const vector<Item>& items) {
        int choice;
        cout << "Enter the item number you wish to buy: ";
        cin >> choice;

        if (choice > 0 && choice <= items.size()) {
            user.purchase(const_cast<Item&>(items[choice - 1]));
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    void displayMenu() {
        cout << "Categories:\n";
        cout << "1. Laptops\n";
        cout << "2. Phones\n";
        cout << "3. Others\n";
        cout << "Enter the category number to view items: ";
    }

    void run() {
        while (true) {
            displayMenu();
            int categoryChoice;
            cin >> categoryChoice;

            switch (categoryChoice) {
                case 1:
                    displayItems(laptops);
                    buyItem(laptops);
                    break;
                case 2:
                    displayItems(phones);
                    buyItem(phones);
                    break;
                case 3:
                    displayItems(others);
                    buyItem(others);
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }

            char cont;
            cout << "Do you want to buy another item? (y/n): ";
            cin >> cont;
            if (cont == 'n' || cont == 'N') {
                break;
            }
        }
    }
};

int main() {
    Techgear app;
    app.run();
    return 0;
}
