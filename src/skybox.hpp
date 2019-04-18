//
//  skybox.hpp
//  world
//
//  Created by mzlt on 14/02/2018.
//

#ifndef skybox_hpp
#define skybox_hpp

#include <stdio.h>

#include "ofMain.h"

class skybox {
    
public:
    
    skybox();
    void setup();
    void draw(ofVec3f origin, int size);
    
    
private:
    // texture from https://93i.de/p/free-skybox-texture-set/
    ofImage back;
    ofImage right;
    ofImage front;
    ofImage left;
    ofImage top;
    ofImage bottom;
    void skyFace(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3, ofVec3f vert4, const ofImage& tex);
    void loadTex();

    
};


#endif /* skybox_hpp */
