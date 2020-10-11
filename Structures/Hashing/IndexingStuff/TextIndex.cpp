#include "TextIndex.hpp"

TextIndex::TextIndex(int maxKeys, int unique) 
: numKeys(0),keys(0),recordAddresses(0){
    init(maxKeys,unique);
}

TextIndex::~TextIndex(){
    delete keys;
    delete recordAddresses;
}

int TextIndex::insert(const char* key, int recordAddress){
    int i;
    int index = find(key);
    if(this->unique && index>=0) return 0; //key already exists
    if(this->numKeys == this->maxKeys) return 0; //no space for another key
    for(i=this->numKeys-1; i>=0; i--){
        if(strcmp(key,this->keys[i])>0) break; //insert into i+1
        this->keys[i+1] = this->keys[i];
        this->recordAddresses[i+1] = this->recordAddresses[i];
    }
    this->keys[i+1] = strdup(key);
    this->recordAddresses[i+1] = recordAddress;
    this->numKeys++;
    return 1;
}

int TextIndex::remove(const char* key){
    int index = find(key);
    if(index<0) return 0; //key does not exists
    for(int i=index; i<this->numKeys; i++){
        this->keys[i] = this->keys[i+1];
        this->recordAddresses[i] = this->recordAddresses[i+1];
    }
    this->numKeys--;
    return 1;
}

int TextIndex::search(const char* key) const{
    int index = find(key);
    if(index<0) return index; //key not found
    return this->recordAddresses[index];
}

void TextIndex::print(ostream& stream) const{
    stream << "Text Index max keys " << this->maxKeys 
    << " num keys " << this->numKeys << endl;
    for(int i=0; i<this->numKeys; i++){
        stream << "\tKey[" << i << "] " << this->keys[i]
        << " Record address " << this->recordAddresses[i] << endl;
    }
}

int TextIndex::find(const char* key) const{
    for(int i=0; i<this->numKeys; i++){
        if(strcmp(this->keys[i],key)==0) return i; //found
        else if(strcmp(this->keys[i],key)>0) return -1; //not found
    }
    return -1;
}

int TextIndex::init(int maxKeys, int unique){
    this->unique = unique!=0;
    if(maxKeys<=0){
        this->maxKeys=0;
        return 0;
    }

    this->maxKeys=maxKeys;
    this->keys = new char*[maxKeys];
    this->recordAddresses = new int[maxKeys];
    return 1;
}
