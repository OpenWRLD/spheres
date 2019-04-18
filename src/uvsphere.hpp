//
//  uvsphere.hpp
//  world
//
//  Created by mzlt on 14/02/2018.
//

#ifndef uvsphere_hpp
#define uvsphere_hpp

#include <stdio.h>
#include "ofMain.h"

class uvsphere {
    
public:
    uvsphere();
    void setup(ofVec3f mid, int levels);
    void draw();
    bool smooth;
    
private:
    vector<ofVec3f> vr;
    vector<ofVec3f> norms;
    int levels;
    float r,g,b;
    void createVertices();
    void createVertexNormals();
    ofVec3f calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3);
    void dr(void (uvsphere::*action)(int, int, int), uvsphere* obj);
    void tri(int i1, int i2, int i3);
    void triangle(int i1, int i2, int i3);
    
    
    
    
    
    
    
   
    
    
};


#endif /* uvsphere_hpp */
