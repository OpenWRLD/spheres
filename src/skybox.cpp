//
//  skybox.cpp
//  world
//
//  Created by mzlt on 14/02/2018.
//

#include "skybox.hpp"


//--------------------------------------------------------------
skybox::skybox(){}

//--------------------------------------------------------------
void skybox::setup(){
    loadTex();
}

//--------------------------------------------------------------
void skybox::loadTex(){
    back.load("sky2/back2.png");
    right.load("sky2/right2.png");
    front.load("sky2/front2.png");
    left.load("sky2/left2.png");
    top.load("sky2/top.png");
    bottom.load("sky2/bottom2.png");
}

//--------------------------------------------------------------
void skybox::draw(ofVec3f origin, int size){
    // center around origin
    origin.x = origin.x - size / 2;
    origin.y = origin.y - size / 2;
    origin.z = origin.z - size / 2;
    
    ofVec3f lbb(origin.x, origin.y, origin.z); // left - bottom - back
    ofVec3f lbf(origin.x, origin.y, origin.z + size); // left - bottom - front
    ofVec3f ltb(origin.x, origin.y + size, origin.z); // left - top - back
    ofVec3f ltf(origin.x, origin.y + size, origin.z + size); // left - top - front
    ofVec3f rbb(origin.x + size, origin.y, origin.z); // right - bottom - back
    ofVec3f rbf(origin.x + size, origin.y, origin.z + size); // right - bottom - front
    ofVec3f rtb(origin.x + size, origin.y + size, origin.z); // right - top - back
    ofVec3f rtf(origin.x + size, origin.y + size, origin.z + size); // right - top - front
    
    
    skyFace(rbb, rtb, ltb, lbb, back); // back face
    skyFace(lbb, ltb, ltf, lbf, left); // left face
    skyFace(lbf, ltf, rtf, rbf, front); // front face
    skyFace(rbf, rtf, rtb, rbb, right); // right face
    skyFace(ltf, ltb, rtb, rtf, top); // top face
    skyFace(lbb, lbf, rbf, rbb, bottom); // bottom face
}


//--------------------------------------------------------------
void skybox::skyFace(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3, ofVec3f vert4, const ofImage& tex){
    tex.getTexture().bind();
    glBegin(GL_QUADS);

    // set texture coordinates and vector positionns
    glTexCoord2f(tex.getWidth(), tex.getHeight()); // right bottom
    glVertex3f(vert1.x, vert1.y, vert1.z);
    glTexCoord2f (tex.getWidth(), 0.0); // right top
    glVertex3f(vert2.x, vert2.y, vert2.z);
    glTexCoord2f (0.0, 0.0); // left bottom
    glVertex3f(vert3.x, vert3.y, vert3.z);
    glTexCoord2f(0.0, tex.getHeight()); // left top
    glVertex3f(vert4.x, vert4.y, vert4.z);

    glEnd();
    tex.getTexture().unbind();
}


