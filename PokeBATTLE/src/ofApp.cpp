#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    /*
     API documentation can be found at: https://pokeapi.co/docs/v2
    */
    //PokeAPI.open("https://pokeapi.co/api/v2/berry/cheri");//returns data on the Cheri Berry
    //cout << PokeAPI.getRawString() << endl;

    ScreenX = ofGetWidth(); ScreenY = ofGetHeight();
    CurrPage = Pages::MainMenu;

    PokeStreak = 0;
    HintsRemaining = 4;
    HighScore = 9909;
    PokeSelected.reserve(6);

    StaticImgLoader.load("PokeBattle_Logo.png");
    Click2Play.load("Click2Play.png");
    TopRightBtn.load("Icon_Close.png");
    DifficultyBtn.load("Icon_Diffculty_Easy.png");
    PokeBall.load("sprites/PokeBall.png");

    ofxJSONElement TempAPI;
    string APIstr;
    string tempName; int tempNum; 
    string tempPokeType1, tempPokeType2;

    int MaxSelection = 100;
    Columns = 6; Rows = 5;
    Gap = 5; StartX = 65; StartY = 400;
    FrameX = (((80 * ScreenX) / 100) - (Gap * (Columns - 1))) / Columns; //Allocated space - Gaps / Number of Elements
    FrameY = (((70 * ScreenY) / 100) - (Gap * (Rows - 1))) / Rows;

    Offset = 0; Limit = Columns * Rows;
    PokeAPI.open("https://pokeapi.co/api/v2/evolution-chain/?offset=0&limit=" + to_string(MaxSelection));
    cout << PokeAPI.getRawString() << endl;
    for (int i = 0; i < min(PokeAPI["count"].asInt(), MaxSelection); i++) {
        tempPokeType2 = "none";
        TempAPI.open(PokeAPI["results"][i]["url"].asString());
        APIstr = TempAPI["chain"]["species"]["url"].asString();

        APIstr.erase(APIstr.end() - 1);
        auto isoNum = APIstr.rfind("/");
        APIstr = APIstr.substr(isoNum+1);
        TempAPI.open("https://pokeapi.co/api/v2/pokemon/" + APIstr);

        tempName = TempAPI["name"].asString();
        cout << i << " - " << APIstr << endl;
        tempNum = stoi(APIstr);
        tempPokeType1 = TempAPI["types"][0]["type"]["name"].asString();
        if (TempAPI["types"].size() > 1) tempPokeType2 = TempAPI["types"][1]["type"]["name"].asString();
        Pokemon tempPokemon(tempName, tempNum-1, tempPokeType1, tempPokeType2);
        StarterPoke.push_back(tempPokemon);
    }
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    ButtonsCont.clear();

    switch (CurrPage) {
    case Pages::MainMenu:
        DrawPage1();
        break;
    case Pages::PickPokemon: //TODO: move methods for pages into their own function
        DrawPage2();
        break;
    case Pages::BattleScene:
        DrawPage3();
        break;
    }
    ofSetColor(255); //reset colours after every page
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

    Gradient.addVertex(ofVec3f(0, (Height34p * 2)+15, 0));  //vert5 left - 2/3 down
    Gradient.addColor(ofColor(255, 255, 255));

    Gradient.addVertex(ofVec3f(CurrWidth, (Height34p * 2)+15, 0));  //vert6 right - 2/3 down
    Gradient.addColor(ofColor(255, 255, 255));

    Gradient.addVertex(ofVec3f(0, CurrHeight, 0));  //vert7 left - down
    Gradient.addColor(Col2);

    Gradient.addVertex(ofVec3f(CurrWidth, CurrHeight, 0));  //vert8 right - bottom
    Gradient.addColor(Col2);

    ofFill(); //fill in vertices
    Gradient.draw(); //draw mesh
}

void ofApp::DrawPage1() {
    ofRectangle Button;
    Accent1.set(255, 203, 5);
    Accent2.set(50, 105, 177);
    DrawBG(Accent1, Accent2);

    StaticImgLoader.draw(55, 90, 1170, 255);

    Click2Play.draw(330, 380, 620, 220);
    Button.set(330, 380, 620, 220);
    ButtonsCont.push_back(make_pair(AllButtons::Click2Play, Button));

    TopRightBtn.draw((ScreenX - TopRightBtn.getWidth() - 15), 15, TopRightBtn.getWidth(), 50);
    Button.set((ScreenX - TopRightBtn.getWidth() - 15), 15, TopRightBtn.getWidth(), 50);
    ButtonsCont.push_back(make_pair(AllButtons::TopRightBtn, Button));

    DifficultyBtn.draw(1060, 465, 200, 65);
    Button.set(1060, 465, 200, 65);
    ButtonsCont.push_back(make_pair(AllButtons::DifficultyBtn, Button));
}

