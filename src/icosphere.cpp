//
//  icosphere.cpp
//  world
//
//  Created by mzlt on 13/02/2018.
//

#include "icosphere.hpp"

//--------------------------------------------------------------
icosphere::icosphere(){}

//--------------------------------------------------------------
void icosphere::setup(ofVec3f mid, int levels){
    smooth = false;
    r = ofRandom(0.1,1);
    g = ofRandom(0.1,1);
    b = ofRandom(0.1,1);
    index = 0;
    createVertices();
    createFaces();
    refineFaces(levels);
    for(int i = 0; i < vs.size(); i++) vs[i] += mid;
    createVertexNormals();
}

//--------------------------------------------------------------
void icosphere::draw(){
    glBegin(GL_TRIANGLES);

    float m1 = min(r,g);
    float m = min(m1,b);
    float msr = 0.6f;
    float msg = 0.6f;
    float msb = 0.6f;

    if (m==r) msr = 0.7f;
    else if (m==g) msg = 0.7f;
    else msb = 0.7f;

    GLfloat mat_ambient[]={ 0.0f,0.0f,0.0f,1.0f };
    GLfloat mat_diffuse[]={ r,g,b,1.0f};
    GLfloat mat_specular[]={msr,msg,msb,1.0f };
    GLfloat shine = ofMap(32.0f , 0, 128, 0, 1);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
    
    if (smooth) dr(&icosphere::tri, this);
    else dr(&icosphere::triangle, this);
    
//    drawSmooth();
    
    
    glEnd();
    
}

//--------------------------------------------------------------
void icosphere::dr(void (icosphere::*action)(int, int, int) , icosphere* obj) {

    for (int i = 0; i < faces.size(); i++) {
        (obj->*action)(faces[i].x,faces[i].y,faces[i].z);
    }
}

//--------------------------------------------------------------
void icosphere::createVertices(){ // create 12 vertices of a icosahedron
    double t = (1.0 + sqrt(5.0)) / 2.0;
    
    addV(ofVec3f(-1,  t,  0));
    addV(ofVec3f( 1,  t,  0));
    addV(ofVec3f(-1, -t,  0));
    addV(ofVec3f( 1, -t,  0));
    
    addV(ofVec3f( 0, -1,  t));
    addV(ofVec3f( 0,  1,  t));
    addV(ofVec3f( 0, -1, -t));
    addV(ofVec3f( 0,  1, -t));
    
    addV(ofVec3f( t,  0, -1));
    addV(ofVec3f( t,  0,  1));
    addV(ofVec3f(-t,  0, -1));
    addV(ofVec3f(-t,  0,  1));
}

//--------------------------------------------------------------
void icosphere::createFaces(){
    // create 20 triangles of the icosahedron
    
    // 5 faces around point 0
    faces.push_back(ofVec3f(0, 11, 5));
    faces.push_back(ofVec3f(0, 5, 1));
    faces.push_back(ofVec3f(0, 1, 7));
    faces.push_back(ofVec3f(0, 7, 10));
    faces.push_back(ofVec3f(0, 10, 11));
    
    // 5 adjacent faces
    faces.push_back(ofVec3f(1, 5, 9));
    faces.push_back(ofVec3f(5, 11, 4));
    faces.push_back(ofVec3f(11, 10, 2));
    faces.push_back(ofVec3f(10, 7, 6));
    faces.push_back(ofVec3f(7, 1, 8));
    
    // 5 faces around point 3
    faces.push_back(ofVec3f(3, 9, 4));
    faces.push_back(ofVec3f(3, 4, 2));
    faces.push_back(ofVec3f(3, 2, 6));
    faces.push_back(ofVec3f(3, 6, 8));
    faces.push_back(ofVec3f(3, 8, 9));
    
    // 5 adjacent faces
    faces.push_back(ofVec3f(4, 9, 5));
    faces.push_back(ofVec3f(2, 4, 11));
    faces.push_back(ofVec3f(6, 2, 10));
    faces.push_back(ofVec3f(8, 6, 7));
    faces.push_back(ofVec3f(9, 8, 1));

}

