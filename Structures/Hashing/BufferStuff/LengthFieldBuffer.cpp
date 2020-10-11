#include "LengthFieldBuffer.hpp"

LengthFieldBuffer::LengthFieldBuffer(int maxBytes):VariableLengthBuffer(maxBytes){
    init();
}

void LengthFieldBuffer::clear(){ 
    VariableLengthBuffer::clear();
}

int LengthFieldBuffer::pack(const void* field, int size){
    short length;
    if(size>=0) 
        length = size;
    else 
        length = strlen((char*) field);
    int start = this->nextByte;
    this->nextByte += (length + sizeof(length));
    if(this->nextByte > this->maxBytes) return -1;
    memcpy(&bufferValues[start], &length, sizeof(length));
    memcpy(&bufferValues[start+sizeof(length)], field, length);
    this->bufferSize = this->nextByte;
    return length;
}

int LengthFieldBuffer::unpack(void* field, int maxBytes){
    short length;
    if(this->nextByte>= this->bufferSize) return -1;
    int start = this->nextByte;
    memcpy(&length, &bufferValues[start], sizeof(length));
    if(maxBytes!=-1 && length>maxBytes) return -1;
    this->nextByte += length+sizeof(length);
    if(this->nextByte>bufferSize) return -1;
    memcpy(field, &bufferValues[start+sizeof(length)], length);
    if(maxBytes>length || maxBytes==-1)
        ((char*) field) [length] = 0;
    return length;
}

void LengthFieldBuffer::print(ostream& stream) const{
    stream << "Buffer has characters " << this->maxBytes << " and buffer size " 
    << this->bufferSize << endl;
}

int LengthFieldBuffer::init(){
    initialized = true;
    clear();
    return true;
}

