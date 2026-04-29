#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Struktura patiekalui aprasyti
struct menuItemType {
    string menuItem;
    double menuPrice;
};

// Funkciju prototipai
void getData(menuItemType menuList[], int& count);
void showMenu(menuItemType menuList[], int count);
void printCheck(menuItemType menuList[], const vector<int>& selectedItems, const vector<int>& quantities);

int main() {
    menuItemType menuList[50];
    int menuSize = 0;
    vector<int> selectedItems;
    vector<int> quantities;

    // 1. Nuskaitome duomenis
    getData(menuList, menuSize);

    if (menuSize == 0) {
        cout << "Klaida: Meniu tuscias arba failas nerastas!" << endl;
        return 1;
    }

    // 2. Rodome meniu
    showMenu(menuList, menuSize);

    // 3. Uzsakymo procesas
    int choice, qty;
    cout << "\nIveskite patiekalo numeri (iveskite 0, jei norite baigti): " << endl;

    while (cin >> choice && choice != 0) {
        if (choice > 0 && choice <= menuSize) {
            cout << "Kiek porciju pageidaujate? ";
            cin >> qty;
            selectedItems.push_back(choice - 1);
            quantities.push_back(qty);
            cout << "Prideta! Iveskite kita numeri arba 0 baigti: ";
        }
        else {
            cout << "Neteisingas pasirinkimas, bandykite dar karta: ";
        }
    }

    // 4. Saskaitos generavimas
    if (!selectedItems.empty()) {
        printCheck(menuList, selectedItems, quantities);
    }
    else {
        cout << "Uzsakymas tuscias." << endl;
    }

    return 0;
}

// Funkcija: nuskaito meniu is failo
void getData(menuItemType menuList[], int& count) {
    ifstream file("menu.txt");
    if (!file) {
        // Jei vis tiek neras, pabandyk cia irasyti pilna kelia, pvz:
        // ifstream file("C:\\Users\\Vardas\\source\\repos\\Projektas\\menu.txt");
        return;
    }

    count = 0;
    while (file >> menuList[count].menuItem >> menuList[count].menuPrice) {
        count++;
    }
    file.close();
}

// Funkcija: parodo meniu vartotojui
void showMenu(menuItemType menuList[], int count) {
    cout << "--- Sveiki atvyke i restorana ---" << endl;
    cout << fixed << setprecision(2);
    for (int i = 0; i < count; i++) {
        string name = menuList[i].menuItem;
        for (int j = 0; j < name.length(); j++) if (name[j] == '_') name[j] = ' ';

        cout << i + 1 << ". " << left << setw(40) << name << menuList[i].menuPrice << " EUR" << endl;
    }
    cout << "-----------------------------------------------" << endl;
}

// Funkcija: spausdina saskaita ir issaugo i faila
void printCheck(menuItemType menuList[], const vector<int>& selectedItems, const vector<int>& quantities) {
    ofstream outFile("receipt.txt");
    double subtotal = 0;

    auto printFormat = [&](ostream& out) {
        out << "\n--- Jusu saskaita ---" << endl;
        for (size_t i = 0; i < selectedItems.size(); i++) {
            int idx = selectedItems[i];
            string name = menuList[idx].menuItem;
            for (int j = 0; j < name.length(); j++) if (name[j] == '_') name[j] = ' ';

            double itemTotal = menuList[idx].menuPrice * quantities[i];
            subtotal += itemTotal;

            out << quantities[i] << " x " << left << setw(35) << name
                << itemTotal << " EUR" << endl;
        }

        double tax = subtotal * 0.21;
        double total = subtotal + tax;

        out << fixed << setprecision(2);
        out << "\n-----------------------------------------------" << endl;
        out << left << setw(40) << "Mokesciai (21%)" << tax << " EUR" << endl;
        out << left << setw(40) << "Galutine suma" << total << " EUR" << endl;
        out << "Geros dienos!" << endl;
        };

    printFormat(cout);
    printFormat(outFile);

    outFile.close();
    cout << "\nSaskaita sekmingai issaugota receipt.txt faile." << endl;
}
//Programos pabaiga