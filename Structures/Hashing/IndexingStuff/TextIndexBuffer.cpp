#include "TextIndexBuffer.hpp"

TextIndexBuffer::TextIndexBuffer(int keySize, int maxKeys,int extraFields,
int extraSize):FixedFieldBuffer(1+(2*keySize)+(extraFields*sizeof(int))+extraSize){
    this->maxKeys = maxKeys;
    this->keySize = keySize;
    addField(sizeof(int));
    for(int i=0; i<maxKeys; i++){
        addField(keySize);
        addField(sizeof(int));
    }
    this->dummy = new char[keySize+1];
}

int TextIndexBuffer::pack(const TextIndex& index){
    int result;
    clear();
    result = FixedFieldBuffer::pack(&index.numKeys);
    for(int i=0; i<index.numKeys; i++){
        result = result && FixedFieldBuffer::pack(index.keys[i]);
        result = result && FixedFieldBuffer::pack(&index.recordAddresses[i]);
    }

    for(int i=0; i<index.maxKeys-index.numKeys; i++){
        result = result && FixedFieldBuffer::pack(this->dummy);
        result = result && FixedFieldBuffer::pack(this->dummy);
    }
    return result;
}

int TextIndexBuffer::unpack(TextIndex& index){
    int result;
    clear();
    result = FixedFieldBuffer::unpack(&index.numKeys);
    for(int i=0; i<index.numKeys; i++){
        index.keys[i] = new char[this->keySize];
        result = result && FixedFieldBuffer::unpack(index.keys[i]);
        result = result && FixedFieldBuffer::unpack(&index.recordAddresses[i]);
    }

    for(int i=0; i<index.maxKeys-index.numKeys; i++){
        result = result && FixedFieldBuffer::unpack(this->dummy);
        result = result && FixedFieldBuffer::unpack(this->dummy);
    }
    return result;
}

void TextIndexBuffer::print(ostream& stream)const {
    stream << "TextIndexBuffer: KeySize " << this->keySize 
    << " MaxKeys " << this->maxKeys << endl;
    FixedFieldBuffer::print(stream);
}
