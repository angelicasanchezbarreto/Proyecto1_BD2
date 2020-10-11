#include "VariableLengthBuffer.hpp"

VariableLengthBuffer::VariableLengthBuffer(int maxBytes):IOBuffer(maxBytes){
    init();
}

void VariableLengthBuffer::clear(){ 
    IOBuffer::clear();
}

int VariableLengthBuffer::read(istream& stream){
    if(stream.eof()) return -1;
    int recordAddress = stream.tellg();
    clear();
    unsigned short bufferSize;
    stream.read((char*) &bufferSize, sizeof(bufferSize));
    if(!stream.good()){
        stream.clear();
        return -1;
    }
    this->bufferSize=bufferSize;
    if(this->bufferSize>this->maxBytes) return -1;
    stream.read(this->bufferValues,this->bufferSize);
    if(!stream.good()){
        stream.clear();
        return -1;
    }
    return recordAddress;
}

int VariableLengthBuffer::write(ostream& stream) const{
    int recordAddress = stream.tellp();
    unsigned short bufferSize;
    bufferSize=this->bufferSize;
    stream.write((char*) &bufferSize, sizeof(bufferSize));
    if(!stream) return -1;
    stream.write(this->bufferValues,this->bufferSize);
    if(!stream.good()) return -1;
    return recordAddress;
}

static const char* headerString = "Variable";
static const int headerStringSize = strlen(headerString);

int VariableLengthBuffer::readHeader(istream& stream){
    char str[headerStringSize+1];
    int result;
    result = IOBuffer::readHeader(stream);
    if(!result) return false;
    stream.read(str,headerStringSize);
    if(!stream.good()) return false;
    if(strncmp(str,headerString,headerStringSize)!=0) return false;
    return stream.tellg();
}

int VariableLengthBuffer::writeHeader(ostream& stream) const{
    int result;
    result = IOBuffer::writeHeader(stream);
    if(!result) return false;
    stream.write(headerString,headerStringSize);
    if(!stream.good()) return -1;
    return stream.tellp();
}

void VariableLengthBuffer::print(ostream& stream) const{
    IOBuffer::print(stream);
}

int VariableLengthBuffer::init(){
    clear();
    return true;
}
