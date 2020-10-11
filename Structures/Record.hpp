#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Record{
    public:
        char bodega[30]; //key
        char distrito[30]; 
        int aforo;
        int productos;
        float ventas1;
        float ventas2;
        float ventas3;
        float ventas4;
        bool inAux=false;
        int pointerTo;

        void print();

        Record();
        
        Record(string record);

        Record padding();

        void setPointer(int num);

        void setWhichFile(bool isOnAux);

        Record operator=(const Record &other);

        bool operator==(const Record &other);

        bool checkIfKeyEqual(const Record& record2);

        bool compareKey(string key);

        void setNewValues(Record &other, int pointerTo, bool whichFile);

        void find(ifstream &inFile, ifstream &inAuxFile, string key);
};