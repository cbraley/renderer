#ifndef BVH_H
#define BVH_H

#include <iostream>
#include <stack>
#include <vector>
#include <functional>
//--
#include "accel/AccelStructure.h"
#include "geom/BoundingBox.h"
#include "primitives/Shape.h"
#include "utils/Constants.h"
#include "utils/Assert.h"

//TODO: Experiment with making the indices uchar's to fit more into a cache line
//TODO: Flatten the tree somehow

/**
 *  Bounding volume heierarchy (BVH) class.
 */
class BVH : public AccelStructure{
public:

    enum PartitionStrategy{SPLIT_CENTER, SPLIT_SAH};
    BVH(PartitionStrategy strategy, size_t primitivesPerLeaf = 64);

    virtual void buildAccelStructure(const std::vector<Shape*>& sceneData);
    
    virtual RaySurfIntersection getClosestIntersection(const Ray& r, bool& hitAnything)const;
    virtual bool doesIntersect(const Ray& r,
        float minDist = 0.0f, float maxDist = Constants::MAX_FLOAT_VAL,
        const Shape* ignoreShape = NULL)const;
    
    virtual size_t getNumShapes()const;
    virtual const Shape* getShape(size_t idx)const;

    virtual ~BVH();

private:
    BVH();
    BVH(const BVH& other);

    typedef struct ShapeWBound{
        Shape* shape;
        BoundingBox bbox;
        Point centroid;
        ShapeWBound(Shape* shp, const BoundingBox& aabb);
    }ShapeWBound;

    std::vector<ShapeWBound> scene; //Array of pointers to scene primitives and associated AABB's
    PartitionStrategy strat; //How are we doing splits?
    size_t primsPerLeaf; //How many primitives should we store per leaf node?


    /// \brief List of indices into an array of primitives
    typedef struct PrimList{
        size_t numPrims;
        size_t* indices; //Array of length "numPrims"
    }PrimList;


    PrimList makePrimList(
        std::vector<BVH::ShapeWBound>& primList,
        std::vector<ShapeWBound>::iterator& listBegin,
        std::vector<ShapeWBound>::iterator& listEnd);

    //Tree node class for BVH binary tree
    typedef struct BVHNode{
        BoundingBox bbox; //Bounding box for this level
        bool isLeaf; //Is this a leaf node or not?
       
        BVHNode(const BoundingBox& boundingBox, bool isLeafNode);
        
        void setRightChild(BVHNode* node);
        void setLeftChild (BVHNode* node);

        //In this union, the "children" data is only valid if the node is NOT a leaf node
        //if the node is a leaf node, "primList" is valid
        union{
            BVHNode* children[2]; //Only valid iff the node is non-leaf
            PrimList prims; //Only valid iff the node is a leaf
        };

        ~BVHNode();

        BVHNode* getLeftChild()const;
        BVHNode* getRightChild()const;

        size_t getNumPrims()const;
        Shape* getPrim(size_t index, const std::vector<Shape*>& primVec)const;
    }BVHNode;

    BVHNode* root; //Root of tree; NULL if we have not built yet
    BVHNode* buildTreeRecursive(int level,
        std::vector<ShapeWBound>::iterator startIncl,
        std::vector<ShapeWBound>::iterator endIncl ); //Recursive helper for building trees

    //Helper function used by std::accumulate to do a "fold" operation from functional programming
    //unfortunately this has to be a static member function in order to use the private class ShapeWBound
    //This allows us to compute unions of many bounding boxes in a single line with std::accumulate
    static BoundingBox unionBBoxes(const BoundingBox& a, const BVH::ShapeWBound& b){
        BoundingBox boxB = b.bbox;
        BoundingBox boxA = a;
        boxA.Union(boxB);
        return boxA;
    }

    //Helper functor - We use this instead of a function pointer since we need to
    //keep track of state(the current BoundingBox, bbox)
    struct InBBoxTester : public std::unary_function<BVH::ShapeWBound,bool>{
        BoundingBox bbox;
        InBBoxTester(const BoundingBox& box) : bbox(box) {}
        bool operator()(const BVH::ShapeWBound& shp){
            return shp.shape->inside(bbox);
        }
    };

    struct BBoxCentroidSorter :
        public std::binary_function<BVH::ShapeWBound, BVH::ShapeWBound, bool>
    {
        enum CompareAxis{AXIS_X, AXIS_Y, AXIS_Z};
        CompareAxis axis;
        
        BBoxCentroidSorter(CompareAxis sortAxis) : 
            axis(sortAxis) {}

        BBoxCentroidSorter(size_t sortAxis){
            Assert(sortAxis >= 0 && sortAxis <= 2);
            if(      sortAxis == 0){
                axis = AXIS_X;
            }else if(sortAxis == 1){
                axis = AXIS_Y;
            }else if(sortAxis == 2){
                axis = AXIS_Z;
            }
        }

        bool operator()(const BVH::ShapeWBound& a, const BVH::ShapeWBound& b){
            if(axis       == AXIS_X){
                return a.centroid.x < b.centroid.x;
            }else if(axis == AXIS_Y){
                return a.centroid.y < b.centroid.y;
            }else if(axis == AXIS_Z){
                return a.centroid.z < b.centroid.z;
            }else{
                Assert(false);
            }
        }
    };


};

//Inline function definitions for BVH itself ----------------------------------
inline size_t BVH::getNumShapes()const{
    Assert(root != NULL);
    return scene.size(); 
}
inline const Shape* BVH::getShape(size_t idx)const{
    Assert(root != NULL);
    Assert(idx < scene.size());
    return scene[idx].shape;
}

//Inline function definitions for BVHNode--------------------------------------
inline BVH::BVHNode::BVHNode(const BoundingBox& boundingBox, bool isLeafNode) : 
    bbox(boundingBox), isLeaf(isLeafNode)
{}

inline BVH::BVHNode* BVH::BVHNode::getLeftChild()const{
    Assert(! isLeaf);
    return children[0]; 
}

inline BVH::BVHNode* BVH::BVHNode::getRightChild()const{
    Assert(! isLeaf);
    return children[1]; 
}

inline size_t BVH::BVHNode::getNumPrims()const{
    Assert(isLeaf);
    return prims.numPrims;
}

inline Shape* BVH::BVHNode::getPrim(size_t index, const std::vector<Shape*>& primVec)const{
    Assert(isLeaf);
    Assert(index < getNumPrims() );
    return primVec[ prims.indices[index] ];
}

inline void BVH::BVHNode::setLeftChild (BVHNode* node){
    Assert(!isLeaf);
    children[0] = node;
}

inline void BVH::BVHNode::setRightChild(BVHNode* node){
    Assert(!isLeaf);
    children[1] = node;
}

//Inline function definitions for ShapeWBound----------------------------------
inline BVH::ShapeWBound::ShapeWBound(Shape* shp, const BoundingBox& aabb) : 
    shape(shp), bbox(aabb)
{
    Assert(shape != NULL);
    centroid = aabb.centroid();
}
 

#endif //BVH_H
