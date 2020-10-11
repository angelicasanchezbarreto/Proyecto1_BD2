#ifndef fixedfieldbuffer_hpp_
#define fixedfieldbuffer_hpp_

#include "FixedLengthBuffer.cpp"

class FixedFieldBuffer : public FixedLengthBuffer{
    public:
    FixedFieldBuffer(int maxFields, int recordSize=1000);
    FixedFieldBuffer(int maxFields, int *fieldSize);
    FixedFieldBuffer(const FixedFieldBuffer&);
    FixedFieldBuffer& operator = (const FixedFieldBuffer&);
    void clear();
    int addField(int fieldSize);
    int readHeader(istream&);
    int writeHeader(ostream&) const;
    int pack(const void* field, int size=-1);
    int unpack(void* field, int maxBytes=-1);
    void print(ostream&) const;
    int numberOfFields() const;
    int init(int maxFields);
    int init(int numFields, int* fieldSize);

    protected:
    int* fieldSize;
    int maxFields;
    int numFields;
    int nextField;
};

#endif