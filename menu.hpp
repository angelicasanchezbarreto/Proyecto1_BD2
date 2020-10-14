#pragma once

#include "Structures/SeqFile/sequentialFile.cpp"

struct Menu{
    Menu();
    private:
    int operation;
    int structureNum;
    SequentialFile structureSeq;
    void displayMenu();
    void executeOperation(string key);
    void executeOperation(vector<string> data);
    void setOperation();
    string getOperationName();
    void displayOperationsMenu();
    
};