void ofApp::DrawPage2() {
    ofRectangle Button;
    Accent1.set(75, 75, 75);
    Accent2.set(75, 75, 75);
    DrawBG(Accent1, Accent2);

    StaticImgLoader.load("PokeBattle_Logo.png");
    StaticImgLoader.draw(50, 10, 585, 135);

    TopRightBtn.draw((ScreenX - TopRightBtn.getWidth() - 15), 15, TopRightBtn.getWidth(), 50);
    Button.set((ScreenX - TopRightBtn.getWidth() - 15), 15, TopRightBtn.getWidth(), 50);
    ButtonsCont.push_back(make_pair(AllButtons::TopRightBtn, Button));

    ofSetColor(255);
    Font_Selector.load("Jura-Medium.ttf", 24);
    Font_Selector.drawString("CHOOSE YOUR TEAM", 319, 144);

    ofSetColor(255);
    Font_Selector.load("TurpisItalic.ttf", 40);
    Font_Selector.drawString("Pokemon-Left:", 750, 140);

    ofSetColor(222, 90, 58);
    Font_Selector.load("TurpisItalic.ttf", 80);
    Font_Selector.drawString(to_string(PokemonLeft), 1126, 170);

    ofSetColor(0);
    ofFill();
    ofDrawRectRounded(10, 155, 1105, 12, 6);
    ofDrawRectangle(20, 155, 1095, 12);
    ofDrawRectRounded(1205, 155, 65, 12, 6);
    ofDrawRectangle(1205, 155, 55, 12);
    ofDrawRectRounded(1050, 180, 300, 700, 3);


    ofSetColor(255);
    ofFill();
    ofDrawRectRounded(13, 158, 1099, 6, 4);
    ofDrawRectangle(23, 158, 1089, 6);
    ofDrawRectRounded(1208, 158, 59, 6, 4);
    ofDrawRectangle(1208, 158, 49, 6);
    ofDrawRectRounded(1053, 183, 300, 700, 3);

    ofPushMatrix();
    ofTranslate(1050, 180);
    for (int i = 0; i < PokeSelected.size(); i++) {
        string PokeLocation = "sprites/" + to_string(PokeSelected[i]) + "/back.png";
        StaticImgLoader.load(PokeLocation);
        PokeBall.draw(5, ((ScreenY - 180) / 6) * i+10, (ScreenY - 180) / 6, (ScreenY - 180) / 6);
        StaticImgLoader.draw(60, ((ScreenY - 180) / 6) * i - 25, 160, 160);
    }
    ofPopMatrix();
    Button.set(1050, 180, ScreenX - 1050, ScreenY - 180);
    ButtonsCont.push_back(make_pair(AllButtons::PokeSelected, Button));

    ofPushMatrix();
    ofTranslate(10, 180);
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            if ((i * Columns + j + 1 + Offset) > StarterPoke.size()) break;
            string PokeLocation = "sprites/" + to_string(StarterPoke[i * Columns + j + Offset].PokeNum) + "/front.png";

            Accent1.set(255);
            auto it = find(PokeSelected.begin(), PokeSelected.end(), StarterPoke[i * Columns + j + Offset].PokeNum);
            if (it != PokeSelected.end()) Accent1.set(0, 130, 255);

            StaticImgLoader.load(PokeLocation);
            ofSetColor(0);
            ofFill();
            ofDrawRectRounded((FrameX + Gap) * j, (FrameY + Gap) * i, FrameX, FrameY, 4);
            ofSetColor(Accent1);
            ofFill();
            ofDrawRectRounded(((FrameX + Gap) * j) + 3, ((FrameY + Gap) * i) + 3, FrameX - 6, FrameY - 6, 3);
            ofSetColor(255);
            StaticImgLoader.draw((FrameX + Gap) * j, (FrameY + Gap) * i, FrameX, FrameY);
        }
    }
    ofPopMatrix();
    Button.set(10, 180, (80 * ScreenX) / 100, (70 * ScreenY) / 100);
    ButtonsCont.push_back(make_pair(AllButtons::PokeSelectGrid, Button));

    StaticImgLoader.load("Icon_Play.png");
    if (Offset > 0) {
        ofPushMatrix();
        ofTranslate(40, 690);
        ofScale(-1, 1);
        StaticImgLoader.draw(0, 0, 25, 25);
        ofPopMatrix();
        Button.set(5, 685, 35, 35);
        ButtonsCont.push_back(make_pair(AllButtons::PrevPage, Button));
    }
    if (Offset + Limit < StarterPoke.size()) {
        StaticImgLoader.draw(1000, 690, 25, 25);
        Button.set(995, 685, 35, 35);
        ButtonsCont.push_back(make_pair(AllButtons::NextPage, Button));
    }

    if (PokemonLeft < 6){
        ofSetColor(0);
        ofFill();
        ofDrawRectRounded(270, 685, 500, FrameY, 5);
        ofSetColor(0, 130, 255);
        ofFill();
        ofDrawRectRounded(273, 688, 494, FrameY, 5);
        ofSetColor(255);
        Button.set(270, 685, 500, FrameY);
        ButtonsCont.push_back(make_pair(AllButtons::Start, Button));
        Font_Selector.load("Jura-Medium.ttf", 20);
        Font_Selector.drawString("START", 480, 715);
    }
}

