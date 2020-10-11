#ifndef buffile_hpp_
#define buffile_hpp_

#include <iostream>
#include <string>
#include "Iobuffer.cpp"

class BufferFile{
    public:
    BufferFile(IOBuffer &);
    int openBuffer(string filename, int mode);
    int createBuffer(string filename, int mode);
    int closeBuffer();
    int rewindBuffer();
    int readBuffer(int recordAddress=-1);
    int writeBuffer(int recordAddress=-1);
    int appendBuffer(); //at the end of
    IOBuffer& getBuffer();
    
    protected:
    IOBuffer& Buffer;
    fstream file;
    int headerSize;
    int readHeader();
    int writeHeader();
};

#endif