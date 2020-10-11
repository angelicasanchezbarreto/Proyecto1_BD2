#ifndef variablelengthbuffer_hpp
#define variablelengthbuffer_hpp

#include "Iobuffer.cpp"

class VariableLengthBuffer : public IOBuffer{
    public:
    VariableLengthBuffer(int maxBytes=1000);
    VariableLengthBuffer(const VariableLengthBuffer& buffer)
    :IOBuffer(buffer){}
    void clear();
    int read(istream&);
    int write(ostream&)const;
    int readHeader(istream&);
    int writeHeader(ostream&)const;
    int packFixedLength(void*,int);
    int packDelimited(void*,int);
    int packLength(void*,int);
    void print(ostream&) const;
    int sizeOfBuffer()const;
    int init();
};


#endif