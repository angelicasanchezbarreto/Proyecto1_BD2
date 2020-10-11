#include "Directory.hpp"

const int maxKeySize = 12;

void makeNames(char* name, char*& dirName, char*& bktName){
    ostringstream  directoryName;
    directoryName << name << ".dir" << ends;
    dirName = strdup(directoryName.str().c_str());
    ostringstream bucketName;
    bucketName << name << ".bkt" << ends;
    bktName = strdup(bucketName.str().c_str());
}

Directory::Directory(int maxBucketKeys){
    this->globalDepth = 0;
    this->numCells = 1;
    this->bucketAddresses = new int[numCells];
    this->maxBucketKeys = maxBucketKeys;
    this->directoryBuffer =  new LengthFieldBuffer;
    this->directoryFile = new BufferFile(*directoryBuffer);
    this->currentBucket = new Bucket(*this,maxBucketKeys);
    this->theBucketBuffer = new BucketBuffer(maxKeySize,this->maxBucketKeys);
    this->bucketFile = new BufferFile(*theBucketBuffer);
    this->printBucket = new Bucket(*this, maxBucketKeys);
}

Directory::~Directory(){
    close();
}

int Directory::open(char* name){ 
    int result;
    char* directoryName, *bucketName;
    makeNames(name, directoryName, bucketName);

    result = this->directoryFile->openBuffer(directoryName, ios::in|ios::out);
    if(!result) return 0;
    result = this->directoryFile->readBuffer();
    if(result==-1) return 0;
    result = unpack();
    if(!result==-1) return 0;
    result = this->bucketFile->openBuffer(bucketName, ios::in|ios::out);
    return result;
}

int Directory::create(char* name){
    int result;
    char* directoryName, *bucketName;
    makeNames(name, directoryName, bucketName);

    result = this->directoryFile->createBuffer(directoryName, ios::in|ios::out);
    if(!result) return 0;
    result = this->bucketFile->createBuffer(bucketName, ios::in|ios::out);
    if(!result) return 0;
    this->bucketAddresses[0] = storeBucketInDirectory(this->currentBucket);
    return result;
}

int Directory::close(){
    int result;

    result = pack();
    if(result==-1) return 0;
    this->directoryFile->rewindBuffer();
    result = this->directoryFile->writeBuffer();
    if(result==-1) return 0;
    return this->directoryFile->closeBuffer() && this->bucketFile->closeBuffer();
}

int Directory::insert(char* key, int recordAddress){
    int found = search(key);
    if(found==-1)
        return this->currentBucket->insert(key,recordAddress);
    return 0;
}

int Directory::search(char* key){
    int bucketAddress = findBucketInDirectory(key);
    loadBucketInDirectory(this->currentBucket, bucketAddress);
    return this->currentBucket->search(key);
}

int Directory::findBucketInDirectory(char* key){
    return this->bucketAddresses[MakeAddress(key,this->globalDepth)];
}


int Directory::remove(char* key){
    int bucketAddress = findBucketInDirectory(key);
    loadBucketInDirectory(this->currentBucket, bucketAddress);
    return this->currentBucket->remove(key);
}

int Directory::doubleSizeDirectory(){
    int newSize = 2*this->numCells;
    int* newBucketAddresses = new int[newSize];
    for(int i=0; i<this->numCells; i++){ //double the coverage of each bucket
        newBucketAddresses[2*i] = this->bucketAddresses[i];
        newBucketAddresses[2*i+1] = this->bucketAddresses[i];
    }
    delete this->bucketAddresses;
    this->bucketAddresses = newBucketAddresses;
    this->globalDepth++;
    this->numCells = newSize;
    return 1;
}

int Directory::collapseDirectory(){
    if(this->globalDepth==0) return 0;
    for(int i=0; i<this->numCells; i+=2){
        if(this->bucketAddresses[i]!=this->bucketAddresses[i+1])
            return 0;
    }
    int newSize = this->numCells/2;
    int* newBucketAddresses = new int[newSize];
    for(int j=0; j<newSize; j++)
        newBucketAddresses[j] = this->bucketAddresses[j*2];
    delete this->bucketAddresses;
    this->bucketAddresses = newBucketAddresses;
    this->globalDepth--;
    this->numCells = newSize;
    return 1;
}

int Directory::removeBucketInDirectory(int bucketIndex, int bucketDepth){
    int fillBits = this->globalDepth - bucketDepth;
    int buddyIndex = bucketIndex^1<<(fillBits-1);
    int newBucketAddress = this->bucketAddresses[buddyIndex];
    int lowIndex = bucketIndex >> fillBits << fillBits;
    int highIndex = lowIndex + (1<<fillBits) -1;
    for(int i=lowIndex; i<=highIndex; i++)
        this->bucketAddresses[i] = newBucketAddress;
    return 0;
}

int Directory::insertBucketInDirectory(int bucketAddress, int first, int last){
    for(int i=first; i<=last; i++)
        this->bucketAddresses[i] = bucketAddress;
    return 1;
}

int Directory::pack() const{
    int result, packSize;
    this->directoryBuffer->clear();
    packSize = this->directoryBuffer->pack(&this->globalDepth, sizeof(int));
    if(packSize==-1) return -1;
    for(int i=0; i<this->numCells; i++){
        result = this->directoryBuffer->pack(&this->bucketAddresses[i],sizeof(int));
        if(result==-1) return -1;
        packSize += result;
    }
    return packSize;
}

int Directory::unpack(){
    int result;
    result = this->directoryBuffer->unpack(&this->globalDepth, sizeof(int));
    if(result==-1) return -1;
    this->numCells = 1 << this->globalDepth;
    if(this->bucketAddresses!=0) delete this->bucketAddresses;
    this->bucketAddresses = new int[numCells];
    for(int i=0; i<numCells; i++){
        result = this->directoryBuffer->unpack(&bucketAddresses[i],sizeof(int));
        if(result==-1) return -1;
    }
    return 0;
}

int Directory::storeBucketInDirectory(Bucket* bucket){
    int result;
    result = this->theBucketBuffer->pack(*bucket);
    if(result==-1) return -1;
    int address = bucket->bucketAddress;
    if(address!=0) return this->bucketFile->writeBuffer(address);
    address = this->bucketFile->appendBuffer();
    bucket->bucketAddress = address;
    return address;
}

int Directory::loadBucketInDirectory(Bucket* bucket, int bucketAddress){
    int result;
    result = this->bucketFile->readBuffer(bucketAddress);
    if(result==-1) return 0;
    result = this->theBucketBuffer->unpack(*bucket);
    if(result!=-1) return 0;
    bucket->bucketAddress = bucketAddress;
    return 1;
}

ostream& Directory::print(ostream& stream){
    stream << "Directory depth " << this->globalDepth << " size " << this->numCells << endl;
    for(int i=0; i<this->numCells; i++){
        stream << "bucket " << this->bucketAddresses[i] 
        << " address " << (void*) i << endl;  
        loadBucketInDirectory(this->printBucket, this->bucketAddresses[i]);
        this->printBucket->print(stream);
    }
    stream << "end directory" << endl;
    return stream;
}