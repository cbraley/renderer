#include "gui/ObjParser.h"

#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
//--
#include "utils/StringUtils.h"

namespace ObjParser{

Vertex* parseObjFile(const std::string& file, std::vector<Polygon>& polys){

    std::fstream inFile(file.c_str(), std::fstream::in);
    if(!inFile){ return NULL; }

    const std::string WHITESPACE(" \t\n\r\n");
    const std::string VERTEX("v");
    const std::string NORMAL("vn");
    const std::string FACE("f");

    std::string line;
    std::vector<Vertex> verts;
    std::vector< std::vector<int> > indexList;
    while(getline(inFile, line)){

        std::vector<std::string> toks =
            StringUtils::tokenize(line,
            WHITESPACE);
        if(toks.size() == 0) continue;

        if(toks.size() > 0 && toks[0][0] != '#' ){
            if(toks[0] == VERTEX && toks.size() >= 4){ //can have x,y,z,w
                const float x = atof(toks[1].c_str());
                const float y = atof(toks[2].c_str());
                const float z = atof(toks[3].c_str());
                const float w = toks.size() >= 5 ? atof(toks[4].c_str()) : 1.0f;
                verts.push_back(Vertex(x,y,z,w));
            }else if(toks[0] == NORMAL && toks.size() == 4){
            }else if(toks[0] == FACE && toks.size() >= 2){
                std::vector<int> indices;
                for(size_t j = 1; j < toks.size(); j++){
                    const int currIndex = atoi(toks[j].c_str());
                    if(currIndex > 0){
                        indices.push_back(currIndex - 1);
                    }
                }
                indexList.push_back(indices);
            }else{
            }
        }
    }

    //Make our return data structures
    Vertex* retVerts = new Vertex[verts.size()];
    for(size_t i = 0; i < verts.size(); i++){
        retVerts[i] = Vertex(verts[i]);
    }
    for(size_t i = 0; i < indexList.size(); i++){
        if(indexList[i].size() < 2) { continue; } //Skip degenerate data
        
        Vertex* vl = new Vertex[indexList[i].size()]; 
        for(size_t j = 0; j < indexList[i].size(); j++){
            const int index = indexList[i][j];
            Vertex v = retVerts[ index ];
            vl[j] = v;
        }
        polys.push_back(Polygon(indexList[i].size() ,vl));

    }

    inFile.close();
    return retVerts;
}



}
