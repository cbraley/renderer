#ifndef OBJ_PARSER_H
#define OB_PARSER_H

#include <string>
#include <vector>
#include <iostream>
//--
#include <cstring>
#include <cstdlib>


/**
 *  Really simple code for parsing ONLY the polygon data from an wavelefront .obj file.
 *  Only supports polygons, and does NOT support relative indices.
 */
namespace ObjParser{

    class Vertex{
    public:
        float x,y,z,w;
        Vertex(float a, float b, float c, float d = 1.0f) : x(a),y(b),z(c),w(d) {}
        Vertex() : x(0.0f),y(0.0f),z(0.0f),w(0.0f) {}
        Vertex(const Vertex& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
        friend std::ostream& operator<<(std::ostream& os, const Vertex& v){
            os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
            return os;
        }
    };

    class Polygon{
    public:
        int N;
        Vertex* verts;

        //Takes ownership of the pointer.
        Polygon(int numVerts, Vertex* vdata) : N(numVerts), verts(vdata){}
        virtual ~Polygon(){ /*delete[] verts; verts = NULL;*/ }

        Vertex operator[](int idx)const{ return (verts)[idx]; }
        int getNumVerts()const{ return N; }

        friend std::ostream& operator<<(std::ostream& os, const Polygon& p){
            os << "[N = " << p.N << ", data = ";
            for(int i = 0; i < p.N; i++){
                os << p.verts[i] << "   ";
            }
            os << "]";
            return os;
        }
    };

    /**
     *  Parse an alias-wavefront .obj file.  Currently only parses a subset of .obj
     *  features, and fails when encountering relative indices.
     *
     *  @param file is the file path string
     *  @polys is a vector of polygons.  Data is returned through this parameter.
     *   Each polygon refers to data in the output array.  Once the output array is deallocated by the callee,
     *   the Polygon's in polys will point to junk data and can't be used.
     *  @return an array of Vertices.  This array should be freed by the caller, but once it is freed Polygons that refer
     *   to it will no longer be valid.
     *
     * Returns NULL to signify error conditions.
     */
    Vertex* parseObjFile(const std::string& file, std::vector<Polygon>& polys);


}


#endif //OBJ_PARSER_H
