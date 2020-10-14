#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

class Node {
public:
    bool isLeaf;
    vector<int> keys;
    Node* next;              
    union ptr {                
        vector<Node*> ptr2Tree; 
        vector<fstream*> dataPtr;  

        ptr();   
        ~ptr();  
    } ptr2TreeOrData;

    friend class BPTree; 
public:
    Node();
};

class BPTree {
private:
    int maxIntChildLimit;                                   
    int maxLeafNodeLimit;                                   
    Node* root;                                             
    void addInternal(int x, Node** cursor, Node** child); 
    Node** findParent(Node* cursor, Node* child);
    Node* firstLeftNode(Node* cursor);

public:
    string fileName;
    BPTree();
    BPTree(int degreeInternal, int degreeLeaf);
    Node* getRoot();
    void search(int key);

    void add(int key, fstream& stream);
};
