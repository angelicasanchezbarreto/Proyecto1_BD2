#include "sequentialFile.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <iomanip> 

bool compare(const Record& record1, const Record& record2){
    //string s1(record1.bodega),s2(record2.bodega);
    //return (s1 < s2);
    return record1.bodega<record2.bodega;
}

void SequentialFile::writeRecord(Record record,string filename){        
    ofstream outFile;    
    outFile.open(filename, ios::binary | ios::app);     
    if(!outFile)
        exit(EXIT_FAILURE);   
    outFile.write((char *) &record, sizeof(Record));        
    outFile.close();    
}
        
void SequentialFile::checkOpening(ifstream &mainFile, ifstream &auxFile){
    mainFile.open(filename, ios::binary|ios::in);  
    if(!mainFile)
        exit(EXIT_FAILURE);  
    auxFile.open(auxFilename, ios::binary|ios::in); 
}

int SequentialFile::numberOfRecords(string whichFile){
    ifstream inFile;
    inFile.open(whichFile, ios::binary|ios::in);  
    if(!inFile)
        return 0;  
    int begin = inFile.tellg();
    inFile.seekg (0, ios::end);
    int end = inFile.tellg();
    int num = (end-begin)/sizeof(Record);
    inFile.close();
    return num;
}

void SequentialFile::update(Record record){
    fstream file;
    fstream auxfile;
    file.open(filename,ios::ate|ios::in|ios::out|ios::binary);
    auxfile.open(auxFilename,ios::ate|ios::in|ios::out|ios::binary);

    Record nextRecord;   
    Record prevRecord; 
    file.seekg(0); 
    file.read((char *) &nextRecord, sizeof(nextRecord));
    prevRecord = nextRecord;
    while (!file.eof()) {
        if(!prevRecord.inAux){
            if (record.checkIfKeyEqual(nextRecord)){
                file.seekp(file.tellg() - streamoff(sizeof(Record)));
                file.write((char *)&record, sizeof(Record));
                break;
            }
            prevRecord = nextRecord;
            file.read((char *)&nextRecord, sizeof(Record));
        } else {
            if (record.checkIfKeyEqual(nextRecord)){
                auxfile.seekp(file.tellg() - streamoff(sizeof(Record)));
                auxfile.write((char *)&record, sizeof(Record));
                break;
            }
            prevRecord = nextRecord;
            auxfile.read((char *)&nextRecord, sizeof(Record));
        }
    }
    file.close();
} 

void SequentialFile::reconstruction(){
    ifstream inFile;
    ifstream inAuxFile;        
    checkOpening(inFile,inAuxFile);
    fstream newFile;
    newFile.open("newFile.dat",ios::binary|ios::out|ios::app);
    Record record;
    Record toInsert;
    int pos=1;
    inFile.read((char *) &record, sizeof(record));
    toInsert.setNewValues(record,pos,false);
    newFile.write((char *) &toInsert, sizeof(record)); 
    while(!inFile.eof()){
        pos++;
        if(!record.inAux){
            inFile.seekg(record.pointerTo*sizeof(Record));
            inFile.read((char *) &record, sizeof(record)); 
            toInsert.setNewValues(record,pos,false);
            newFile.write((char *) &toInsert, sizeof(record)); 
        } else {
            inAuxFile.seekg(record.pointerTo*sizeof(Record));
            inAuxFile.read((char *) &record, sizeof(record));
            toInsert.setNewValues(record,pos,false);
            newFile.write((char *) &toInsert, sizeof(record)); 
        }
    }
    this->filename = "newFile.dat";
    remove("aux.dat");
}

void SequentialFile::openFile(){
    fstream mainOut;
    ifstream dataIn;
    mainOut.open(filename, ios::binary|ios::out);  
    if(!mainOut)
        exit(EXIT_FAILURE);  
    dataIn.open(dataFilename); 
    if(!dataIn)
        exit(EXIT_FAILURE);

    string line;
    while(!dataIn.eof()){
        getline(dataIn,line);
        Record record(line);
        //mainOut.write((char*)&record, sizeof(Record));
        //initData(record);
        add(record);
        records.push_back(record);
    }

    mainOut.close();  
    dataIn.close();  
}

