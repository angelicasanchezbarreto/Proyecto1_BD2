#ifndef fixedLengthBuffer_hpp
#define fixedLengthBuffer_hpp

#include "Iobuffer.hpp"

class FixedLengthBuffer : public IOBuffer{
    public:
    FixedLengthBuffer(int recordSize=1000);
    FixedLengthBuffer(const FixedLengthBuffer& buffer);
    void clear();
    int read(istream&);
    int write(ostream&)const;
    int readHeader(istream&);
    int writeHeader(ostream&) const;
    void print(ostream&)const;
    int sizeOfBuffer() const;

    protected:
    int init(int recordSize);
    int changeRecordSize(int recordSize);
};

#endif