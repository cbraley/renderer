#include "accel/BVH.h"
//--
#include <algorithm>
#include <iterator>
#include <numeric>

//Preprocessor directives controlling behavior @ compile time-----------------

//Implementation of BVHNode----------------------------------------------------

BVH::BVHNode::~BVHNode(){
    if(isLeaf){
        //Delete primitive index array
        delete[] prims.indices; 
        prims.indices = NULL; 
    }else{
        //Delete both left and right nodes
        delete children[0];
        children[0] = NULL;
        delete children[1];
        children[1] = NULL;
    }
}

//Implementation of PrimList--------------------------------------------------

BVH::PrimList BVH::makePrimList(
    std::vector<BVH::ShapeWBound>& primList,
    std::vector<BVH::ShapeWBound>::iterator& listBegin,
    std::vector<BVH::ShapeWBound>::iterator& listEnd)
{
    PrimList ret;
    ret.numPrims = std::distance(listBegin, listEnd);
    
    //There can be zero primitives in the case where we are using a simple
    //split strategy and then all the primitives fall on one side
    Assert(ret.numPrims >= 0);

    //Allocate data for array of size_t's storing primitive indices
    if(ret.numPrims == 0){
        ret.indices = NULL;
        //This is OK since later on delete[] NULL is OK
    }else{
        ret.indices = new size_t[ret.numPrims];

        //Copy in the indices
        std::vector<BVH::ShapeWBound>::iterator firstElemItr = primList.begin();
        const size_t offset = std::distance(listBegin, firstElemItr);
        for(size_t i = 0; i < ret.numPrims; i++){
            ret.indices[i] = offset + i;
        }
    }
}

//Implementation of BVH-------------------------------------------------------

static Point computeAvgBBoxCentroid(
    std::vector<BVH::ShapeWBound>::iterator startIncl,
    std::vector<BVH::ShapeWBound>::iterator endIncl )
{
    //Add toghether all the points
    Point accum(0.0f, 0.0f, 0.0f);
    std::vector<BVH::ShapeWBound>::iterator iter = startIncl;
    while(iter <= endIncl){
        accum += (iter++)->centroid;
    }
    
    //Divide by the number of points
    const size_t numPoints = std::distance(startIncl, endIncl);
    const float mul = 1.0f / ((float)numPoints);
    return Point(accum.x * mul, accum.y * mul, accum.z * mul);
}


BVH::BVHNode* BVH::buildTreeRecursive(int level,
    std::vector<BVH::ShapeWBound>::iterator startIncl,
    std::vector<BVH::ShapeWBound>::iterator endIncl )
{
    std::cout << "btr level: " << level << std::endl;

    //We need to bound the primitives in the range [startIncl, endIncl]
    //within the std::vector scene
    //if there are <= primsPerLeaf primitives in this range, then we create a leaf node
    const int numPrimsInRange = std::distance(startIncl, endIncl);
    std::cout << "\tnum prims = " << numPrimsInRange << std::endl;
    Assert(numPrimsInRange >= 0);
    const bool makingLeaf = numPrimsInRange <= primsPerLeaf;

    //Compute a bounding box for all the primitives in this node
    //Note that here I use "std::accumulate," which I think should be 
    //called "std::fold" for the functional programming "fold" function
    //found in languages like ML
    BoundingBox bbox;
    if(numPrimsInRange != 0){
        bbox = std::accumulate(startIncl, endIncl, bbox, BVH::unionBBoxes);
    }
    std::cout << "\tbounding box: " << bbox << std::endl;

    //If no primitives are in range that indicates an edge case in which all the
    //primitives are in the other subtree and we should just build a degenerate 
    //bounding box and make it a leaf.
    
    std::vector<BVH::ShapeWBound>::iterator leftStart, rightStart, leftEnd, rightEnd;
 
    if(makingLeaf){ //Making a leaf node
        std::cout << "Making a leaf." << std::endl;
        //Construct leaf
        BVH::BVHNode* newLeaf = new BVH::BVHNode(bbox, true);
        //Add data to the leaf
        //the data is a list of integers, where each integer is an index into 
        //the array of primitives
        newLeaf->prims = makePrimList(scene, startIncl, endIncl);
        std::cout << "Made a leaf." << std::endl;
        return newLeaf;
    }else{ //Making an internal node
        //Allocate new node
        BVH::BVHNode* newInternal = new BVH::BVHNode(bbox, false);
        
        //Split the node based on the chosen split strategy
        BoundingBox leftBBox, rightBBox;
        if(strat == BVH::SPLIT_SAH){ //SAH = "Surface area heuristic"
            Assert(false);
            return NULL;//TODO: Implement
        }else if(strat == BVH::SPLIT_CENTER){ //Split down the center
            
            //We need to choose an axis to split on: X,Y, or Z
            //we choose based on the level in the tree
            const size_t axis = level % 3;
            std::cout << "\taxis = " << axis << std::endl;
            BBoxCentroidSorter sorter(axis);

            //Find the item that would take the nth position in 
            //the array in the event that it was sorted
            //Note that this is faster than the O(n log(n))
            //from naive sorting
            std::vector<BVH::ShapeWBound>::iterator midIter;
            
            //TODO: What should miditer be before this call?
            midIter = startIncl + (numPrimsInRange/2);

            std::nth_element(startIncl, midIter, endIncl, sorter);
            std::cout << "\tcompleted the partition."  << std::endl;

            //Compute bounding boxes for each subtree
            /*
            std::cout << "\tBEGIN TEST CALLS." << std::endl;
            BVH::ShapeWBound t1 = *startIncl;
            std::cout << "\tstart bbox = " << t1 << std::endl;
            BVH::ShapeWBound t2 = *midIter;
            std::cout << "\tmid bbox = " << t2 << std::endl;
            BVH::ShapeWBound t3 = *endIncl;
            std::cout << "\tend bbox = " << t3 << std::endl;
            std::cout << "\tEND TEST CALLS." << std::endl;
            */

            BoundingBox leftBBox =
                std::accumulate(startIncl, midIter-1, BoundingBox(), BVH::unionBBoxes);
            std::cout << "\tleft subtree bbox = " << leftBBox << std::endl;
            BoundingBox rightBBox =
                std::accumulate(midIter  , endIncl  , BoundingBox(), BVH::unionBBoxes);
            std::cout << "\tright subtree bbox = " << rightBBox << std::endl;
            std::cout << "\tcomputed subtree bounding boxes."  << std::endl;
            
            //Partition the resulting objects into "those in the left subtree"
            //and "those in the right subtree"
            BVH::InBBoxTester bboxCheck(leftBBox);
            std::vector<BVH::ShapeWBound>::iterator bound =
                std::partition(startIncl, endIncl, bboxCheck);

            //Make sure that each primitive is in one of the two bounding boxes
            //TODO: Do this, but only in debug mode

        }else{ //We did not consider some strategy! (Should never happen)
            Assert(false);
            return NULL;
        }


        //Partition the primitives into each node

        //Recursively create children
        std::cout << "\tabout to make 2 recursive calls." << std::endl;
        newInternal->setLeftChild (
            buildTreeRecursive(level + 1, leftStart , leftEnd ));
        newInternal->setRightChild(
            buildTreeRecursive(level + 1, rightStart, rightEnd));
        return newInternal;
    }
}

