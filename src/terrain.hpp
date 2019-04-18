//
//  terrain.hpp
//  world
//
//  Created by mzlt on 17/02/2018.
//

#ifndef terrain_hpp
#define terrain_hpp

#include <stdio.h>

#include "ofMain.h"

class terrain {
    
public:
    terrain();
    void setup(ofVec3f mid, int levels, bool sm);
    void draw();
    bool smooth;
    
private:
    vector<ofVec3f> vr;
    vector<ofVec3f> norms;
    int levels;
    void createVertices();
    void createVertexNormals();
    void triangle(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3);
    void tri(int i1, int i2, int i3);
    
    ofVec3f calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3);

    float vis;
    float dens = 0.015;
    float grad = 1.5;
};

#endif /* terrain_hpp */
