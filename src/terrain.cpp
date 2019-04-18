//
//  terrain.cpp
//  world
//
//  Created by mzlt on 17/02/2018.
//

#include "terrain.hpp"
//--------------------------------------------------------------
terrain::terrain(){}

//--------------------------------------------------------------
void terrain::setup(ofVec3f mid, int complexity, bool sm){
    smooth = sm;
    if(vr.size()!=0) vr.clear();
    if(norms.size()!=0) norms.clear();
    levels = complexity;
    createVertices();
    for(int i = 0; i < vr.size(); i++) {
        vr[i] += mid; // position in the centre
    }
    if (smooth) createVertexNormals();

}

//--------------------------------------------------------------
void terrain::draw(){
    glEnable(GL_FOG);

    glClearColor(0.9843, 0.9882, 0.9725, 0.0);
    GLfloat fogColor[] = {0.9843, 0.9882, 0.9725, 0.0};

    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 15.0f);
    glFogf(GL_FOG_END, 25.0f);

    glBegin(GL_TRIANGLES);
        
    GLfloat mat_ambient[]={ 0.0425f, 0.0325f,0.0125f, 1.0};
    GLfloat mat_diffuse[]={0.79f, 0.69f, 0.49f, 1.0};
    GLfloat mat_specular[] ={0.216228f, 0.216228f, 0.216228f, 1.0};
    GLfloat shine = ofMap(12.8f , 0, 128, 0, 1);


    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
    
    for(int i = 0; i < levels - 1; i++) {
        for(int j = 0; j < levels - 1; j++) {
            int topLeft     = i + levels * j;
            int bottomLeft  = i + 1 + levels * j;
            int topRight    = i + levels * (j + 1);
            int bottomRight = i + 1 + levels * (j + 1);
            

            
            if (smooth) {
                tri(topLeft, bottomLeft, topRight);
                tri(bottomLeft, bottomRight, topRight);
            } else {
                triangle(vr[topLeft], vr[bottomLeft], vr[topRight]);
                triangle(vr[bottomLeft], vr[bottomRight], vr[topRight]);
            }
        }
    }
    glEnd();
    glDisable(GL_FOG);
}


//--------------------------------------------------------------
void terrain::createVertices(){

    for (int i = -levels/2; i < levels/2; i++) {
        for (int j = -levels/2; j < levels/2; j++) {
            
            float imap = ofMap(i, -levels/2, levels/2, -25, 25);
            float jmap = ofMap(j, -levels/2, levels/2, -25, 25);
            
            float s;
            if (smooth) s = 2;
            else s = 3;
            
            float n1 = ofNoise(imap * 0.1, jmap * 0.1, 0.02) * s;
            ofVec3f v(imap, n1, jmap);
            
            float dis = sqrt((v.x*v.x)+(v.y*v.y));
                        
            vr.push_back(v);
        }
    }
}

//--------------------------------------------------------------
void terrain::createVertexNormals(){
    
    norms = vr; // create vector same size as vertex one
    for(int i = 0; i < norms.size(); i++) norms[i] = ofVec3f(0,0,0); // start each vertex at 0,0,0
    
    for(int i = 0; i < levels - 1; i++) {
        for(int j = 0; j < levels - 1; j++) {
            int topLeft     = i + levels * j;
            int bottomLeft  = i + 1 + levels * j;
            int topRight    = i + levels * (j + 1);
            int bottomRight = i + 1 + levels * (j + 1);
            // calculate normal for each face and add them up for each vector
            // store at the same index as its vertex
            
            norms[topLeft] += calcFaceNormal(vr[topLeft], vr[bottomLeft], vr[topRight]);
            norms[bottomLeft] += calcFaceNormal(vr[topLeft], vr[bottomLeft], vr[topRight]);
            norms[topRight] += calcFaceNormal(vr[topLeft], vr[bottomLeft], vr[topRight]);
            

            norms[bottomLeft] += calcFaceNormal(vr[bottomLeft], vr[bottomRight], vr[topRight]);
            norms[bottomRight] += calcFaceNormal(vr[bottomLeft], vr[bottomRight], vr[topRight]);
            norms[topRight] += calcFaceNormal(vr[bottomLeft], vr[bottomRight], vr[topRight]);
            
        }
    }
    // normalize vertex normals
    for(int i = 0; i < norms.size(); i++) norms[i] = norms[i].normalize();
}



//--------------------------------------------------------------
void terrain::tri(int i1, int i2, int i3){
    
    glNormal3f(norms[i1].x, norms[i1].y, norms[i1].z);
    glVertex3f(vr[i1].x, vr[i1].y, vr[i1].z);
    glNormal3f(norms[i2].x, norms[i2].y, norms[i2].z);
    glVertex3f(vr[i2].x, vr[i2].y, vr[i2].z);
    glNormal3f(norms[i3].x, norms[i3].y, norms[i3].z);
    glVertex3f(vr[i3].x, vr[i3].y, vr[i3].z);
}


//--------------------------------------------------------------
ofVec3f terrain::calcFaceNormal(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3){
    
    //calculate normals
    ofVec3f A(vert3.x - vert1.x,vert3.y - vert1.y,vert3.z - vert1.z); // calc difference between 3 and 1
    ofVec3f B(vert2.x - vert1.x,vert2.y - vert1.y,vert2.z - vert1.z); // calc difference between 2 and 1
    ofVec3f norm(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); // cross product
    norm.normalize();
    return norm;

}

//--------------------------------------------------------------
void terrain::triangle(ofVec3f vert1, ofVec3f vert2, ofVec3f vert3){
    
    //calculate face normals
    ofVec3f norm = calcFaceNormal(vert1, vert2, vert3);
    // same normal for all vertices in triangle
    glNormal3f(norm.x, norm.y, norm.z);
    glVertex3f(vert1.x, vert1.y, vert1.z);
    glVertex3f(vert2.x, vert2.y, vert2.z);
    glVertex3f(vert3.x, vert3.y, vert3.z);
}
