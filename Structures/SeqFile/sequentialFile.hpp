#pragma once

#include "../Record/Record.cpp"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <iomanip> 


class SequentialFile{
    private:
        string filename;
        string auxFilename;
        string dataFilename;
        string indexFilename;
        pair<int,bool> header;
        vector<Record> records;

        void openFile();

        void writeRecord(Record record,string filename);
        
        void checkOpening(ifstream &mainFile, ifstream &auxFile);

        int numberOfRecords(string whichFile);

        void update(Record record);

        void reconstruction();

        void initData(Record record);

    public:
    
        SequentialFile();

        Record search(string key);

        vector<Record> search(string begin, string end);

        void scanAll();

        void add(Record record);

        /* ~SequentialFile(){
            remove("datos1.dat");
            remove("aux.dat");
        } */


};