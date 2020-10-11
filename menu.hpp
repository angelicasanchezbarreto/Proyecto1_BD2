#include "Structures/SeqFile/sequentialFile.cpp"

struct Menu{
    void displayMenu();
    void executeOperation();
    private:
    int operation;
    int structureNum;
    SequentialFile structure;
    //Hashing hashingFile;
    void setStructure(int structure);

    template<typename T>
    void setOperation(T structure);
    string getOperation();
    void displayOperationsMenu();
};