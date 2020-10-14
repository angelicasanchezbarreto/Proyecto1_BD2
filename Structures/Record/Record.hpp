#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

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
        
        Record(vector<string> data);

        Record(string record);

        Record operator=(const Record &other);

        bool compareKey(string key);

    protected:

        void padding(int size, char* variable);

        void setPointer(int num);

        void setWhichFile(bool isOnAux);

        bool operator==(const Record &other);

        bool checkIfKeyEqual(const Record& record2);

        void setNewValues(Record &other, int pointerTo, bool whichFile);

        void find(fstream &inFile, fstream &inAuxFile, string key, pair<int,bool> header);

        friend class SequentialFile;
};