void SequentialFile::initData(Record record){
    fstream stream;
    stream.open(filename,ios::binary|ios::out|ios::in);
    if(numberOfRecords(filename)==0){
        record.setPointer(1);
        stream.write((char*)&record, sizeof(Record));
    }  else {
        Record nextRecord;   
        Record prevRecord;   
        stream.read((char *) &nextRecord, sizeof(Record));
        prevRecord = nextRecord;
        while (!stream.eof()) {
            if(compare(nextRecord,record)) {
                prevRecord = nextRecord;
                if(!nextRecord.inAux){
                    stream.seekg(nextRecord.pointerTo*sizeof(Record));
                    stream.read((char *) &nextRecord, sizeof(Record));    
                } /* else {
                    inAuxFile.seekg(nextRecord.pointerTo*sizeof(Record));
                    inAuxFile.read((char *) &nextRecord, sizeof(nextRecord));
                } */
            }
            else break;
        }
        if(prevRecord==nextRecord){
            record.setPointer(1);
            prevRecord.setPointer(numberOfRecords(auxFilename));
            update(prevRecord);                
        }
    
        record.setPointer(prevRecord.pointerTo);
        record.setWhichFile(prevRecord.inAux);
        if(!(prevRecord==nextRecord)){
            prevRecord.setWhichFile(true); //true if is at the aux file
            prevRecord.setPointer(numberOfRecords(auxFilename));
            update(prevRecord);                
        }
        writeRecord(record,auxFilename);
    }
    stream.close();
}

//PUBLIC

SequentialFile::SequentialFile(){
    this->filename = "data.dat";
    this->auxFilename = "aux.dat";
    this->dataFilename = "bodegas.txt";
    openFile();
}

Record SequentialFile::search(string key){
    ifstream inFile;
    ifstream inAuxFile;        
    checkOpening(inFile,inAuxFile);
    Record record;   
    record.find(inFile,inAuxFile,key); 
    inFile.close();  
    inAuxFile.close();  
    return record;
}

vector<Record> SequentialFile::search(string begin, string end){
    vector<Record> records;
    ifstream inFile;
    ifstream inAuxFile;        
    checkOpening(inFile,inAuxFile); 

    Record record;   
    record.find(inFile,inAuxFile,begin); 
    records.push_back(record);

    while (!(record.compareKey(end))) { 
        if(!record.inAux){
            inFile.seekg(record.pointerTo*sizeof(Record));
            inFile.read((char *) &record, sizeof(record));    
        } else {
            inAuxFile.seekg(record.pointerTo*sizeof(Record));
            inAuxFile.read((char *) &record, sizeof(record));
        }
        records.push_back(record);
    }
    inFile.close();  
    inAuxFile.close();  
    return records;
}

void SequentialFile::scanAll(){
    ifstream inFile;
    ifstream inAuxFile;
    checkOpening(inFile,inAuxFile);

    Record record;
    if(numberOfRecords(filename)>0){
        cout << "IN MAIN FILE:" << endl;
        while(inFile.read((char*) &record, sizeof(Record))){
            //inFile.seekg()
            cout << setw(10);
            record.print();
            cout << endl;
        }
    }
    if(numberOfRecords(auxFilename)>0){
        cout << "IN AUX FILE:" << endl;
        while(inAuxFile.read((char*) &record, sizeof(record))){
            cout << setw(10);
            record.print();
            cout << endl;
        }
    }
    inFile.close();
    inAuxFile.close();
}

void SequentialFile::add(Record record){
    fstream inFile;
    fstream inAuxFile;
    inFile.open(filename, ios::binary|ios::in|ios::out);  
    if(!inFile)
        exit(EXIT_FAILURE);  
    inAuxFile.open(auxFilename, ios::binary|ios::in|ios::out);  

    if(numberOfRecords(filename)==0){
        record.setPointer(1);
        inFile.write((char*)&record, sizeof(Record));
        this->header = make_pair(0,false);
    }
    else if(numberOfRecords(auxFilename)<5){
        Record nextRecord;   
        Record prevRecord; 
        if(!header.second)
            inFile.read((char *) &nextRecord, sizeof(Record));
        else
            inAuxFile.read((char *) &nextRecord, sizeof(Record));

        prevRecord = nextRecord;
        while (!inFile.eof()) {
            if(compare(nextRecord,record)) {
                prevRecord = nextRecord;
                if(!nextRecord.inAux){
                    inFile.seekg(nextRecord.pointerTo*sizeof(Record));
                    inFile.read((char *) &nextRecord, sizeof(Record));    
                } else {
                    inAuxFile.seekg(nextRecord.pointerTo*sizeof(Record));
                    inAuxFile.read((char *) &nextRecord, sizeof(nextRecord));
                }
            }
            else break;
        }
        if(prevRecord==nextRecord){
            record.setPointer(this->header.first);
            record.setWhichFile(this->header.second);
            this->header.first = numberOfRecords(auxFilename)-1;
            this->header.second=true;
            writeRecord(record,auxFilename);
            //this->header.first = inAuxFile.tellg(); //check later
            
        } else{
            record.setPointer(prevRecord.pointerTo);
            record.setWhichFile(prevRecord.inAux);
            writeRecord(record,auxFilename);
            prevRecord.setWhichFile(true); //true if is at the aux file
            prevRecord.setPointer(numberOfRecords(auxFilename));
            update(prevRecord);                
        }
    }

    if(numberOfRecords(auxFilename)==5){
        reconstruction();
    }

    inFile.close();
    inAuxFile.close();
}
