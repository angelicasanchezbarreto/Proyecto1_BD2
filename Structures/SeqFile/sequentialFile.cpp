#include "sequentialFile.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <iomanip> 

bool compare(const Record& record1, const Record& record2){
    string s1(record1.bodega),s2(record2.bodega);
    return (s1 < s2);
}

void SequentialFile::writeRecord(Record record,string filename){        
    ofstream outFile;    
    outFile.open(filename, ios::binary | ios::app);     
    if(!outFile)
        exit(EXIT_FAILURE);   
    outFile.write((char *) &record, sizeof(Record));        
    outFile.close();    
}
        
void SequentialFile::checkOpeningIn(fstream &mainFile, fstream &auxFile){
    mainFile.open(filename, ios::binary|ios::in);  
    if(!mainFile)
        exit(EXIT_FAILURE);  
    auxFile.open(auxFilename, ios::binary|ios::in); 
    if(!auxFile)
        exit(EXIT_FAILURE); 
}

void SequentialFile::checkOpeningInOut(fstream &mainFile, fstream &auxFile){
    mainFile.open(filename, ios::binary|ios::out|ios::in);  
    if(!mainFile)
        exit(EXIT_FAILURE);  
    auxFile.open(auxFilename, ios::binary|ios::out|ios::in); 
    if(!mainFile)
        exit(EXIT_FAILURE); 
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

Record SequentialFile::getFirstRecord(fstream& file, fstream& auxfile){
    Record record;
    if(!header.second){
        file.seekg(header.first*sizeof(Record));
        file.read((char *) &record, sizeof(Record));
    } else{
        auxfile.seekg(header.first*sizeof(Record));
        auxfile.read((char *) &record, sizeof(Record));
    }
    return record;
}

void SequentialFile::update(Record record){
    int cont = this->numOfRecords;
    fstream file;
    fstream auxfile;
    checkOpeningInOut(file,auxfile);

    Record prevRecord; 
    Record nextRecord = getFirstRecord(file,auxfile);  

    prevRecord = nextRecord;
    bool changed=false;
    while (changed==false) {
        if(!prevRecord.inAux){
            if (record.checkIfKeyEqual(nextRecord)){
                file.seekp(file.tellg() - streamoff(sizeof(Record)));
                file.write((char *)&record, sizeof(Record));
                changed=true;
                break;
            }
            prevRecord = nextRecord;
            file.seekg(nextRecord.pointerTo*sizeof(Record));
            file.read((char *)&nextRecord, sizeof(Record));
        } else {
            if (record.checkIfKeyEqual(nextRecord)){
                auxfile.seekp(auxfile.tellg() - streamoff(sizeof(Record)));
                auxfile.write((char *)&record, sizeof(Record));
                changed=true;
                break;
            }
            prevRecord = nextRecord;
            auxfile.seekg(nextRecord.pointerTo*sizeof(Record));
            auxfile.read((char *)&nextRecord, sizeof(Record));
        }
    }
    file.close();
} 

void SequentialFile::reconstruction(){
    int cont = this->numOfRecords;
    fstream inFile;
    fstream inAuxFile;        
    checkOpeningIn(inFile,inAuxFile);

    fstream newFile;
    newFile.open("newFile.dat",ios::binary|ios::out|ios::app);
    Record record = getFirstRecord(inFile,inAuxFile);
    Record toInsert;
    int pos=1;

    this->header={0,false};
    toInsert.setNewValues(record,pos,false);
    newFile.write((char *) &toInsert, sizeof(record)); 
    while(cont--){
        pos++;
        if(cont==0)
            pos=-1;
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
    inFile.clear();
    inAuxFile.clear();
    remove("data.dat");
    remove("aux.dat");
    rename("newFile.dat","data.dat");
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
    while(getline(dataIn,line)){
        Record record(line);
        add(record);
        records.push_back(record);
    }

    mainOut.close();  
    dataIn.close();  
}


//PUBLIC

SequentialFile::SequentialFile(){
    this->filename = "data.dat";
    this->auxFilename = "aux.dat";
    this->dataFilename = "bodegas2.txt";
    openFile();
}

Record SequentialFile::search(string key){
    fstream inFile;
    fstream inAuxFile;        
    checkOpeningIn(inFile,inAuxFile);
    Record record;   
    record.find(inFile,inAuxFile,key,this->header); 
    inFile.close();  
    inAuxFile.close();  
    return record;
}

void SequentialFile::scanAll(){
    fstream inFile;
    fstream inAuxFile;
    checkOpeningIn(inFile,inAuxFile);

    Record record;
    if(numberOfRecords(filename)>0){
        cout << "IN MAIN FILE:" << endl;
        while(inFile.read((char*) &record, sizeof(Record))){
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

void SequentialFile::print(){
    Record temp;
    fstream inFile;
    fstream inAuxFile;
    checkOpeningIn(inFile,inAuxFile);
    temp = getFirstRecord(inFile, inAuxFile);
    while (temp.pointerTo!=-1){
        temp.print(); cout << endl;
        if(!temp.inAux){
            inFile.seekg(temp.pointerTo*sizeof(Record));
            inFile.read((char *) &temp, sizeof(Record));    
        } else {
            inAuxFile.seekg(temp.pointerTo*sizeof(Record));
            inAuxFile.read((char *) &temp, sizeof(Record));
        }
    }

}

void SequentialFile::add(Record record){
    int cont = this->numOfRecords;
    fstream inFile;
    fstream inAuxFile;
    checkOpeningInOut(inFile,inAuxFile); 

    if(numberOfRecords(filename)==0){
        record.setPointer(-1);
        inFile.write((char*)&record, sizeof(Record));
        this->header = make_pair(0,false);
    }
    else if(numberOfRecords(auxFilename)<5){
        Record prevRecord;
        Record nextRecord = getFirstRecord(inFile,inAuxFile); 

        prevRecord = nextRecord;
        while (cont--) {
            if(compare(nextRecord,record)) {
                prevRecord = nextRecord;
                if(nextRecord.pointerTo==-1)
                    break;
                if(!nextRecord.inAux){
                    inFile.seekg(nextRecord.pointerTo*sizeof(Record));
                    inFile.read((char *) &nextRecord, sizeof(Record));    
                } else {
                    inAuxFile.seekg(nextRecord.pointerTo*sizeof(Record));
                    inAuxFile.read((char *) &nextRecord, sizeof(Record));
                }
            }
            else break;
        }
        if(prevRecord==nextRecord){
            if(!compare(prevRecord,record)){
                record.setPointer(this->header.first);
                record.setWhichFile(this->header.second);
                this->header.first = numberOfRecords(auxFilename);
                this->header.second=true;
                prevRecord.setPointer(this->header.first);
                prevRecord.setWhichFile(this->header.second);
                writeRecord(record,auxFilename);
            } else {
                record.setPointer(prevRecord.pointerTo);
                record.setWhichFile(true);
                prevRecord.setPointer(numberOfRecords(auxFilename));
                prevRecord.setWhichFile(true);
                writeRecord(record,auxFilename);
                update(prevRecord);
            }
            
        } else{
            record.setPointer(prevRecord.pointerTo);
            record.setWhichFile(prevRecord.inAux);
            prevRecord.setWhichFile(true); //true if is at the aux file
            prevRecord.setPointer(numberOfRecords(auxFilename));
            writeRecord(record,auxFilename);
            update(prevRecord);                
        }
    }

    if(numberOfRecords(auxFilename)==5){
        reconstruction();
    }

    this->numOfRecords++;

    inFile.close();
    inAuxFile.close();
}

void SequentialFile::eliminate(string key){
    bool found=false;
    int cont = this->numOfRecords;
    fstream inFile;
    fstream inAuxFile;
    inFile.open(filename,ios::ate|ios::in|ios::out|ios::binary);
    inAuxFile.open(auxFilename,ios::ate|ios::in|ios::out|ios::binary);

    fstream newFileMain;
    fstream newFileAux;
    newFileMain.open("newMainFile.dat",ios::binary|ios::out|ios::app);
    newFileAux.open("newAuxFile.dat",ios::binary|ios::out|ios::app);

    Record prevRecord; 
    Record nextRecord = getFirstRecord(inFile,inAuxFile);  
    prevRecord = nextRecord;
    while (found==false) {
        if(!nextRecord.checkIfKeyEqual(key)) {

            if(prevRecord.pointerTo==this->header.first && prevRecord.inAux==this->header.second){
                if(this->header.second==false)
                    newFileMain.write((char*) &nextRecord, sizeof(Record));
                else 
                    newFileAux.write((char*) &nextRecord, sizeof(Record));
            } else {
                if(prevRecord.inAux==false)
                    newFileMain.write((char*) &nextRecord, sizeof(Record));
                else
                    newFileAux.write((char*) &nextRecord, sizeof(Record));
            }
            prevRecord = nextRecord;

            if(nextRecord.pointerTo==-1)
                break;
            if(!nextRecord.inAux){
                inFile.seekg(nextRecord.pointerTo*sizeof(Record));
                inFile.read((char *) &nextRecord, sizeof(Record)); 
            } else {
                inAuxFile.seekg(nextRecord.pointerTo*sizeof(Record));
                inAuxFile.read((char *) &nextRecord, sizeof(nextRecord));
            }
        }
        else found=true;
    }
    Record tempRecord;
    while (nextRecord.pointerTo!=-1) {
        if(nextRecord.pointerTo==-1)
            break;
        if(!nextRecord.inAux){
            tempRecord.setNewValues(nextRecord,nextRecord.pointerTo-1,nextRecord.inAux);
            newFileMain.write((char*) &tempRecord, sizeof(Record));
            inFile.seekg(nextRecord.pointerTo*sizeof(Record));
            inFile.read((char *) &nextRecord, sizeof(Record)); 
        } else {
            tempRecord.setNewValues(nextRecord,nextRecord.pointerTo-1,nextRecord.inAux);
            newFileAux.write((char*) &tempRecord, sizeof(Record));
            inAuxFile.seekg(nextRecord.pointerTo*sizeof(Record));
            inAuxFile.read((char *) &nextRecord, sizeof(nextRecord));
        }
    }

    if(prevRecord==nextRecord){
        this->header=make_pair(nextRecord.pointerTo,nextRecord.inAux);
    }

    /* inFile.clear();
    inAuxFile.clear(); */
    remove("data.dat");
    remove("aux.dat");
    rename("newMainFile.dat","data.dat");
    rename("newAuxFile.dat","aux.dat");
}

