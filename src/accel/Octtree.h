#ifndef OCTTREE_H
#define OCTTREE_H

#include <iostream>
#include <cassert>
#include <stack>
//--
#include "utils/Assert.h"

template<typename T>
class Octtree{
public:

    Octtree(int maxTreeDepth = 256, int maxPrimsPerNode = 32);

    virtual ~Octtree();


private:

    /**
     *  Octtree node structure.
     */
    typedef struct OctVox{
    private:
        OctVox* children;     //Child pointers in Western reading order, then front to back(8)
        int numData;         //How many data elements here? -1 for leaves
        T* data;             //Array of data, NULL for non-leaf nodes
    public:

        /// \brief Construct an internal node.
//        OctVox(OctVox* childPtr) : children(childPtr), numData(-1), data(NULL)
//        {
//           Assert(children != NULL);
//        }

        /// \brief Construct a leaf node with no data.
        OctVox() :  numData(0), data(NULL){
            for(int i = 0; i < 8; i++){
                children[i] = NULL;
            }
        }

        inline bool isLeaf()const    { return numData == -1; }
        inline bool isInternal()const{ return numData != -1; }

        inline int getNumData()const{
            Assert(isLeaf());
            return numData;
        }
        inline T* getData(int index)const{
            Assert(isLeaf() && index >= 0 && index < getNumData());
            return data[index];
        }

        inline OctVox* getChild(int index)const{
            Assert(isInternal() && index >= 0 && index < 8);
            return children[index];
        }

        inline void setChild(int index, OctVox* newKid)const{
            Assert(isInternal() && index >= 0 && index < 8);
            children[index] = newKid;
        }

        inline ~OctVox(){
            if(isLeaf()){
                delete data; data = NULL;
            }
        }


    private:
        OctVox(const OctVox& other);
        OctVox& operator=(const OctVox& rhs);
    }OctVox;

    OctVox* root;
    int numNodes;
    int maxDepth;
    int maxPrims;

    Octtree(const Octtree& other);
    Octtree& operator=(const Octtree& rhs)const;

};

template<typename T>
Octtree<T>::Octtree(int maxTreeDepth, int maxPrimsPerNode) : 
    root(NULL), numNodes(0), maxDepth(maxTreeDepth), maxPrims(maxPrimsPerNode)
{
    Assert(maxDepth > 0);
    Assert(maxPrims > 0);
    root = new OctVox();
    numNodes = 1;
}

template<typename T>
Octtree<T>::~Octtree(){

    std::stack<OctVox*> delStack;
    delStack.push(root);

    while(! delStack.empty()){
        OctVox* curr = delStack.top();
        delStack.pop();

        if(curr->isLeaf()){

        }else{

        }
    }
}


#endif //OCTTREE_H
