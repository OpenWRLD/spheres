//
//  cubesphere.hpp
//  world
//
//  Created by mzlt on 14/02/2018.
//

#ifndef cubesphere_hpp
#define cubesphere_hpp

#include <stdio.h>

#include "ofMain.h"


// code adapted from https://github.com/caosdoar/spheres/blob/master/src/spheres.cpp

class cubesphere {
    
public:
    
    cubesphere();
    void setup(ofVec3f mid, int levels);
    void drawSmooth();
    bool smooth;
    void drawFlat();
    
    void draw();
    
private:
    float r, g, b;
    int levels;
    vector<ofVec3f> vertices;
    vector<ofVec3f> norms;
    void addQuad(int a, int b, int c, int d);
    void addQuadAlt(int a, int b, int c, int d);
    void createOriginsRightsUps(vector<ofVec3f> &o,vector<ofVec3f> &r, vector<ofVec3f> &u);
    void createVertices(vector<ofVec3f> &o,vector<ofVec3f> &r, vector<ofVec3f> &u);
    ofVec3f calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3);
    void dr(void (cubesphere::*action)(int, int, int), cubesphere* obj);
    void tri(int i1, int i2, int i3);
    void triangle(int i1, int i2, int i3);
    

};


#endif /* cubesphere_hpp */
