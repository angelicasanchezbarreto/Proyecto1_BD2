#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <queue>
#include <cstring>

#include "../B+Tree/B+Tree.hpp"
#include "../Record/Record.cpp"

void insertionMethod(BPTree** bPTree) {
    vector<string> data;
    string dato;
    int num;

    cout << "Inserta el codigo de la bodega: ";
    cin >> dato;
    num = stoi(dato);
    data.push_back(dato);

    cout << "Inserta el nombre de la bodega: ";
    cin >> dato;
    data.push_back(dato);

    cout << "\nInserta el distrito: ";
    cin >> dato;
    data.push_back(dato);

    cout << "\nInserta el aforo: ";
    cin >> dato;
    data.push_back(dato);

    cout << "\nInserta la cantidad de productos: ";
    cin >> dato;
    data.push_back(dato);

    for(int i=1; i<=4; i++){
        cout << "\nInserta las ventas de la semana " << i;
        cin >> dato;
        data.push_back(dato);
    }

    Record rec(data);
    string fileName = "data/";
    fileName += to_string(num) + ".dat";

    fstream* filePtr = new fstream;
    filePtr->open(fileName, ios::in | ios::out | ios::binary);
    filePtr->write((char *) &rec,sizeof(Record));
    (*bPTree)->add(num, *filePtr);
    filePtr->close();
}

void searchMethod(BPTree* bPTree) {
    int codigo;
    cout << "Cuál es el código de la bodega a buscar? ";
    cin >> codigo;
    bPTree->search(codigo);
}

void BPTree::add(int key, fstream& filePtr) {

    if (root == NULL) {
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new (&root->ptr2TreeOrData.dataPtr) std::vector<fstream>;
        root->ptr2TreeOrData.dataPtr.push_back(&filePtr);

        return;
    } else {
        Node* cursor = root;
        Node* parent = NULL;
        while (cursor->isLeaf == false) {
            parent = cursor;
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
        }

        if (cursor->keys.size() < maxLeafNodeLimit) {

            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor->keys.push_back(key);
            cursor->ptr2TreeOrData.dataPtr.push_back(&filePtr);

            if (i != cursor->keys.size() - 1) {
                for (int j = cursor->keys.size() - 1; j > i; j--) {  
                    cursor->keys[j] = cursor->keys[j - 1];
                    cursor->ptr2TreeOrData.dataPtr[j] = cursor->ptr2TreeOrData.dataPtr[j - 1];
                }

                cursor->keys[i] = key;
                cursor->ptr2TreeOrData.dataPtr[i] = &filePtr;
            }
        } else {
            vector<int> virtualNode(cursor->keys);
            vector<fstream*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            virtualNode.push_back(key);          
            virtualDataNode.push_back(&filePtr);  

            if (i != virtualNode.size() - 1) {
                for (int j = virtualNode.size() - 1; j > i; j--) {  
                    virtualNode[j] = virtualNode[j - 1];
                    virtualDataNode[j] = virtualDataNode[j - 1];
                }

                virtualNode[i] = key;
                virtualDataNode[i] = &filePtr;
            }

            Node* newLeaf = new Node;
            newLeaf->isLeaf = true;
            new (&newLeaf->ptr2TreeOrData.dataPtr) std::vector<fstream*>;

            Node* temp = cursor->next;
            cursor->next = newLeaf;
            newLeaf->next = temp;

            cursor->keys.resize((maxLeafNodeLimit) / 2 +1);
            cursor->ptr2TreeOrData.dataPtr.reserve((maxLeafNodeLimit) / 2 +1);
            for (int i = 0; i <= (maxLeafNodeLimit) / 2; i++) {
                cursor->keys[i] = virtualNode[i];
                cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
            }

            for (int i = (maxLeafNodeLimit) / 2 + 1; i < virtualNode.size(); i++) {
                newLeaf->keys.push_back(virtualNode[i]);
                newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[i]);
            }

            if (cursor == root) {


                Node* newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
                root = newRoot;
            } else {
                addInternal(newLeaf->keys[0], &parent, &newLeaf);
            }
        }
    }
}

