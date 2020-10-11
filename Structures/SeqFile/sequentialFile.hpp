#include "Record.cpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <iomanip> 

class SequentialFile{
    private:
        string filename;
        string auxFilename;
        string dataFilename;

        void openFile();

        void writeRecord(Record record,string filename);
        
        void checkOpening(ifstream &mainFile, ifstream &auxFile);

        int numberOfRecords(string whichFile);

        void update(Record record);

        void reconstruction();

    public:
    
        SequentialFile();

        void insertAll(vector<Record> records);

        Record search(string key);

        vector<Record> search(string begin, string end);

        void scanAll();

        void add(Record record);

        /* ~SequentialFile(){
            remove("datos1.dat");
            remove("aux.dat");
        } */


};