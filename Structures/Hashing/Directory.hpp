#ifndef directory_hpp
#define directory_hpp

#include "BufferStuff/Buffile.cpp"
#include "Bucket.cpp"
#include "BufferStuff/LengthFieldBuffer.cpp"
#include "Hash.hpp"
#include <sstream>

/* class Bucket;
class BucketBuffer; */

class Directory{
    public:
    Directory(int maxBucketKeys=-1);
    ~Directory();

    int open(char* name); //reads the directory from file
    int create(char* name);
    int close(); //writes directory back into file
    int insert(char* key, int recordAddress);
    int remove(char* key);
    int search(char* key); //returns recAddr for key
    ostream& print(ostream& stream);

    protected:
    int globalDepth;
    int numCells; //2^depth
    int* bucketAddresses; //array of bucket addresses

    int doubleSizeDirectory();
    int collapseDirectory(); //halve the size
    int insertBucketInDirectory(int bucketAddress, int first, int last);
    int removeBucketInDirectory(int bucketIndex, int depth);
    int findBucketInDirectory(char* key); //finds bucket address for key
    int storeBucketInDirectory(Bucket* bucket); //update bucket in file
    int loadBucketInDirectory(Bucket* bucket, int bucketAddress);
    

    int maxBucketKeys;
    BufferFile* directoryFile;
    LengthFieldBuffer* directoryBuffer;
    Bucket* currentBucket;
    BucketBuffer* theBucketBuffer;
    BufferFile* bucketFile;
    Bucket* printBucket;
    int pack()const;
    int unpack();

    friend class Bucket;
};

#endif