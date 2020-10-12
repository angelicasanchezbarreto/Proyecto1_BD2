#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>

using namespace std;

class Record{
    public:
        char bodega[40]; //key
        char distrito[40]; 
        int aforo;
        int productos;
        float ventas1;
        float ventas2;
        float ventas3;
        float ventas4;
        bool inAux=false;
        int pointerTo;

        void print();

        Record(){}
        
        Record(string record);

        void padding(int size, char* variable);

        void setPointer(int num);

        void setWhichFile(bool isOnAux);

        Record operator=(const Record &other);

        bool operator==(const Record &other);

        bool checkIfKeyEqual(const Record& record2);

        bool compareKey(string key);

        void setNewValues(Record &other, int pointerTo, bool whichFile);

        void find(ifstream &inFile, ifstream &inAuxFile, string key, pair<int,bool> header);
};