//--------------------------------------------------------------
void icosphere::createVertexNormals(){
    
    norms = vs; // create vector same size as vertex one
    for(int i = 0; i < norms.size(); i++) norms[i] = ofVec3f(0,0,0); // start each vertex at 0,0,0
    
    for (int i = 0; i < faces.size(); i++) {
        norms[faces[i].x] += calcFaceNormal(vs[faces[i].x],vs[faces[i].y],vs[faces[i].z]);
        norms[faces[i].y] += calcFaceNormal(vs[faces[i].x],vs[faces[i].y],vs[faces[i].z]);
        norms[faces[i].z] += calcFaceNormal(vs[faces[i].x],vs[faces[i].y],vs[faces[i].z]);
    }

    // normalize vertex normals
    for(int i = 0; i < norms.size(); i++) norms[i] = norms[i].normalize();
}



//--------------------------------------------------------------
void icosphere::refineFaces(int &levels){
    for (int i = 0; i < levels; i++) {
        vector<ofVec3f> newfaces;
        for (int j = 0; j < faces.size(); j++){
            // return index of point in the middle of each edge
            int a = getMiddlePoint(faces[j].x, faces[j].y);
            int b = getMiddlePoint(faces[j].y, faces[j].z);
            int c = getMiddlePoint(faces[j].z, faces[j].x);
            // create a new vector of triangle indicies
            newfaces.push_back(ofVec3f(faces[j].x, a, c));
            newfaces.push_back(ofVec3f(faces[j].y, b, a));
            newfaces.push_back(ofVec3f(faces[j].z, c, b));
            newfaces.push_back(ofVec3f(a, b, c));
        }
        faces = newfaces;
        newfaces.clear();
    }
    
}

//--------------------------------------------------------------
int icosphere::getMiddlePoint(int p1, int p2){
    
    long smallerIndex;
    long greaterIndex;
    
    // determine which index is bigger
    if(p1 < p2) {
        smallerIndex = p1;
        greaterIndex = p2;
    } else {
        smallerIndex = p2;
        greaterIndex = p1;
    }
    
    // not sure what key is this has to do with bit shifting which is beyond me as of yet
    long key = (smallerIndex << 32) + greaterIndex;
    // search the map for the key
    unordered_map<long,int>::const_iterator got = middlePointIndexCache.find(key);
    
    if ( got == middlePointIndexCache.end() ){ // if the key doesn't yet exist
        // find a middle point between the two indexes
        ofVec3f middle((vs[p1].x + vs[p2].x) / 2.0, (vs[p1].y + vs[p2].y) / 2.0, (vs[p1].z + vs[p2].z) / 2.0);
        int i = addV(middle); // add to the vs and return new index
        middlePointIndexCache.emplace(key, i); // add the new pair to the map
        return i; // return the index of the new point
    } else {
        // found
        return got->second; // return the index of the key
    }
}

//--------------------------------------------------------------
int icosphere::addV(ofVec3f v){
    vs.push_back(v.normalize());
    return index++;
}

//--------------------------------------------------------------
void icosphere::triangle(int i1, int i2, int i3){
    ofVec3f vert1 = vs[i1];
    ofVec3f vert2 = vs[i2];
    ofVec3f vert3 = vs[i3];
    ofVec3f norm = calcFaceNormal(vert1, vert2, vert3);
    
    glNormal3f(norm.x, norm.y, norm.z);
    glVertex3f(vert1.x, vert1.y, vert1.z);
    glVertex3f(vert2.x, vert2.y, vert2.z);
    glVertex3f(vert3.x, vert3.y, vert3.z);
}

//--------------------------------------------------------------
ofVec3f icosphere::calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3){
    //calculate normals
    ofVec3f A(vert3.x - vert1.x,vert3.y - vert1.y,vert3.z - vert1.z); // calc difference between 3 and 1
    ofVec3f B(vert2.x - vert1.x,vert2.y - vert1.y,vert2.z - vert1.z); // calc difference between 2 and 1
    ofVec3f norm(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); // cross product
    norm.normalize();
    
    return norm;
}

//--------------------------------------------------------------
void icosphere::tri(int i1, int i2, int i3){
    
    glNormal3f(norms[i1].x, norms[i1].y, norms[i1].z);
    glVertex3f(vs[i1].x, vs[i1].y, vs[i1].z);
    glNormal3f(norms[i2].x, norms[i2].y, norms[i2].z);
    glVertex3f(vs[i2].x, vs[i2].y, vs[i2].z);
    glNormal3f(norms[i3].x, norms[i3].y, norms[i3].z);
    glVertex3f(vs[i3].x, vs[i3].y, vs[i3].z);
}