void BPTree::addInternal(int x, Node** cursor, Node** child) {
    if ((*cursor)->keys.size() < maxIntChildLimit - 1) {

        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        (*cursor)->keys.push_back(x);
        (*cursor)->ptr2TreeOrData.ptr2Tree.push_back(*child);

        if (i != (*cursor)->keys.size() - 1) { 

            for (int j = (*cursor)->keys.size() - 1; j > i; j--) { 
                (*cursor)->keys[j] = (*cursor)->keys[j - 1];
            }

            for (int j = (*cursor)->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--) {
                (*cursor)->ptr2TreeOrData.ptr2Tree[j] = (*cursor)->ptr2TreeOrData.ptr2Tree[j - 1];
            }

            (*cursor)->keys[i] = x;
            (*cursor)->ptr2TreeOrData.ptr2Tree[i + 1] = *child;
        }
    } else { 
        vector<int> virtualKeyNode((*cursor)->keys);
        vector<Node*> virtualTreePtrNode((*cursor)->ptr2TreeOrData.ptr2Tree);

        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin(); 
        virtualKeyNode.push_back(x);                                                                   
        virtualTreePtrNode.push_back(*child);                                                

        if (i != virtualKeyNode.size() - 1) {
            for (int j = virtualKeyNode.size() - 1; j > i; j--) {  
                virtualKeyNode[j] = virtualKeyNode[j - 1];
            }

            for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
                virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
            }

            virtualKeyNode[i] = x;
            virtualTreePtrNode[i + 1] = *child;
        }

        int partitionKey;                                            
        partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];  
        int partitionIdx = (virtualKeyNode.size() / 2);

        (*cursor)->keys.resize(partitionIdx);
        (*cursor)->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
        (*cursor)->ptr2TreeOrData.ptr2Tree.reserve(partitionIdx + 1);
        for (int i = 0; i < partitionIdx; i++) {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }

        for (int i = 0; i < partitionIdx + 1; i++) {
            (*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
        }

        Node* newInternalNode = new Node;
        new (&newInternalNode->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;

        for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }

        for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {  
            newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
        }

        if ((*cursor) == root) {
            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);

            root = newRoot;
        } else {
            addInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}


void BPTree::search(int key) {
    if (root == NULL) {
        return;
    } else {
        Node* cursor = root;
        while (cursor->isLeaf == false) {
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
        }

        int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

        if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
            return;
        }
        Record rec;
        vector<Record> recs;
        string fileName = "data/";
        string data;
        fileName += to_string(key) + ".dat";
        fstream* filePtr = new fstream;
        filePtr->open(fileName, ios::in | ios::out | ios::binary);
        while (!filePtr->eof()) {
            filePtr->read((char*) &rec, sizeof(Record));
        }
        filePtr->close();
        //return recs;
    }
}

Node* parent = NULL;

Node::Node() {
    this->isLeaf = false;
    this->next = NULL;
}

BPTree::BPTree() {
    this->maxIntChildLimit = 4;
    this->maxLeafNodeLimit = 3;
    this->root = NULL;
}

BPTree::BPTree(int degreeInternal, int degreeLeaf) {
    this->maxIntChildLimit = degreeInternal;
    this->maxLeafNodeLimit = degreeLeaf;
    this->root = NULL;
}

Node* BPTree::getRoot() {
    return this->root;
}

Node* BPTree::firstLeftNode(Node* cursor) {
    if (cursor->isLeaf)
        return cursor;
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
        if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
            return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);

    return NULL;
}

Node** BPTree::findParent(Node* cursor, Node* child) {
    if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf)
        return NULL;

    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
            parent = cursor;
        } else {
            Node* tmpCursor = cursor->ptr2TreeOrData.ptr2Tree[i];
            findParent(tmpCursor, child);
        }
    }

    return &parent;
}

