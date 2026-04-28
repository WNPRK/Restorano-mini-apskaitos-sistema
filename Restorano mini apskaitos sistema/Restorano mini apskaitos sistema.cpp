#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Struktūra patiekalui aprašyti
struct menuItemType {
    string menuItem;
    double menuPrice;
};

// Funkcijų prototipai
void getData(menuItemType menuList[], int& count);
void showMenu(menuItemType menuList[], int count);
void printCheck(menuItemType menuList[], const vector<int>& selectedItems, const vector<int>& quantities);

int main() {
    menuItemType menuList[50]; // Masyvas meniu saugojimui
    int menuSize = 0;
    vector<int> selectedItems; // Vartotojo pasirinktų patiekalų indeksai
    vector<int> quantities;    // Kiekvieno pasirinkto patiekalo kiekis

    // 1. Nuskaitome duomenis
    getData(menuList, menuSize);

    // 2. Rodome meniu
    showMenu(menuList, menuSize);

    // 3. Užsakymo procesas
    int choice, qty;
    cout << "\nĮveskite patiekalo numerį (įveskite 0, jei norite baigti): " << endl;

    while (cin >> choice && choice != 0) {
        if (choice > 0 && choice <= menuSize) {
            cout << "Kiek porcijų pageidaujate? ";
            cin >> qty;
            selectedItems.push_back(choice - 1);
            quantities.push_back(qty);
            cout << "Pridėta! Įveskite kitą numerį arba 0 baigti: ";
        }
        else {
            cout << "Neteisingas pasirinkimas, bandykite dar kartą: ";
        }
    }

    // 4. Sąskaitos generavimas
    if (!selectedItems.empty()) {
        printCheck(menuList, selectedItems, quantities);
    }
    else {
        cout << "Užsakymas tuščias." << endl;
    }

    return 0;
}

// Funkcija: nuskaito meniu iš failo
void getData(menuItemType menuList[], int& count) {
    ifstream file("menu.txt");
    if (!file) {
        cerr << "Klaida: Nepavyko rasti menu.txt failo!" << endl;
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
    cout << "--- Sveiki atvykę į restoraną „Gera Nuotaika“ ---" << endl;
    cout << fixed << setprecision(2);
    for (int i = 0; i < count; i++) {
        // Pakeičiame apatinius brūkšnius tarpais gražesniam vaizdui
        string name = menuList[i].menuItem;
        for (int j = 0; j < name.length(); j++) if (name[j] == '_') name[j] = ' ';

        cout << i + 1 << ". " << left << setw(40) << name << menuList[i].menuPrice << " €" << endl;
    }
    cout << "-----------------------------------------------" << endl;
}

// Funkcija: spausdina sąskaitą ir išsaugo į failą
void printCheck(menuItemType menuList[], const vector<int>& selectedItems, const vector<int>& quantities) {
    ofstream outFile("receipt.txt");
    double subtotal = 0;

    auto printFormat = [&](ostream& out) {
        out << "\n--- Jūsų sąskaita ---" << endl;
        for (size_t i = 0; i < selectedItems.size(); i++) {
            int idx = selectedItems[i];
            string name = menuList[idx].menuItem;
            for (int j = 0; j < name.length(); j++) if (name[j] == '_') name[j] = ' ';

            double itemTotal = menuList[idx].menuPrice * quantities[i];
            subtotal += itemTotal;

            out << quantities[i] << " x " << left << setw(35) << name
                << menuList[idx].menuPrice * quantities[i] << "€" << endl;
        }

        double tax = subtotal * 0.21;
        double total = subtotal + tax;

        out << fixed << setprecision(2);
        out << "\n-----------------------------------------------" << endl;
        out << left << setw(40) << "Mokesčiai (21%)" << tax << "€" << endl;
        out << left << setw(40) << "Galutinė suma" << total << "€" << endl;
        out << "Ačiū, kad lankėtės!" << endl;
        };

    printFormat(cout);    // Išveda į ekraną
    printFormat(outFile); // Įrašo į failą

    outFile.close();
    cout << "\nSąskaita sėkmingai išsaugota receipt.txt faile." << endl;
}