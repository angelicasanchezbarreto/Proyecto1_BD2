#include "Iobuffer.hpp"

IOBuffer::IOBuffer(int maxBytes ){
    initBytes(maxBytes);
}

IOBuffer& IOBuffer::operator=(const IOBuffer& other){
    if(this->maxBytes<other.bufferSize) return *this; //fails

    this->initialized=other.initialized;
    this->bufferSize=other.bufferSize;
    memcpy(this->bufferValues,other.bufferValues,other.bufferSize);
    this->nextByte=other.nextByte;
    this->packingMode=other.packingMode;
    return *this;
}

void IOBuffer::clear(){
    this->nextByte=0;
    this->packingMode=true;
}

void IOBuffer::print(ostream& stream) const{
    stream << "Max bytes: "<< this->maxBytes << " Buffer size: " << this->bufferSize;
}

int IOBuffer::initBytes(int maxBytes){
    this->initialized=false;
    if(maxBytes<0) this->maxBytes=0;
    this->maxBytes=maxBytes;
    this->bufferValues=new char[maxBytes]; //check
    this->bufferSize=0;
    clear();
    return 1;
}

int IOBuffer::dRead(istream& stream, int recordAddress){
    stream.seekg(recordAddress, ios::beg);
    if(stream.tellg()!= recordAddress) return -1;
    return read(stream);
}

int IOBuffer::dWrite(ostream& stream, int recordAddress) const{
    stream.seekp(recordAddress, ios::beg);
    if(stream.tellp()!= recordAddress) return -1;
    return write(stream);
}

static const char* headerStr = "IOBuffer";
static const int headerSize = strlen(headerStr);

int IOBuffer::readHeader(istream& stream){
    char str[9];
    stream.seekg(0, ios::beg);
    stream.read(str, headerSize);
    if(!stream.good()) return -1;
    if(strncmp(str,headerStr,headerSize)==0) 
        return headerSize;
    else return -1;
}

int IOBuffer::writeHeader(ostream& stream) const{
    stream.seekp(0, ios::beg);
    stream.write(headerStr, headerSize);
    if(!stream.good()) return -1;
    return headerSize;
}