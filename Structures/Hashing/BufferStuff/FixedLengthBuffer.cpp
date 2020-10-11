#include "FixedLengthBuffer.hpp"

inline FixedLengthBuffer::FixedLengthBuffer(const FixedLengthBuffer& buffer)
:IOBuffer(buffer){
    init(buffer.bufferSize);
}

FixedLengthBuffer::FixedLengthBuffer(int recordSize):IOBuffer(recordSize){
    init(recordSize);
}

void FixedLengthBuffer::clear(){
    IOBuffer::clear();
    this->bufferValues[0]=0;
    this->packingMode=true;
}

int FixedLengthBuffer::read(istream& stream){
    int recordAddress = stream.tellg();
    stream.clear();
    clear();
    this->packingMode=false;
    stream.read(this->bufferValues,this->bufferSize);
    if(!stream.good()){
        stream.clear();
        return recordAddress;
    }
    return recordAddress;
}

int FixedLengthBuffer::write(ostream& stream)const{
    int recordAddress = stream.tellp();
    stream.write(this->bufferValues,this->bufferSize);
    if(!stream.good()){
        return -1;
    }
    return recordAddress;
}

static const char* headerString = "Fixed";
static const int headerStringSize = strlen(headerString);

int FixedLengthBuffer::readHeader(istream& stream){
    char str[headerStringSize+1];
    int recordSize;
    int result;
    result = IOBuffer::readHeader(stream);
    if(result<0) return -1;
    stream.read(str,headerStringSize);
    if(!stream.good()) return -1;
    if(strncmp(str,headerString,headerStringSize)!=0) return -1;
    stream.read((char*) &recordSize, sizeof(recordSize));
    if(this->initialized){
        if(recordSize!=this->bufferSize) return -1;
    }
    changeRecordSize(recordSize);
    return stream.tellg();
}

int FixedLengthBuffer::writeHeader(ostream& stream) const{
    int result;
    if(!this->initialized) return -1;
    result = IOBuffer::writeHeader(stream);
    if(!result) return -1;
    stream.write(headerString,headerStringSize);
    if(!stream.good()) return -1;
    stream.write((char*) &this->bufferSize, sizeof(this->bufferSize));
    if(!stream.good()) return -1;
    return stream.tellp();
}

void FixedLengthBuffer::print(ostream& stream) const{
    IOBuffer::print(stream);
    stream << "Fixed ";
}

int FixedLengthBuffer::init(int recordSize){
    clear();
    this->bufferSize=recordSize;
    return 1;
}

int FixedLengthBuffer::changeRecordSize(int recordSize){
    this->bufferSize=recordSize;
    return 1;
}