#ifndef hash_hpp
#define hash_hpp

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int hashFunc(char *key){
    int sum=0;
    int len=strlen(key);
    if(len%2==1) len++;
    for(int j=0; j<len; j+=2)
        sum = (sum+100*key[j]+key[j+1])%19937;
    return sum;
}

int MakeAddress(char* key, int depth){
    int retval=0;
    int mask=1;
    int hashVal = hashFunc(key);

    for(int i=0; i<depth; i++){
        retval = retval<<1;
        int lowbit = hashVal&mask;
        retval = retval|lowbit;
        hashVal = hashVal>>1;
    }
    return retval;
}

#endif