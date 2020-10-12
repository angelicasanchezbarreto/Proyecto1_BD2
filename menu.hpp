#pragma once
#include "Structures/SeqFile/sequentialFile.cpp"
//#include "Structures/Hashing/mainHash.cpp"

struct Menu{
    Menu(){}
    void displayMenu();
    //void executeOperation();
    private:
    int operation;
    int structureNum;
    
    SequentialFile structure;
    //Hashing hashingFile;
    void setStructure(int structure);

    void Insertion();
    void Search();
    void Remove();

    void displayOperationsMenu();
};