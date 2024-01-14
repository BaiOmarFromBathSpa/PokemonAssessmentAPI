#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "vector"

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
	ofRectangle DrawImage(ofImage aImage, int width /* = 0 */, int height /* = 0 */, char alignX /* = 'n' */, char alignY /* = 'n' */, int*Padding);
	// it appears openframeworks doesn't like default values... since they are custom functions ill put the default values whenever i call them with "my defaults" :|
	
	enum class Pages {
		MainMenu,
		PickPokemon,
		BattleScene
	} CurrPage;

	enum class AllButtons {
		none,
		Click2Play,
		TopRightBtn,
		DifficultyBtn
	};

	int PokeStreak;
	int HintsRemaining;
	int HighScore;
	
	ofxJSONElement PokeAPI;//stores data from API
	ofColor Accent1, Accent2;

	ofImage StaticImgLoader; // load images wih no function.... just to be looked at
	ofImage Click2Play;
	ofImage TopRightBtn; //top right icon... could serve multiple functions based on page or state
	ofImage DifficultyBtn;
	
	ofTrueTypeFont Font_Selector;
	int DifficultyOption;

	vector< pair<AllButtons, ofRectangle> > Buttons; 
};