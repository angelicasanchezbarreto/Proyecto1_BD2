#include "Record.hpp"

void Record::print(){
    string inAux;
    if(this->inAux) inAux = "AUX";
    else inAux = "MAIN";
    cout << bodega << " " << distrito << " " << aforo << " " << productos << 
    " " << ventas1 << " " << ventas2 << " " << ventas3 << " " << ventas4;
}

Record Record::padding(){
    for (int i = 0; i < 30; i++){
        if (bodega[i] == '\0')
            bodega[i] = ' ';
    }
    for (int i = 0; i < 30; i++){
        if (distrito[i] == '\0')
            distrito[i] = ' ';
    }
    return *this;
}

void Record::setPointer(int num){
    this->pointerTo = num;
}

void Record::setWhichFile(bool isOnAux){
    this->inAux = isOnAux;
}

Record Record::operator=(const Record &other) {
    strcpy(bodega,other.bodega);
    strcpy(distrito,other.distrito);
    this->aforo=other.aforo;
    this->productos=other.productos;
    this->ventas1=other.ventas1;
    this->ventas2=other.ventas2;
    this->ventas3=other.ventas3;
    this->ventas4=other.ventas4;
    this->inAux=other.inAux;
    this->pointerTo=other.pointerTo;
    return *this;
}

bool Record::operator==(const Record &other) {
    string s1(this->bodega),s2(other.bodega);
    string s3(this->distrito),s4(other.distrito);
    return (s1==s2 && s3==s4 &&
    this->aforo==other.aforo &&
    this->productos==other.productos &&
    this->ventas1==other.ventas1 &&
    this->ventas2==other.ventas2 &&
    this->ventas3==other.ventas3 &&
    this->ventas4==other.ventas4 &&
    this->inAux==other.inAux && 
    this->pointerTo==other.pointerTo);
}

bool Record::checkIfKeyEqual(const Record& record2){
    string s1(this->bodega),s2(record2.bodega);
    return (s1 == s2);
}

bool Record::compareKey(string key){
    for(int i=0; i<key.length();){
        if(key[i]==this->bodega[i])
            i++;
        else
            return false;
    }
    return true;
}

void Record::setNewValues(Record &other, int pointerTo, bool whichFile){
    *this=other;
    this->setPointer(pointerTo);
    this->setWhichFile(whichFile);
}

void Record::find(ifstream &inFile, ifstream &inAuxFile, string key){
    inFile.read((char *) this, sizeof(Record));
    while (!(this->compareKey(key))) { 
        if(!this->inAux){
            inFile.seekg(this->pointerTo*sizeof(Record));
            inFile.read((char *) this, sizeof(Record));    
        } else {
            inAuxFile.seekg(this->pointerTo*sizeof(Record));
            inAuxFile.read((char *) this, sizeof(Record));
        }
    }  
}