#include <iostream>
#include <cstring>

#include "TextIndexBuffer.cpp"

using namespace std;

class TextIndex{
    public:
    TextIndex(int maxKeys=100, int unique=1);
    ~TextIndex();
    int insert(const char* key, int recordAddress);
    int remove(const char* key);
    int search(const char* key) const;
    void print(ostream&) const;

    protected:
    int maxKeys;
    int numKeys;
    char** keys;
    int* recordAddresses;
    int find(const char* key) const;
    int init(int maxKeys, int unique);
    int unique; //if true, each value must be unique

    friend class TextIndexBuffer;
};

