#include "Structures/SeqFile/sequentialFile.cpp"

int main(){

    SequentialFile sequentialFile("datos1.dat");
    vector<Record> datos;
    datos.push_back(Record{"0001","Pepito","Derecho",5}.padding());
    datos.push_back(Record{"0002","Pedrito","Medicina",3}.padding());
    datos.push_back(Record{"0003","Josefina","Negocios",9}.padding());
    datos.push_back(Record{"0004","Anastasia","Ingenieria",2}.padding());
    datos.push_back(Record{"0005","Carlitos","Administracion",8}.padding());
    datos.push_back(Record{"0006","Manolito","Psicologia",1}.padding());
    datos.push_back(Record{"0007","Jaimito","Educacion",4}.padding());
    datos.push_back(Record{"0008","Sandrita","Medicina",2}.padding());

    cout << "AFTER INSERTING FROM VECTOR:" << endl << endl;;
    sequentialFile.insertAll(datos);
    sequentialFile.scanAll();
    cout << endl;

    cout << "SEARCH TEST:" << endl << endl;
    Record search1 = sequentialFile.search("Pepito");
    Record search2 = sequentialFile.search("Josefina");
    Record search3 = sequentialFile.search("Anastasia");
    cout << setw(10);
    search1.print();
    cout << endl;
    cout << setw(10);
    search2.print();
    cout << endl;
    cout << setw(10);
    search3.print();
    cout << endl << endl;

    cout << "SEARCH TEST IN RANGE 1:" << endl << endl;
    vector<Record> range1 = sequentialFile.search("Carlitos","Pedrito");
    for(auto &i:range1){
        cout << setw(10);
        i.print();
        cout << endl;
    }
    cout << endl;

    cout << "SEARCH TEST IN RANGE 2:" << endl << endl;
    vector<Record> range2 = sequentialFile.search("Pepito","Sandrita");
    for(auto &i:range2){
        cout << setw(10);
        i.print();
        cout << endl;
    }
    cout << endl;

    cout << "ADD TEST 1:" << endl << endl;;
    sequentialFile.add(Record{"0005","Jesus","Filosofia",7}.padding());
    sequentialFile.scanAll();
    cout << endl;

    cout << "ADD TEST 2:" << endl << endl;;
    sequentialFile.add(Record{"0008","Alejo","Artes",10}.padding());
    sequentialFile.scanAll();
    cout << endl;

    cout << "ADD TEST 3:" << endl << endl;;
    sequentialFile.add(Record{"0008","Jeronimo","Contabilidad",2}.padding());
    sequentialFile.scanAll();
    cout << endl;

    cout << "ADD TEST 4:" << endl << endl;;
    sequentialFile.add(Record{"0008","Omar","Contabilidad",2}.padding());
    sequentialFile.scanAll();
    cout << endl;

    cout << "ADD TEST 5:" << endl << endl;;
    sequentialFile.add(Record{"0008","Roberta","Contabilidad",2}.padding());
    sequentialFile.scanAll();
    cout << endl;

    cout << "SEARCH 1 AFTER ADD TEST:" << endl << endl;;
    Record search4 = sequentialFile.search("Jeronimo"); 
    cout << setw(10);
    search4.print();
    cout << endl << endl;

    cout << "SEARCH 2 AFTER ADD TEST:" << endl << endl;;
    Record search5 = sequentialFile.search("Omar"); 
    cout << setw(10);
    search5.print();
    cout << endl;

    return 0;
}