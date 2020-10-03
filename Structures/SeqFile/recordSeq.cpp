#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class RecordSeq{
    public:
        char codigo[5];
        char nombre[20]; //key
        char carrera[15];
        int ciclo;
        bool inAux=false;
        int pointerTo;

        void print(){
            string inAux;
            if(this->inAux) inAux = "AUX";
            else inAux = "MAIN";
            cout << codigo << " " << nombre << " " << carrera << " " << ciclo << " " << pointerTo << " " << inAux;
        }

        RecordSeq padding(){
            for (int i = 0; i < 4; i++){
                if (codigo[i] == '\0')
                    codigo[i] = ' ';
            }
            for (int i = 0; i < 19; i++){
                if (nombre[i] == '\0')
                    nombre[i] = ' ';
            }
            for (int i = 0; i < 14; i++){
                if (carrera[i] == '\0')
                    carrera[i] = ' ';
            }
            return *this;
        }

        void setPointer(int num){
            this->pointerTo = num;
        }

        void setWhichFile(bool isOnAux){
            this->inAux = isOnAux;
        }

        RecordSeq operator=(const RecordSeq &other) {
            strcpy(codigo,other.codigo);
            strcpy(nombre,other.nombre);
            strcpy(carrera,other.carrera);
            this->ciclo=other.ciclo;
            this->inAux=other.inAux;
            this->pointerTo=other.pointerTo;
            return *this;
        }

        bool operator==(const RecordSeq &other) {
            string s1(this->codigo),s2(other.codigo);
            string s3(this->nombre),s4(other.nombre);
            string s5(this->carrera),s6(other.carrera);
            return (s1==s2 && s3==s4 && s5==s6 && 
            this->ciclo==other.ciclo && 
            this->inAux==other.inAux && 
            this->pointerTo==other.pointerTo);
        }

        bool checkIfKeyEqual(const RecordSeq& record2){
            string s1(this->nombre),s2(record2.nombre);
            return (s1 == s2);
        }

        bool compareKey(string key){
            for(int i=0; i<key.length();){
                if(key[i]==this->nombre[i])
                    i++;
                else
                    return false;
            }
            return true;
        }

        void setNewValues(RecordSeq &other, int pointerTo, bool whichFile){
            *this=other;
            this->setPointer(pointerTo);
            this->setWhichFile(whichFile);
        }

        void find(ifstream &inFile, ifstream &inAuxFile, string key){
            inFile.read((char *) this, sizeof(RecordSeq));
            while (!(this->compareKey(key))) { 
                if(!this->inAux){
                    inFile.seekg(this->pointerTo*sizeof(RecordSeq));
                    inFile.read((char *) this, sizeof(RecordSeq));    
                } else {
                    inAuxFile.seekg(this->pointerTo*sizeof(RecordSeq));
                    inAuxFile.read((char *) this, sizeof(RecordSeq));
                }
            }  
        }
};