#include "menu.hpp"

void Menu::displayMenu(){
    cout << "WELCOME TO B0DEGA-LAND\n";
    cout << "Select the structure you want to work with: \n";
    cout << "1 Extendible Hashing\n";
    cout << "2 Sequential File\n";
    cout << "Insert the corresponding number: \n";
    cin >> structureNum;
    displayOperationsMenu();
}

void Menu::displayOperationsMenu(){
    cout << "Select the operation you want to  perform: \n";
    cout << "1. Insert\n";
    cout << "2. Search\n";
    cout << "3. Delete\n";
    cout << "Insert the corresponding number: \n";
    cin >> operation;
    switch (operation)
    {
    case 1:
        Insertion();
        break;
    case 2:
        Search();
        break;
    case 3:
        Remove();
        break;
    default:
        break;
    }
    //cout << "Insert the name of the record you want to " << getOperation() << endl;
}

void Menu::Insertion(){
    Record reg;
    cout << "Write the data to insert\n";
    cin >> reg.bodega >> reg.distrito >> reg.aforo >> reg.productos >> reg.ventas1 >> reg.ventas2 >> reg.ventas3 >> reg.ventas4;
    if(structureNum == 1)
        structure.add(reg);
    /*
    else
        hashing.add(reg);
        */
}

void Menu::Remove(){
    int pos;
    cout << "Write the position to eliminate\n";
    cin >> pos;
    /*
    if(structureNum == 1)
        structure.remove(pos);
    else
        hashing.remove(pos);
        */
}

void Menu::Search(){
    string key;
    cout << "Write the bodega to search\n";
    cin >> key;
    if(structureNum == 1)
        structure.search(key);
    /*
    else
        hashing.search(key);
    */
}

