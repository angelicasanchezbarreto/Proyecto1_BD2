#ifndef iobuffer_hpp
#define iobuffer_hpp

#include <fstream>
#include <cstring>

using namespace std;

class IOBuffer{
    public:
    IOBuffer(int maxBytes=1000);
    IOBuffer& operator=(const IOBuffer&);
    virtual void clear();
    virtual int pack(const void* field, int size=-1)=0;
    virtual int unpack(void *field, int maxBytes=-1)=0;
    virtual void print(ostream&) const;
    int initBytes(int maxBytes);
    virtual int read(istream&)=0;
    virtual int write(ostream&)const =0;
    virtual int dRead(istream&, int recordAddress);
    virtual int dWrite(ostream&, int recordAddress) const;
    virtual int readHeader(istream&);
    virtual int writeHeader(ostream&) const;

    protected:
    int initialized; //TRUE if is initialized
    char* bufferValues; //Buffer
    int bufferSize;
    int maxBytes;
    int nextByte;
    int packingMode; //TRUE if is in packing mode
};

#endif