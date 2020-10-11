#include "FixedFieldBuffer.hpp"

FixedFieldBuffer::FixedFieldBuffer(int maxFields, int maxBytes)
:FixedLengthBuffer(maxBytes){
    init(maxBytes);
}

static int sumFieldSizes(int numFields, int* fieldSize){
    int sum = 0; 
    for(int i=0; i<numFields; i++)
        sum += fieldSize[i];
    return sum;
}

FixedFieldBuffer &FixedFieldBuffer::operator=(const FixedFieldBuffer &buffer){
    if(this->numFields!=buffer.numFields) return *this;
    for(int i=0; i<this->numFields; i++)
        if(this->fieldSize[i]!=buffer.fieldSize[i]) return *this;
    this->nextField = buffer.nextField;
    FixedLengthBuffer::operator=(buffer);
    return *this;
}

FixedFieldBuffer::FixedFieldBuffer(int numFields, int* fieldSize)
:FixedLengthBuffer(sumFieldSizes(numFields,fieldSize)){
    init(numFields,fieldSize);
}

int FixedFieldBuffer::numberOfFields() const{
    return this->numFields;
}

void FixedFieldBuffer::clear(){
    FixedLengthBuffer::clear();
    this->nextField=0;
    this->bufferValues[0]=0;
    this->packingMode=true;
}

int FixedFieldBuffer::addField(int fieldSize){
    this->initialized=true;
    if(this->numFields==this->maxFields) return false;
    if(this->bufferSize+fieldSize > this->maxBytes) return false;
    this->fieldSize[this->numFields] = fieldSize;
    this->numFields++;
    this->bufferSize += fieldSize;
    return true;
}

static const char* headerString = "Fixed";
static const int headerStringSize = strlen(headerString);

int FixedFieldBuffer::readHeader(istream& stream){
    char *str = new char[headerStringSize+1];
    int numFields, *fieldSize;
    int result;
    result = FixedLengthBuffer::readHeader(stream);
    if(result<0) return -1;
    stream.read(str,headerStringSize);
    if(!stream.good()) return -1;
    if(strncmp(str,headerString,headerStringSize)!=0) return -1;
    stream.read((char*) &numFields, sizeof(numFields));
    if(!stream) return -1;
    fieldSize = new int[numFields];
    for(int i=0; i<numFields; i++)
        stream.read((char*) &fieldSize[i], sizeof(fieldSize[i]));
    if(this->initialized){
        if(numFields!=this->numFields) return -1;
        for(int i=0; i<numFields; i++)
            if(fieldSize[i]!=this->fieldSize[i]) return -1;
        return stream.tellg();
    }
    init(numFields,fieldSize);
    return stream.tellg();
}

int FixedFieldBuffer::writeHeader(ostream& stream) const{
    int result;
    if(!this->initialized) return -1;
    result = FixedLengthBuffer::writeHeader(stream);
    if(!result) return -1;
    stream.write(headerString,headerStringSize);
    if(!stream.good()) return -1;
    stream.write((char*) &this->numFields, sizeof(this->numFields));
    if(!stream.good()) return -1;
    return stream.tellp();
}

int FixedFieldBuffer::pack(const void* field, int size){
    if(this->nextField==this->numFields || !this->packingMode) 
        return -1;
    int start = this->nextByte;
    int packSize = this->fieldSize[this->nextField];
    if(size!=-1 && packSize!=size) return -1;
    memcpy(&this->bufferValues[start], field, packSize);
    this->nextByte += packSize;
    this->nextField++;
    if(this->nextField==this->numFields){
        this->packingMode=-1;
        this->nextField=this->nextByte=0;
    }
    return packSize;
}

int FixedFieldBuffer::unpack(void* field, int maxBytes){
    this->packingMode=false;
    if(this->nextField==this->numFields) return -1;
    int start = this->nextByte;
    int packSize = this->fieldSize[this->nextField];
    memcpy(field, &this->bufferValues[start], packSize);
    this->nextByte += packSize;
    this->nextField++;
    if(this->nextField==this->numFields) clear();
    return packSize;
}

void FixedFieldBuffer::print(ostream& stream) const{
    FixedLengthBuffer::print(stream);
    stream << endl;
    stream << "\t max fields " << this->maxFields 
    << " and actual " << this->numFields << endl;
    for(int i=0; i<this->numFields; i++)
        stream << "\tfield " << i << " size " << this->fieldSize[i] << endl;
    this->bufferValues[this->bufferSize]=0;
    stream << "NextByte " << this->nextByte << endl;
    stream << "Buffer '" << this->bufferValues << "'" << endl;
}

int FixedFieldBuffer::init(int numFields, int* fieldSize){
    this->initialized=true;
    init(numFields);
    for(int i=0; i<numFields; i++)
        addField(this->fieldSize[i]);
    return true;
}