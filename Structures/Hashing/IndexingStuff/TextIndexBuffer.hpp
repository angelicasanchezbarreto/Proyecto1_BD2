#ifndef textindexbuffer_hpp
#define textindexbuffer_hpp

#include "../BufferStuff/FixedFieldBuffer.hpp"
#include "TextIndex.hpp"

class TextIndexBuffer : public FixedFieldBuffer{
    public:
    TextIndexBuffer(int keySize, int maxKeys=100,int extraFields=0, int extraSize=0);
    int pack(const TextIndex &);
    int unpack(TextIndex &);
    void print(ostream &) const;

    protected:
    int maxKeys;
    int keySize;
    char* dummy;
};

#endif