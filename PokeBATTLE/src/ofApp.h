#pragma once

#include "ofMain.h"
#include "ofxJSON.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void DrawBG(ofColor Col1, ofColor Col2);
	void DrawImage(ofImage aImage, int width /* = 0 */, int height /* = 0 */, char alignX /* = 'n' */, char alignY /* = 'n' */, int*Padding);
	// it appears openframeworks doesn't like default values... since they are custom functions ill put the default values whenever i call them with "my defaults" :|
	ofxJSONElement json;//stores data from API
	ofColor Accent1, Accent2;
	ofImage Logo;
};