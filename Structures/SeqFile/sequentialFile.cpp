#include "recordSeq.cpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <iomanip> 

bool compare(const RecordSeq& record1, const RecordSeq& record2){
    string s1(record1.nombre),s2(record2.nombre);
    return (s1 < s2);
}

class SequentialFile{
    private:
        string filename;
        string auxFilename;

        void writeRecord(RecordSeq record,string filename){        
            ofstream outFile;        
            outFile.open(filename, ios::binary | ios::app);     
            if(!outFile)
                exit(EXIT_FAILURE);   
            outFile.write((char *) &record, sizeof(record));        
            outFile.close();    
        }
        
        void checkOpening(ifstream &mainFile, ifstream &auxFile){
            mainFile.open(filename, ios::binary|ios::in);  
            if(!mainFile)
                exit(EXIT_FAILURE);  
            auxFile.open(auxFilename, ios::binary|ios::in); 
        }

        int numberOfRecords(string whichFile){
            ifstream inFile;
            inFile.open(whichFile, ios::binary|ios::in);  
            if(!inFile)
                return 0;  
            int begin = inFile.tellg();
            inFile.seekg (0, ios::end);
            int end = inFile.tellg();
            int num = (end-begin)/sizeof(RecordSeq);
            inFile.close();
            return num;
        }

        void update(RecordSeq record){
            fstream file;
            fstream auxfile;
            file.open(filename,ios::ate|ios::in|ios::out|ios::binary);
            auxfile.open(auxFilename,ios::ate|ios::in|ios::out|ios::binary);

            RecordSeq nextRecord;   
            RecordSeq prevRecord; 
            file.seekg(0); 
            file.read((char *) &nextRecord, sizeof(nextRecord));
            prevRecord = nextRecord;
            while (!file.eof()) {
                if(!prevRecord.inAux){
                    if (record.checkIfKeyEqual(nextRecord)){
                        file.seekp(file.tellg() - streamoff(sizeof(RecordSeq)));
                        file.write((char *)&record, sizeof(RecordSeq));
                        break;
                    }
                    prevRecord = nextRecord;
                    file.read((char *)&nextRecord, sizeof(RecordSeq));
                } else {
                    if (record.checkIfKeyEqual(nextRecord)){
                        auxfile.seekp(file.tellg() - streamoff(sizeof(RecordSeq)));
                        auxfile.write((char *)&record, sizeof(RecordSeq));
                        break;
                    }
                    prevRecord = nextRecord;
                    auxfile.read((char *)&nextRecord, sizeof(RecordSeq));
                }
            }
            file.close();
        } 

        void reconstruction(){
            ifstream inFile;
            ifstream inAuxFile;        
            checkOpening(inFile,inAuxFile);
            fstream newFile;
            newFile.open("newFile.dat",ios::binary|ios::out|ios::app);
            RecordSeq record;
            RecordSeq toInsert;
            int pos=1;
            inFile.read((char *) &record, sizeof(record));
            toInsert.setNewValues(record,pos,false);
            newFile.write((char *) &toInsert, sizeof(record)); 
            while(!inFile.eof()){
                pos++;
                if(!record.inAux){
                    inFile.seekg(record.pointerTo*sizeof(RecordSeq));
                    inFile.read((char *) &record, sizeof(record)); 
                    toInsert.setNewValues(record,pos,false);
                    newFile.write((char *) &toInsert, sizeof(record)); 
                } else {
                    inAuxFile.seekg(record.pointerTo*sizeof(RecordSeq));
                    inAuxFile.read((char *) &record, sizeof(record));
                    toInsert.setNewValues(record,pos,false);
                    newFile.write((char *) &toInsert, sizeof(record)); 
                }
            }
            this->filename = "newFile.dat";
            remove("aux.dat");
        }

    public:
    
        SequentialFile(string filename){
            this->filename = filename;
            this->auxFilename = "aux.dat";
        }

        void insertAll(vector<RecordSeq> records){
            sort(records.begin(),records.end(),compare);
            for(int i=0; i<records.size(); i++){
                records[i].setPointer(i+1);
                writeRecord(records[i],filename);
            }
        }

        RecordSeq search(string key){
            ifstream inFile;
            ifstream inAuxFile;        
            checkOpening(inFile,inAuxFile);
            RecordSeq record;   
            record.find(inFile,inAuxFile,key); 
            inFile.close();  
            inAuxFile.close();  
            return record;
        }

        vector<RecordSeq> search(string begin, string end){
            vector<RecordSeq> records;
            ifstream inFile;
            ifstream inAuxFile;        
            checkOpening(inFile,inAuxFile); 

            RecordSeq record;   
            record.find(inFile,inAuxFile,begin); 
            records.push_back(record);

            while (!(record.compareKey(end))) { 
                if(!record.inAux){
                    inFile.seekg(record.pointerTo*sizeof(RecordSeq));
                    inFile.read((char *) &record, sizeof(record));    
                } else {
                    inAuxFile.seekg(record.pointerTo*sizeof(RecordSeq));
                    inAuxFile.read((char *) &record, sizeof(record));
                }
                records.push_back(record);
            }
            inFile.close();  
            inAuxFile.close();  
            return records;
        }

        void scanAll(){
            ifstream inFile;
            ifstream inAuxFile;
            checkOpening(inFile,inAuxFile);

            RecordSeq record;
            if(numberOfRecords(filename)>0){
                cout << "IN MAIN FILE:" << endl;
                while(inFile.read((char*) &record, sizeof(record))){
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

        void add(RecordSeq record){
            fstream inFile;
            fstream inAuxFile;        
            inFile.open(filename, ios::binary|ios::in|ios::out);  
            if(!inFile)
                exit(EXIT_FAILURE);  
            inAuxFile.open(auxFilename, ios::binary|ios::in|ios::out);  
            
            if(numberOfRecords(auxFilename)<5){
                RecordSeq nextRecord;   
                RecordSeq prevRecord;   
                inFile.read((char *) &nextRecord, sizeof(nextRecord));
                prevRecord = nextRecord;
                while (!inFile.eof()) {
                    if(compare(nextRecord,record)) {
                        prevRecord = nextRecord;
                        if(!nextRecord.inAux){
                            inFile.seekg(nextRecord.pointerTo*sizeof(RecordSeq));
                            inFile.read((char *) &nextRecord, sizeof(nextRecord));    
                        } else {
                            inAuxFile.seekg(nextRecord.pointerTo*sizeof(RecordSeq));
                            inAuxFile.read((char *) &nextRecord, sizeof(nextRecord));
                        }
                    }
                    else break;
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

            if(numberOfRecords(auxFilename)==5){
                reconstruction();
            }

            inFile.close();
            inAuxFile.close();
        }

        bool remove(int pos){
            fstream inFile;
            fstream inAuxFile;
            inFile.open(filename, ios::binary|ios::in|ios::out);
            inAuxFile.open(auxFilename, ios::binary|ios::in|ios::out);

            while(inFile.eof()){
                
            }
        }

        /* ~SequentialFile(){
            remove("datos1.dat");
            remove("aux.dat");
        } */
};