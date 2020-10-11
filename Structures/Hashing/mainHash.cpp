#include "Directory.cpp"

class Directory;

int main(){
    int result;
    Directory dir(4);
    result = dir.create((char*)"hashfile");
    if(result==0){
        cout << "Please delete tsthash.dir and tsthash.bkt" << endl;
        return 0;
    }

    char* keys[]={(char*)"bill",(char*)"lee",(char*)"pauline",(char*)"alan",(char*)"julie",
                (char*)"mike",(char*)"elizabeth",(char*)"mark",(char*)"ashley",(char*)"peter",
                (char*)"joan",(char*)"john",(char*)"charles",(char*)"mary",(char*)"emily"};
    
    const int numKeys = 15;
    for(int i=0; i<numKeys; i++){
        cout << keys[i] << " " << hashFunc(keys[i]) << " " 
        << MakeAddress(keys[i],16) << endl;
        result = dir.insert(keys[i],100+i);
        if(result==0)
            cout <<"insert for " << keys[i] << " failed" << endl;
        dir.print(cout);
    }
    return 1;
}