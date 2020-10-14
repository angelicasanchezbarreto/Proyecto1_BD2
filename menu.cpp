#include "menu.hpp"

Menu::Menu(){
    this->operation=0;
    displayMenu();
}

void Menu::displayMenu(){
    cout << "BIENVENIDO A B0DEGA-LAND\n";
    cout << "Selecciona la estructura con la que quieres trabajar: \n";
    cout << "1. Extendible Hashing\n";
    cout << "2. Sequential File\n";
    cout << "Inserta el número correspondiente: \n";
    cin >> structureNum;
    cout << endl;
    displayOperationsMenu();
    if(this->operation!=5)
        displayMenu();
}


void Menu::displayOperationsMenu(){
    cout << "Selecciona la operación que quieres realizar: \n";
    cout << "1. Insertar registro\n";
    cout << "2. Buscar registro\n";
    cout << "3. Eliminar registro\n";
    cout << "4. Mostrar registros\n";
    cout << "5. Salir\n";
    cout << "Inserta el número correspondiente: \n";
    cin >> operation;
    cout << endl;
    setOperation();
}

string Menu::getOperationName(){
    string result;
    switch (operation){
    case 1: result = "insertar"; break;
    case 2: result = "buscar"; break;
    case 3: result = "eliminar"; break;
    default: break;
    }
    return result;
}

void Menu::setOperation(){
    vector<string> data;
    string temp;
    if(this->operation==2 || this->operation==3){
        cout << "Inserta el nombre de la bodega que deseas " << getOperationName() << endl;
        cin >> temp;
        executeOperation(temp);
    }
    else if(this->operation==1){
        cout << "Inserta el nombre de la bodega "; cin >> temp; data.push_back(temp);
        cout << "Inserta el nombre del distrito "; cin >> temp; data.push_back(temp);
        cout << "Inserta el aforo "; cin >> temp; data.push_back(temp);
        cout << "Inserta la cantidad de productos "; cin >> temp; data.push_back(temp);
        cout << "Inserta la cantidad de ventas en la semana 1 "; cin >> temp; data.push_back(temp);
        cout << "Inserta la cantidad de ventas en la semana 2 "; cin >> temp; data.push_back(temp);
        cout << "Inserta la cantidad de ventas en la semana 3 "; cin >> temp; data.push_back(temp);
        cout << "Inserta la cantidad de ventas en la semana 4 "; cin >> temp; data.push_back(temp);
        executeOperation(data);
    } else if(this->operation==4){
        cout << "BODEGA "; cout << setw(40);
        cout << "DISTRITO"; cout << setw(35);
        cout << "AFORO"; cout << setw(15);
        cout << "PRODUCTOS"; cout << setw(15);
        cout << "VENTAS1"; cout << setw(15);
        cout << "VENTAS2"; cout << setw(15);
        cout << "VENTAS3"; cout << setw(15);
        cout << "VENTAS4"; cout << setw(15);
        cout << endl;
        this->structureSeq.print();
    }
    cout << endl;
}

void Menu::executeOperation(string key){
    Record rec;
    if(this->operation==2){
        if(this->structureNum==2)
            rec = this->structureSeq.search(key);
        /* else
            rec = this->structureHash.search(key); */
        rec.print();
    
    } else if(this->operation==3)
        this->structureSeq.eliminate(key);
        cout << endl;
}

void Menu::executeOperation(vector<string> data){
    Record rec(data);
    if(this->structureNum==2)
        this->structureSeq.add(rec);
    /* else
        this->structureHash.add(rec); */
}