BVH::BVH(PartitionStrategy strategy, size_t primitivesPerLeaf) : 
    strat(strategy), primsPerLeaf(primitivesPerLeaf), root(NULL)
{
    Assert(primsPerLeaf > 0);
}

void BVH::buildAccelStructure(const std::vector<Shape*>& sceneData){
    Assert(scene.empty());

    //Get bounding boxes for each shape
    scene.reserve(sceneData.size()); //Reserve space ahead of time to avoid realloc's
    for(size_t i = 0; i < sceneData.size(); i++){
        //Bound current primitive
        BoundingBox currBBox = sceneData[i]->worldSpaceBound();
        //Add bbox and shape to list
        BVH::ShapeWBound curr(sceneData[i], currBBox);
        scene.push_back(curr);
    }

    //Build tree recursively in a top down manner
    root = buildTreeRecursive(0, scene.begin(), scene.end());
}

RaySurfIntersection BVH::getClosestIntersection(const Ray& r, bool& hitAnything)const{
    RaySurfIntersection hit;
    Assert(false);
    return hit;
}

bool BVH::doesIntersect(const Ray& r, float minDist, float maxDist, const Shape* ignoreShape)const{
    Assert(false);
    return false;
}


BVH::~BVH(){
    delete root; //This is OK since delete NULL is fine
}


void BVH::traverseHelper(BVH::BVHVisitor* visitor, int level,
    BVHNode* currRoot)
{
    Assert(visitor != NULL);
    if(currRoot == NULL){
        return;
    }else{
        const bool isLeaf = currRoot->isLeaf;
        
        if(!isLeaf){
            traverseHelper(visitor, level + 1, currRoot->getLeftChild());
        }

        visitor->visit(
            isLeaf,
            level,
            currRoot->bbox,
            isLeaf ? currRoot->getNumPrims() : 0
            );

        if(!isLeaf){
            traverseHelper(visitor, level + 1, currRoot->getRightChild());
        }
    }
}

void BVH::traverse(BVH::BVHVisitor* visitor){
    traverseHelper(visitor, 0, root);
}



BVH::PrintVisitor::PrintVisitor(std::ostream& stream) : 
    os(stream)
{
}

void BVH::PrintVisitor::visit(bool isLeaf, int level, const BoundingBox& aabb,
    size_t numPrims)
{
    //Print N spaces, where N is the current traversal depth
    for(int i = 0; i < level; i++){
        os << " ";
    }

    if(isLeaf){
        os << level << ": " << "leaf, " << numPrims << " primitives." << std::endl;
    }else{
        os << level << ": " << aabb << std::endl;
    }
}

