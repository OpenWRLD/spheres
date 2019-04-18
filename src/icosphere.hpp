//
//  icosphere.hpp
//  world
//
//  Created by mzlt on 13/02/2018.
//

#ifndef icosphere_hpp
#define icosphere_hpp

#include "ofMain.h"


// code adapted from c# from http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

class icosphere {
    
public:
    
    icosphere();
    void setup(ofVec3f mid, int levels);
    void draw();
    bool smooth;
    void drawSmooth();
    void drawFlat();
    
private:
    float r, g, b;
    int index;
    int addV(ofVec3f v);
    int getMiddlePoint(int p1, int p2);
    vector<ofVec3f> vs;
    vector<ofVec3f> faces;
    vector<ofVec3f> norms;
    unordered_map<long, int> middlePointIndexCache;
    void createVertices();
    void createFaces();
    void refineFaces(int &levels);
    void createVertexNormals();
    ofVec3f calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3);
    void dr(void (icosphere::*action)(int, int, int), icosphere* obj);
    void tri(int i1, int i2, int i3);
    void triangle(int i1, int i2, int i3);
    
    


};


#endif /* icosphere_hpp */
