#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include <vector>
#include <algorithm>
#include <string>


class MakeButton {
	ofRectangle MyRect;
public:
	MakeButton(ofImage Picture) {
		
	};
	MakeButton(ofRectangle Rectangle) {

	};
};

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
		DifficultyBtn,
		PrevPage,
		NextPage,
		PokeSelectGrid,
		Start
	};

	int PokeStreak;
	int HintsRemaining;
	int HighScore;
	int PokemonLeft;
	int DifficultyOption;

	vector<int> PokeInPage;
	vector<int> PokeSelected;
	int Offset, Limit;
	int Columns, Rows;
	int Gap, StartX, StartY;
	int FrameX, FrameY; //Allocated space - Gaps / Number of Elements
	int ScreenX, ScreenY;

	ofxJSONElement PokeAPI;//stores data from API
	ofColor Accent1, Accent2;

	ofImage StaticImgLoader; // load images that aren't buttons / animated
	ofImage Click2Play;
	ofImage TopRightBtn; //top right icon... could serve multiple functions based on page or state
	ofImage DifficultyBtn;
	
	ofTrueTypeFont Font_Selector;
	vector< pair<AllButtons, ofRectangle> > ButtonsCont; 
};