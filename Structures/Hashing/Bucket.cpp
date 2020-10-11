#include "Bucket.hpp"

//CLASS BUCKET

Bucket::Bucket(Directory& dir, int maxKeys):TextIndex(maxKeys), directory(dir){
    this->bucketAddress = 0;
    this->localDepth = 0;
}

int Bucket::insert(char* key, int recordAddress){
    if(this->numKeys < this->maxKeys){
        int result = TextIndex::insert(key,recordAddress);
        directory.storeBucketInDirectory(this);
        return result;
    }
    else{
        split();
        return this->directory.insert(key,recordAddress);
    }
}

int Bucket::remove(char* key){
    int result = TextIndex::remove(key);
    if(!result) return 0;
    tryCombine();
    this->directory.storeBucketInDirectory(this);
    return 1;
}

Bucket* Bucket::split(){
    int newStart, newEnd;
    if(this->localDepth==this->directory.globalDepth)
        this->directory.doubleSizeDirectory();
    Bucket* newBucket = new Bucket(this->directory, this->maxKeys);
    this->directory.storeBucketInDirectory(newBucket);
    newRange(newStart,newEnd);
    this->directory.insertBucketInDirectory(newBucket->bucketAddress,newStart,newEnd);
    this->localDepth++;
    newBucket->localDepth = this->localDepth;
    redistribute(*newBucket);
    this->directory.storeBucketInDirectory(this);
    this->directory.storeBucketInDirectory(newBucket);
    return newBucket;
}

int Bucket::findBuddy(){
    if(this->directory.globalDepth==0) return -1;
    if(this->localDepth<this->directory.globalDepth) return -1;
    int sharedAddress = MakeAddress(keys[0], this->localDepth);
    return sharedAddress^1;
}

int Bucket::tryCombine(){
    int result;
    int buddyIndex = findBuddy();
    if(buddyIndex==-1) return 0;
    int buddyAddress = this->directory.bucketAddresses[buddyIndex];
    Bucket* buddyBucket = new Bucket(this->directory, maxKeys);
    this->directory.loadBucketInDirectory(buddyBucket, buddyAddress);
    if(numKeys+buddyBucket->numKeys > maxKeys) return 0;
    combine(buddyBucket, buddyIndex);
    result = this->directory.collapseDirectory();
    if(result) tryCombine();
    return 0;
}

int Bucket::combine(Bucket* buddy, int buddyIndex){
    int result;
    for(int i=0; i<buddy->numKeys; i++){
        result = insert(buddy->keys[i], buddy->recordAddresses[i]);
        if(!result) return 0;
    }
    this->localDepth--;
    this->directory.removeBucketInDirectory(buddyIndex,this->localDepth);
    return 1;
}

int Bucket::newRange(int& newStart, int& newEnd){
    int sharedAddress = MakeAddress(keys[0],this->localDepth);
    int bitsToFill = this->directory.globalDepth - (this->localDepth+1);
    newStart = (sharedAddress << 1)|1;
    newEnd = newStart;
    for(int i=0; i<bitsToFill; i++){
        newStart = newStart << 1;
        newEnd = (newEnd << 1)|1;
    }
    return 1;
}

int Bucket::redistribute(Bucket& newBucket){
    for(int i=numKeys-1; i>=0; i--){
        int bucketAddress = this->directory.findBucketInDirectory(keys[i]);
        if(bucketAddress!=this->bucketAddress){
            newBucket.TextIndex::insert(keys[i],recordAddresses[i]);
            TextIndex::remove(keys[i]);
        }
    }
    return 1;
}

/* ostream& Bucket::printBucket(ostream& stream){
    stream << "Bucket depth: " << this->localDepth << endl;
    TextIndex::print(stream);
    return stream;
} */

//CLASS BUCKET BUFFER

BucketBuffer::BucketBuffer(int keySize, int maxKeys)
:TextIndexBuffer(keySize,maxKeys,1,sizeof(int)){
    addField(sizeof(int));
}

int BucketBuffer::pack(const Bucket& bucket){
    int result;
    TextIndexBuffer::pack(bucket);
    if(result==-1) return -1;
    return FixedFieldBuffer::pack(&bucket.localDepth);
}

int BucketBuffer::unpack(Bucket& bucket){
    int result;
    result = TextIndexBuffer::unpack(bucket);
    if(result==-1) return -1;
    return FixedFieldBuffer::unpack(&bucket.localDepth);
}