//
//  cubesphere.cpp
//  world
//
//  Created by mzlt on 14/02/2018.
//

#include "cubesphere.hpp"


//--------------------------------------------------------------
cubesphere::cubesphere(){}

//--------------------------------------------------------------
void cubesphere::setup(ofVec3f mid, int complexity){
    smooth = false;
    levels = complexity;
    vector<ofVec3f> origins;
    vector<ofVec3f> rights;
    vector<ofVec3f> ups;
    createOriginsRightsUps(origins, rights, ups);
    createVertices(origins, rights, ups);
    for(int i = 0; i < vertices.size(); i++) vertices[i] += mid;
    r = ofRandom(0.1,1);
    g = ofRandom(0.1,1);
    b = ofRandom(0.1,1);
}

//--------------------------------------------------------------
void cubesphere::draw(){
    glBegin(GL_TRIANGLES);

    GLfloat mat_ambient[]={ r * 0.3f, g * 0.3f, b * 0.3f, 1.0f };
    GLfloat mat_diffuse[]={ r, g, b, 1.0f };
    GLfloat mat_specular[]={ 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat shine = ofMap(27.8974f, 0, 128, 0, 1);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
    
    if (smooth) dr(&cubesphere::tri, this);
    else dr(&cubesphere::triangle, this);
    
    
    glEnd();
}


//--------------------------------------------------------------
void cubesphere::dr(void (cubesphere::*action)(int, int, int) , cubesphere* obj){
    
    const int k = levels + 1;
    for (int face = 0; face < 6; ++face) {
        for (int j = 0; j < levels; ++j){
            const bool bottom = j < (levels / 2);
            for (int i = 0; i < levels; ++i){
                const bool left = i < (levels / 2);
                int a = (face * k + j) * k + i;
                int b = (face * k + j) * k + i + 1;
                int c = (face * k + j + 1) * k + i;
                int d = (face * k + j + 1) * k + i + 1;
                if (bottom || left) {
                    (obj->*action)(a,c,b);
                    (obj->*action)(c,d,b);
                } else {
                    (obj->*action)(a,c,d);
                    (obj->*action)(a,d,b);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void cubesphere::createOriginsRightsUps(vector<ofVec3f> &o,vector<ofVec3f> &r, vector<ofVec3f> &u){
    o.push_back(ofVec3f(-1.0, -1.0, -1.0));
    o.push_back(ofVec3f(1.0, -1.0, -1.0));
    o.push_back(ofVec3f(1.0, -1.0, 1.0));
    o.push_back(ofVec3f(-1.0, -1.0, 1.0));
    o.push_back(ofVec3f(-1.0, 1.0, -1.0));
    o.push_back(ofVec3f(-1.0, -1.0, 1.0));
    
    r.push_back(ofVec3f(2.0, 0.0, 0.0));
    r.push_back(ofVec3f(0.0, 0.0, 2.0));
    r.push_back(ofVec3f(-2.0, 0.0, 0.0));
    r.push_back(ofVec3f(0.0, 0.0, -2.0));
    r.push_back(ofVec3f(2.0, 0.0, 0.0));
    r.push_back(ofVec3f(2.0, 0.0, 0.0));
    
    u.push_back(ofVec3f(0.0, 2.0, 0.0));
    u.push_back(ofVec3f(0.0, 2.0, 0.0));
    u.push_back(ofVec3f(0.0, 2.0, 0.0));
    u.push_back(ofVec3f(0.0, 2.0, 0.0));
    u.push_back(ofVec3f(0.0, 0.0, 2.0));
    u.push_back(ofVec3f(0.0, 0.0, -2.0));
}

//--------------------------------------------------------------
void cubesphere::createVertices(vector<ofVec3f> &o,vector<ofVec3f> &r, vector<ofVec3f> &u){
    const double step = 1.0 / double(levels);
    const ofVec3f step3(step, step, step);
    for (int face = 0; face < 6; ++face){
        const ofVec3f origin = o[face];
        const ofVec3f right = r[face];
        const ofVec3f up = u[face];
        for (int j = 0; j < levels + 1; ++j){
            const ofVec3f j3(j,j,j);
            for (int i = 0; i < levels + 1; ++i){
                const ofVec3f i3(i,i,i);
                const ofVec3f p = origin + step3 * (i3 * right + j3 * up);
                const ofVec3f p2 = p * p;
                const ofVec3f n(
                                p.x * sqrt(1.0 - 0.5 * (p2.y + p2.z) + p2.y * p2.z / 3.0),
                                p.y * sqrt(1.0 - 0.5 * (p2.z + p2.x) + p2.z * p2.x / 3.0),
                                p.z * sqrt(1.0 - 0.5 * (p2.x + p2.y) + p2.x * p2.y / 3.0)
                                );
                vertices.push_back(n);
                norms.push_back(-n);
            }
        }
    }
}

//--------------------------------------------------------------
void cubesphere::triangle(int i1, int i2, int i3){
    ofVec3f norm = calcFaceNormal(vertices[i1], vertices[i2], vertices[i3]);
    
    glNormal3f(norm.x, norm.y, norm.z);
    glVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z);
    glVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z);
    glVertex3f(vertices[i3].x, vertices[i3].y, vertices[i3].z);
}

//--------------------------------------------------------------
ofVec3f cubesphere::calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3){
    //calculate normals
    ofVec3f A(vert3.x - vert1.x,vert3.y - vert1.y,vert3.z - vert1.z); // calc difference between 3 and 1
    ofVec3f B(vert2.x - vert1.x,vert2.y - vert1.y,vert2.z - vert1.z); // calc difference between 2 and 1
    ofVec3f norm(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); // cross product
    norm.normalize();
    
    return norm;
}

//--------------------------------------------------------------
void cubesphere::tri(int i1, int i2, int i3){
    
    glNormal3f(norms[i1].x, norms[i1].y, norms[i1].z);
    glVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z);
    glNormal3f(norms[i2].x, norms[i2].y, norms[i2].z);
    glVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z);
    glNormal3f(norms[i3].x, norms[i3].y, norms[i3].z);
    glVertex3f(vertices[i3].x, vertices[i3].y, vertices[i3].z);
}



