#ifndef lengthfieldbuffer_hpp
#define lengthfieldbuffer_hpp

#include <cstring>
#include <iostream>
#include <fstream>
#include "VariableLengthBuffer.cpp"

using namespace std;

class LengthFieldBuffer : public VariableLengthBuffer{
    public:
    LengthFieldBuffer(int maxBytes=1000);
    LengthFieldBuffer(const LengthFieldBuffer& buffer)
    :VariableLengthBuffer(buffer){}
    void clear();
    int pack(const void* field, int size=-1);
    int unpack(void* field, int maxBytes=-1);
    void print(ostream&) const;
    int init();
};


#endif