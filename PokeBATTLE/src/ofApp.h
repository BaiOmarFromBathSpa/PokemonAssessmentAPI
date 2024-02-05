#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include <vector>
#include <algorithm>
#include <string>

enum class PokeTypes {
	none,
	Bug, Dark, Dragon,
	Electric, Fairy, Fighting,
	Fire, Flying, Ghost,
	Grass, Ground, Ice,
	Normal, Poison, Psychic,
	Rock, Steel, Water
};

class Pokemon {
public:
	string PokeName;
	int PokeNum;
	pair <PokeTypes, PokeTypes> PokeTyping;
	pair <string, string> strPokeTyping;
	//pair <ofColor, ofColor> PokeTypingCol;

	PokeTypes setTyping(string &Type) {
		if (Type == "none") return PokeTypes::none;
		else if (Type == "bug") return PokeTypes::Bug;
		else if (Type == "dark") return PokeTypes::Dark;
		else if (Type == "dragon") return PokeTypes::Dragon;
		else if (Type == "electric") return PokeTypes::Electric;
		else if (Type == "fairy") return PokeTypes::Fairy;
		else if (Type == "fighting") return PokeTypes::Fighting;
		else if (Type == "fire") return PokeTypes::Fire;
		else if (Type == "flying") return PokeTypes::Flying;
		else if (Type == "ghost") return PokeTypes::Ghost;
		else if (Type == "grass") return PokeTypes::Grass;
		else if (Type == "ground") return PokeTypes::Ground;
		else if (Type == "ice") return PokeTypes::Ice;
		else if (Type == "normal") return PokeTypes::Normal;
		else if (Type == "poison") return PokeTypes::Poison;
		else if (Type == "psychic") return PokeTypes::Psychic;
		else if (Type == "rock") return PokeTypes::Rock;
		else if (Type == "steel") return PokeTypes::Steel;
		else if (Type == "water") return PokeTypes::Water;
		else {
			Type = "normal";
			return PokeTypes::Normal;
		}
	}
	Pokemon(string aName, int aNum, string Type1, string Type2 = "none") {
		PokeName = aName;
		PokeNum = aNum;
		PokeTyping = make_pair(setTyping(Type1), setTyping(Type2));
		strPokeTyping = make_pair(Type1, Type2);
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
	void DrawPage1();
	void DrawPage2();
	void DrawPage3();

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
		PokeSelected,
		Start,
		RevivePotion
	};

	int PokeStreak;
	int HintsRemaining;
	int HighScore;
	int PokemonLeft;
	int DifficultyOption;
	int CurrPoke;
	pair <bool, bool> RevivePotion;
	bool GamePaused;

	vector<Pokemon> StarterPoke;
	vector<int> PokeSelected; //issues with iterator had to duplicate make a int vector instead of a "Pokemon" one
	vector<Pokemon> PokeTeam;
	int Offset, Limit;
	int Columns, Rows;
	int Gap, StartX, StartY;
	int FrameX, FrameY; //Allocated space - Gaps / Number of Elements
	int ScreenX, ScreenY;
	int ReviveCount, PotionCount;

	ofxJSONElement PokeAPI;//stores data from API
	ofColor Accent1, Accent2;

	ofImage StaticImgLoader; // load images that aren't buttons / animated
	ofImage Click2Play;
	ofImage TopRightBtn; //top right icon... could serve multiple functions based on page or state
	ofImage DifficultyBtn;
	ofImage PokeBall;
	
	ofTrueTypeFont Font_Selector;
	vector< pair<AllButtons, ofRectangle> > ButtonsCont; 
};