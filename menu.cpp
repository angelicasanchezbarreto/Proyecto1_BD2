#include "menu.hpp"

void Menu::displayMenu(){
    cout << "WELCOME TO B0DEGA-LAND\n";
    cout << "Select the structure you want to work with: \n";
    cout << "1. Extendible Hashing\n";
    cout << "2. Sequential File\n";
    cout << "Insert the corresponding number: \n";
    cin >> structureNum;
    setStructure(structureNum);
    displayOperationsMenu();
}

void Menu::setStructure(int structure){
    switch (structure){
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }
}

void Menu::displayOperationsMenu(){
    cout << "Select the operation you want to  perform: \n";
    cout << "1. Insert\n";
    cout << "2. Search\n";
    cout << "3. Delete\n";
    cout << "Insert the corresponding number: \n";
    cin >> operation;
    cout << "Insert the name of the record you want to " << getOperation() << endl;
}

string Menu::getOperation(){
    string result;
    switch (operation){
    case 1: result = "insert"; break;
    case 2: result = "search"; break;
    case 3: result = "delete"; break;
    default: break;
    }
    return result;
}