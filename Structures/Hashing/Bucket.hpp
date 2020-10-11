#ifndef bucket_hpp
#define bucket_hpp

#include <fstream>
#include "IndexingStuff/TextIndexBuffer.cpp"
#include "Directory.hpp"
#include "Hash.hpp"
using namespace std;

class Directory;
const int defaultMaxKeys = 100;

class Bucket : protected TextIndex{
    protected:
    //the access to the bucket members is only through class directory
    Bucket(Directory &dir, int maxKeys = defaultMaxKeys);
    int insert(char* key, int recordAddress);
    int remove(char* key);
    Bucket* split(); //returns memory address of new bucket
    int newRange(int &newStart, int &newEnd); //calculate the range of new split bucket
    int redistribute(Bucket &newBucket);
    int findBuddy(); //next bucket
    int tryCombine();
    int combine(Bucket* buddy, int buddyIndex);
    //ostream& printBucket(ostream& stream);

    int localDepth; //# bits used in common by keys in bucket
    Directory& directory;
    int bucketAddress;
    friend class Directory;
    friend class BucketBuffer;
};

class BucketBuffer : public TextIndexBuffer{
    public:
    BucketBuffer(int keySize, int maxKeys);
    int pack(const Bucket &bucket);
    int unpack(Bucket &bucket);
};

#endif