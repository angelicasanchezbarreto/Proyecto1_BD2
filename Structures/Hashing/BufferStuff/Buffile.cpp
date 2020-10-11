#include "Buffile.hpp"

//PUBLIC METHODS

BufferFile::BufferFile(IOBuffer& from):Buffer(from){};

int BufferFile::openBuffer(string filename, int mode){
    /* if(mode&ios::trunc||mode&ios::noreplace) return false;
    this->file.open(filename,mode|ios::in|ios::nocreate|ios::binary); */
    if(mode&ios::trunc) return false;
    this->file.open(filename,ios::in|ios::binary);
    if(!this->file.good()) return false;

    this->file.seekg(0,ios::beg);
    this->file.seekp(0,ios::beg);
    this->headerSize=readHeader();
    if(!this->headerSize) return false;

    this->file.seekp(this->headerSize,ios::beg);
    this->file.seekg(this->headerSize,ios::beg);
    return this->file.good();
}

int BufferFile::createBuffer(string filename, int mode){
    /* if(!mode&ios::out||mode&ios::noreplace) return false;
    this->file.open(filename,mode|ios::out|ios::noreplace|ios::binary); */
    if(!mode&ios::out) return false;
    this->file.open(filename,ios::out|ios::binary);
    if(!this->file.good()){
        this->file.close();
        return false;
    }
    this->headerSize=writeHeader();
    return this->headerSize!=0;
}

int BufferFile::closeBuffer(){
    this->file.close();
    return true;
}

int BufferFile::rewindBuffer(){
    this->file.seekg(this->headerSize,ios::beg);
    this->file.seekp(this->headerSize,ios::beg);
    return 1;
}

int BufferFile::readBuffer(int recordAddress){
    if(recordAddress==-1)
        return this->Buffer.read(this->file);
    else 
        return this->Buffer.dRead(this->file,recordAddress);
}

int BufferFile::writeBuffer(int recordAddress){
    if(recordAddress==-1)
        return this->Buffer.write(this->file);
    else 
        return this->Buffer.dWrite(this->file,recordAddress);
}

int BufferFile::appendBuffer(){
    this->file.seekp(0,ios::end);
    return this->Buffer.write(this->file);
}

IOBuffer& BufferFile::getBuffer(){
    return this->Buffer;
}

//PROTECTED METHODS

int BufferFile::readHeader(){
    return this->Buffer.readHeader(this->file);
}

int BufferFile::writeHeader(){
    return this->Buffer.writeHeader(this->file);
}