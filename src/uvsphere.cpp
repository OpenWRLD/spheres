//
//  uvsphere.cpp
//  world
//
//  Created by mzlt on 14/02/2018.
//

#include "uvsphere.hpp"

//--------------------------------------------------------------
uvsphere::uvsphere(){}

//--------------------------------------------------------------
void uvsphere::setup(ofVec3f mid, int complexity){
    smooth = false;
    levels = complexity;
    createVertices();
    for(int i = 0; i < vr.size(); i++) vr[i] += mid;
    createVertexNormals();
    r = ofRandom(0.1,1);
    g = ofRandom(0.1,1);
    b = ofRandom(0.1,1);
}

//--------------------------------------------------------------
void uvsphere::draw(){
    glBegin(GL_TRIANGLES);
    
    GLfloat mat_ambient[]={ r*0.25f, r*0.25f, r*0.25f, 1.0f };
    GLfloat mat_diffuse[]={r, g, b, 1.0f };
    GLfloat mat_specular[]={0.296648f, 0.296648f, 0.296648f, 1.0f };
    GLfloat shine = ofMap(11.264f , 0, 128, 0, 1);
    
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
    
    if (smooth) dr(&uvsphere::tri, this);
    else dr(&uvsphere::triangle, this);

    
    
     glEnd();
    
}

//--------------------------------------------------------------
void uvsphere::dr(void (uvsphere::*action)(int, int, int) , uvsphere* obj) {
    
    //top row triangle fan
    for (int j=1; j<levels; j++) {
        (obj->*action)(0,j,j+1);
    }
    (obj->*action)(0, levels, 1);
    
    //triangle strips
    for (int i=0; i < levels-2; i++) {
        int top = i * levels + 1;
        int bottom = (i + 1) * levels + 1;
        for (int j = 0; j < levels - 1; j++) {
            (obj->*action)(bottom + j, bottom + j + 1, top + j + 1);
            (obj->*action)(bottom + j, top + j + 1, top + j);
        }
        (obj->*action)(bottom + levels - 1, bottom, top);
        (obj->*action)(bottom + levels - 1, top, top + levels - 1);
    }
    
    //bottom row triangle fan
    int last = vr.size()-1;
    for (int j = last - 1; j > last - levels; j--) {
        (obj->*action)(last, j, j - 1);
    }
    (obj->*action)(last,last-levels,last-1);
    
}

//--------------------------------------------------------------
void uvsphere::createVertices(){
    vr.push_back(ofVec3f(0,0,1));
    for (int i = 1; i < levels; i++) {
        double phi = PI * double(i)/levels;
        double cosPhi = cos(phi);
        double sinPhi = sin(phi);
        for (int j = 0; j < levels; j++) {
            double theta = TWO_PI * double(j)/levels;
            vr.push_back(ofVec3f(cos(theta) * sinPhi, sin(theta) * sinPhi, cosPhi));
        }
    }
    vr.push_back(ofVec3f(0,0,-1));
}

