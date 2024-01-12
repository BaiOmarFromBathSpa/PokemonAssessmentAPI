#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    /*
     API documentation can be found at: https://pokeapi.co/docs/v2
    */
    json.open("https://pokeapi.co/api/v2/berry/cheri");//returns data on the Cheri Berry
    cout << json.getRawString() << endl;
    ofBackground(255,255,255);
    GradTop = (255, 203, 5);
    GradBtm = (50, 105, 177);   
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    int CurrPage = 1;
    if (CurrPage == 1) {

    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//-----------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}