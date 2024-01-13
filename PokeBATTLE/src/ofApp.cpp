#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    /*
     API documentation can be found at: https://pokeapi.co/docs/v2
    */
    json.open("https://pokeapi.co/api/v2/berry/cheri");//returns data on the Cheri Berry
    cout << json.getRawString() << endl;
    ofBackground(255,255,255);
    Logo.load("PokeBattle_Logo.png");
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    int CurrPage = 1;
    if (CurrPage == 1) {
        int CurrHeight = ofGetHeight(), CurrWidth = ofGetWidth();
        int PaddingPercent[4] = {0,0,0,0};
        Accent1.set(255, 203, 5);
        Accent2.set(50, 105, 177);

        DrawBG(Accent1, Accent2);

        PaddingPercent[0] = 15; //set padding before going into function - it gets reset within the function hurray pointers
        DrawImage(Logo, 90, 0, 'r', 't', &PaddingPercent[0]); //fix tomorrow getting late late

    }
}
void ofApp::DrawBG(ofColor Col1, ofColor Col2) {
    int CurrHeight = ofGetHeight(), CurrWidth = ofGetWidth();
    int Height34p = (33 * CurrHeight) / 100; //using to simulate 1/3 of screen height in int form

    ofMesh Gradient;    //3d mesh used in 2d form to make a gradient as the regular background ones are too limiting (by making z axis 0)
    Gradient.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);  
    //vertices are zig zagged to get 2d plane effect notice GL_TRIANGLE_STRIP - https://openframeworks.cc/documentation/3d/gl_primitives_640x269.png

    Gradient.addVertex(ofVec3f(0, 0, 0)); //vert1 left - top
    Gradient.addColor(Col1);

    Gradient.addVertex(ofVec3f(CurrWidth, 0, 0)); //vert2 right - top
    Gradient.addColor(Col1);

    Gradient.addVertex(ofVec3f(0, Height34p, 0)); //vert3 left - 1/3 down
    Gradient.addColor(Col1);

    Gradient.addVertex(ofVec3f(CurrWidth, Height34p, 0)); //vert4 right - 1/3 down
    Gradient.addColor(Col1);

    Gradient.addVertex(ofVec3f(0, Height34p * 2, 0));  //vert5 left - 2/3 down
    Gradient.addColor(ofColor(255, 255, 255));

    Gradient.addVertex(ofVec3f(CurrWidth, Height34p * 2, 0));  //vert6 right - 2/3 down
    Gradient.addColor(ofColor(255, 255, 255));

    Gradient.addVertex(ofVec3f(0, CurrHeight, 0));  //vert7 left - down
    Gradient.addColor(Col2);

    Gradient.addVertex(ofVec3f(CurrWidth, CurrHeight, 0));  //vert8 right - bottom
    Gradient.addColor(Col2);

    ofFill(); //fill in vertices
    Gradient.draw(); //draw mesh
}

void ofApp::DrawImage(ofImage aImage, int widthPercent, int heightPercent, char alignX, char alignY, int* PaddingPtr) {
    /* Arguments Explains
        1 - Image being used
        2 - percentage of width of screen to occupy [ if set to 0 set at native res - recent fix i was going to make it invisible ]
        3 - percentage of height of screen to occupy [ <= 0 to maintain native image aspect ratio ]
        4 - Align on the x axis of the screen [ l - left(default) | c - center | r - right] <- invalid syntax is set to default
        5 - Align on the y axis of the screen [ t - top(default) | c - center | b - bottom] <- invalid syntax is set to default 
        6 - Padding of element... basically lets you nudge is a little bit - takes in a array technically but issues so pointer to first element in array of 4 [you can go higher they'll just be ignored] */

    int Padding[4]; //4 bytes of data duplicated for a a few milisecs its ok
    for (int i = 0; i < 4; i++) {
        Padding[i] = *PaddingPtr; //There is probably a more efficient way but i cant pass arrays and am bad at pointers and lightly frustrated it's almost 3am
        *PaddingPtr = 0;          //reset padding when no longer in use
        PaddingPtr++;
    }

    if (widthPercent <= 0) //set to native if unset
        widthPercent = ((aImage.getWidth() * 100) / ofGetWidth());

    if (alignX != 'l')
        if (alignX != 'r' && alignX != 'c') alignX = 'l';
    if (alignY != 't')
        if (alignY != 'b' && alignY != 'c') alignY = 't';

    /* ^ Set Default Values ^ */

    double aspectRatio = aImage.getHeight() / aImage.getWidth(); //get native images aspect ratio
    int CurrHeight = ofGetHeight(), CurrWidth = ofGetWidth();   //i assume these functions can be computationally expensive if run every frame so minimising their call count
    int PaddingLeft = ((Padding[3] - Padding[1]) * CurrWidth) / 100, PaddingTop = ((Padding[0] - Padding[2]) * CurrHeight) / 100; //collapse padding because only the top and left matter (positioning when drawing image)
    
    switch (alignX) {
    case 'c':
        PaddingLeft += (CurrWidth / 2) - (((widthPercent * CurrWidth) / 100) / 2); //if centering get half ofscreen minus half of image width
        break;
    case 'r':
        PaddingLeft = (CurrWidth - ((widthPercent * CurrWidth) / 100)) - ((PaddingLeft * CurrWidth) / 100); //if right then get whole screen minus size of element
        break;
    case 'l': default:
        break;
    }
    
    if (heightPercent <= 0) {//if height unset maintain aspect ratio
        heightPercent = widthPercent * aspectRatio;
    }

    switch (alignY) { //sucks repeating code but hardly worth the trouble of nested functions
    case 'c':
        PaddingTop += (CurrHeight / 2) - (((heightPercent * CurrHeight) / 100) / 2);
        break;
    case 'b':
        PaddingTop = (CurrHeight + ((heightPercent * CurrHeight) / 100)) - PaddingTop;
        break;
    case 't': default:
        break;
    }   
    
    aImage.draw(PaddingLeft, PaddingTop, ((widthPercent * CurrWidth) / 100), ((heightPercent * CurrWidth) / 100));
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