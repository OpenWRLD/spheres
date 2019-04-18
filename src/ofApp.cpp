#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    smooth = false;

    
    
    // This generates a bunch of arrays that we can pass in as lighting parameters
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    GLfloat position[] = { -0.3f, -0.2f, -0.3f, 1.0f };
    
    // These fuctions define the characteristics of our light, GL_LIGHT0 - remember there are 8 lights
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    

//
    glShadeModel(GL_SMOOTH); // Can also be flat. When flat, the last colour specified for a triangle is the colour of the triangle
    
    numspheres = 5;
    
    for(int i = 0; i < numspheres; i ++){
        uvs.push_back(new uvsphere());
        icos.push_back(new icosphere());
        cubes.push_back(new cubesphere());
    }
    


    for(int i = 0; i < numspheres; i ++){
        
        int radius = 10;

        double xposu = cos((2 * PI) * double(i) / numspheres) * radius;
        double zposu = sin((2 * PI) * double(i) / numspheres) * radius;
        double xposi = cos(2 * PI / 3 + (2 * PI) * double(i) / numspheres) * radius;
        double zposi = sin(2 * PI / 3 + (2 * PI) * double(i) / numspheres) * radius;
        double xposc = cos(4 * PI / 3 + (2 * PI) * double(i) / numspheres) * radius;
        double zposc = sin(4 * PI / 3 + (2 * PI) * double(i) / numspheres) * radius;
        
        uvs[i]->setup(ofVec3f(xposu,0, zposu), floor(ofRandom(5,60)));
        icos[i]->setup(ofVec3f(xposi,0, zposi), floor(ofRandom(0,5)));
        cubes[i]->setup(ofVec3f(xposc,0, zposc), floor(ofRandom(1,10)));
    }
    
    
    sky.setup();
    ter.setup(ofVec3f(0,-5,0), 20, 0);
    
    
    
    cam.setNearClip(1);
    cam.setFarClip(-100);
    cam.setPosition(0,0,0);
}


//--------------------------------------------------------------
void ofApp::update(){
    
    
}



//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofEnableDepthTest();
    cam.begin();
    sky.draw(ofVec3f(0,0,0), 50);
    

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    
    
    for(int i = 0; i < numspheres; i++) {
        uvs[i]->draw();
        icos[i]->draw();
        cubes[i]->draw();
    }
    ter.draw();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    
    cam.end();
    ofDisableDepthTest();
    
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
//    cout<<key<<endl;
    if (key==32) {
        smooth = !smooth;
        smoothE();
    }
    else if (key==356) cam.pan(1);
    else if (key==357) cam.tilt(1);
    else if(key==358) cam.pan(-1);
    else if (key==359) cam.tilt(-1);
}

//--------------------------------------------------------------
void ofApp::smoothE(){
    for(int i = 0; i < numspheres; i++) {
        cubes[i]->smooth = smooth;
        uvs[i]->smooth = smooth;
        icos[i]->smooth = smooth;
    }
    if (smooth) ter.setup(ofVec3f(0,-5,0), 200, 1);
    else ter.setup(ofVec3f(0,-5,0), 20, 0);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