//--------------------------------------------------------------
void uvsphere::createVertexNormals(){
    
    norms = vr; // create vector same size as vertex one
    for(int i = 0; i < norms.size(); i++) norms[i] = ofVec3f(0,0,0); // start each vertex at 0,0,0
    
    //top row triangle fan
    for (int j=1; j<levels; j++) {
        norms[0] += calcFaceNormal(vr[0], vr[j], vr[j+1]);
        norms[j] += calcFaceNormal(vr[0], vr[j], vr[j+1]);
        norms[j+1] += calcFaceNormal(vr[0], vr[j], vr[j+1]);
    }
    norms[0] += calcFaceNormal(vr[0], vr[levels], vr[1]);
    norms[levels] += calcFaceNormal(vr[0], vr[levels], vr[1]);
    norms[1] += calcFaceNormal(vr[0], vr[levels], vr[1]);

    
    //triangle strips
    for (int i=0; i < levels-2; i++) {
        int top = i * levels + 1;
        int bottom = (i + 1) * levels + 1;
        for (int j = 0; j < levels - 1; j++) {
            norms[bottom + j] += calcFaceNormal(vr[bottom + j], vr[bottom + j + 1], vr[top + j + 1]);
            norms[bottom + j + 1] += calcFaceNormal(vr[bottom + j], vr[bottom + j + 1], vr[top + j + 1]);
            norms[top + j + 1] += calcFaceNormal(vr[bottom + j], vr[bottom + j + 1], vr[top + j + 1]);
            norms[bottom + j] += calcFaceNormal(vr[bottom + j], vr[top + j + 1], vr[top + j]);
            norms[top + j + 1] += calcFaceNormal(vr[bottom + j], vr[top + j + 1], vr[top + j]);
            norms[top + j] += calcFaceNormal(vr[bottom + j], vr[top + j + 1], vr[top + j]);
        }
        norms[bottom + levels-1] += calcFaceNormal(vr[bottom + levels-1], vr[bottom], vr[top]);
        norms[bottom] += calcFaceNormal(vr[bottom + levels-1], vr[bottom], vr[top]);
        norms[top] += calcFaceNormal(vr[bottom + levels-1], vr[bottom], vr[top]);
        norms[bottom + levels-1] += calcFaceNormal(vr[bottom + levels-1], vr[top], vr[top + levels -1]);
        norms[top] += calcFaceNormal(vr[bottom + levels-1], vr[top], vr[top + levels -1]);
        norms[top + levels-1] += calcFaceNormal(vr[bottom + levels-1], vr[top], vr[top + levels -1]);

    }
    //bottom row triangle fan
    int last = vr.size()-1;
    for (int j = last - 1; j > last - levels; j--) {
        norms[last] += calcFaceNormal(vr[last], vr[j], vr[j-1]);
        norms[j] += calcFaceNormal(vr[last], vr[j], vr[j-1]);
        norms[j-1] += calcFaceNormal(vr[last], vr[j], vr[j-1]);
    }
    norms[last] += calcFaceNormal(vr[last], vr[last-levels], vr[last-1]);
    norms[last-levels] += calcFaceNormal(vr[last], vr[last-levels], vr[last-1]);
    norms[last-1] += calcFaceNormal(vr[last], vr[last-levels], vr[last-1]);

    
    // normalize vertex normals
    for(int i = 0; i < norms.size(); i++) norms[i] = norms[i].normalize();
}

//--------------------------------------------------------------
void uvsphere::tri(int i1, int i2, int i3){
    
    glNormal3f(norms[i1].x, norms[i1].y, norms[i1].z);
    glVertex3f(vr[i1].x, vr[i1].y, vr[i1].z);
    glNormal3f(norms[i2].x, norms[i2].y, norms[i2].z);
    glVertex3f(vr[i2].x, vr[i2].y, vr[i2].z);
    glNormal3f(norms[i3].x, norms[i3].y, norms[i3].z);
    glVertex3f(vr[i3].x, vr[i3].y, vr[i3].z);
}

//--------------------------------------------------------------
void uvsphere::triangle(int i1, int i2, int i3){
    ofVec3f vert1 = vr[i1];
    ofVec3f vert2 = vr[i2];
    ofVec3f vert3 = vr[i3];
    ofVec3f norm = calcFaceNormal(vert1, vert2, vert3);
    
    glNormal3f(norm.x, norm.y, norm.z);
    glVertex3f(vert1.x, vert1.y, vert1.z);
    glVertex3f(vert2.x, vert2.y, vert2.z);
    glVertex3f(vert3.x, vert3.y, vert3.z);
}

//--------------------------------------------------------------
ofVec3f uvsphere::calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3){
    //calculate normals
    ofVec3f A(vert3.x - vert1.x,vert3.y - vert1.y,vert3.z - vert1.z); // calc difference between 3 and 1
    ofVec3f B(vert2.x - vert1.x,vert2.y - vert1.y,vert2.z - vert1.z); // calc difference between 2 and 1
    ofVec3f norm(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); // cross product
    norm.normalize();
    
    return norm;
}