void ofApp::DrawPage3() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    OF_EXIT_APP(0);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//-----------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    cout << "Pressed: x-" << x << " | y-" << y << endl;
    if (button != 0) return; //check if left click return if not (no right clicking in my app)
    ofxJSONElement TempAPI;
    string APIstr;

    AllButtons Pressed = AllButtons::none;
    for (int i = 0; i < ButtonsCont.size(); i++) {
        if (ButtonsCont[i].second.inside(x, y)) {
            Pressed = ButtonsCont[i].first; //its an enum so idk
            break;
        }
    }

    switch (Pressed) {
    case AllButtons::none:
        return;
    case AllButtons::Click2Play:
        CurrPage = Pages::PickPokemon;
        TopRightBtn.load("Icon_Back.png");
        PokemonLeft = 6;
        PokeSelected.clear();
        Offset = 0;
        break;
    case AllButtons::TopRightBtn:
        switch (CurrPage) {
        case Pages::MainMenu:
            OF_EXIT_APP(0);
        case Pages::PickPokemon:
            CurrPage = Pages::MainMenu;
            StaticImgLoader.load("PokeBattle_Logo.png");
            TopRightBtn.load("Icon_Close.png");
            break;
        case Pages::BattleScene:
            break;
        }
        break;

    case AllButtons::DifficultyBtn:
        DifficultyOption++;
        switch (DifficultyOption) {
        case 1:
            DifficultyBtn.load("Icon_Diffculty_Medium.png");
            break;
        case 2:
            DifficultyBtn.load("Icon_Diffculty_Hard.png");
            break;
        case 0: default:
            DifficultyOption = 0;
            DifficultyBtn.load("Icon_Diffculty_Easy.png");
            break;
        }
        break;
    case AllButtons::PrevPage:
        Offset -= Limit;
        if (Offset < 0) Offset = 0;
        break;
    case AllButtons::NextPage:
        if (Offset + Limit > StarterPoke.size()) break;
        Offset += Limit;
        break;
    case AllButtons::PokeSelectGrid:
        if (PokeSelected.size() >= 6) {
            if(PokeSelected.size() > 6) PokeSelected.resize(6); //hate to bug out this way
            break;
        }
        x -= 10; y -= 180;
        x /= FrameX; y /= FrameY;
        if ((y * Columns + x + 1 + Offset) > StarterPoke.size()) break;
        PokeSelected.push_back(StarterPoke[y * Columns + x + Offset].PokeNum);
        PokemonLeft = 6 - PokeSelected.size();
        break;
    case AllButtons::PokeSelected:
        if (PokeSelected.size() == 0) break;
        y -= 180;
        y /= ((ScreenY - 180) / 6);
        if (PokeSelected.size() < y+1) break;
        PokeSelected.erase(PokeSelected.begin()+y);
        PokemonLeft = 6 - PokeSelected.size();
        break;
    case AllButtons::Start:
        CurrPage = Pages::BattleScene;
        TopRightBtn.load("Icon_Pause.png");
        PokemonLeft = 6;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}