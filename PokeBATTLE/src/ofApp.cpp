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
    CurrPoke = -1; //Pokeball

    StaticImgLoader.load("PokeBattle_Logo.png");
    Click2Play.load("Click2Play.png");
    TopRightBtn.load("Icon_Close.png");
    DifficultyBtn.load("Icon_Diffculty_Easy.png");
    PokeBall.load("sprites/PokeBall.png");

    ofxJSONElement TempAPI;
    string APIstr;
    string tempName; int tempNum; 
    string tempPokeType1, tempPokeType2;
    PokeSelected.reserve(6);

    int MaxSelection = 74; //PLEASE CHANGE THIS VALUE, LOW FOR TESTING (HIGHER=SLOWER-LOAD)
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
    ofSetFrameRate(60);
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
    //cout << "page rendered\n"; // So ive come to realise using a static IMG saver to save on memory wasnt the right call... far too late to do much about it tho :/
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

    TopRightBtn.draw(1210, 15, TopRightBtn.getWidth(), 50);
    Button.set(1210, 15, TopRightBtn.getWidth(), 50);
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

    TopRightBtn.draw(1200, 15, TopRightBtn.getWidth(), 50);
    Button.set(1200, 15, TopRightBtn.getWidth(), 50);
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
        PokeBall.draw(20, ((ScreenY - 180) / 6) * i+10, (ScreenY - 180) / 6, (ScreenY - 180) / 6);
        StaticImgLoader.draw(75, ((ScreenY - 180) / 6) * i - 25, 160, 160);
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
            for (int k = 0; k < PokeSelected.size(); k++) {
                if (PokeSelected[k] == StarterPoke[i * Columns + j + Offset].PokeNum) Accent1.set(0, 130, 255);
            }

            StaticImgLoader.load(PokeLocation);
            ofSetColor(0);
            ofFill();
            ofDrawRectRounded((FrameX + Gap) * j, (FrameY + Gap) * i, FrameX, FrameY, 4);
            ofSetColor(Accent1);
            ofFill();
            ofDrawRectRounded(((FrameX + Gap) * j) + 3, ((FrameY + Gap) * i) + 3, FrameX - 6, FrameY - 6, 3);
            ofSetColor(255);
            StaticImgLoader.draw((FrameX + Gap) * j, (FrameY + Gap) * i, FrameX, FrameY);

            StaticImgLoader.load("sprites/Types/" + StarterPoke[i * Columns + j + Offset].strPokeTyping.first + ".png");
            StaticImgLoader.draw((FrameX + Gap) * j, (FrameY + Gap) * i+5, 60, 20);
            StaticImgLoader.load("sprites/Types/" + StarterPoke[i * Columns + j + Offset].strPokeTyping.second + ".png");
            StaticImgLoader.draw((FrameX + Gap) * j +FrameX-60, (FrameY + Gap) * i +FrameY-25, 60, 20);
            ofSetColor(0);
            Font_Selector.load("TurpisItalic.ttf", 12);
            Font_Selector.drawString(StarterPoke[i * Columns + j + Offset].PokeName, (FrameX + Gap) * j + 5, (FrameY + Gap) * i + FrameY - 5);
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
    ofRectangle Button;
    Accent1.set(75, 75, 75);
    Accent2.set(75, 75, 75);
    DrawBG(Accent1, Accent2);


    ofSetColor(255, 255, 255);
    Font_Selector.load("TurpisItalic.ttf", 40);
    Font_Selector.drawString("Poke-Streak:", 39, 60);

    Font_Selector.load("TurpisItalic.ttf", 24);
    Font_Selector.drawString("Hints:", 34, 90);

    ofSetColor(255, 255, 255);
    Font_Selector.load("TurpisItalic.ttf", 36);
    Font_Selector.drawString("Hi-Sco:", 25, 130);

    ofSetColor(222, 90, 58);
    Font_Selector.load("TurpisItalic.ttf", 96);
    Font_Selector.drawString(to_string(PokeStreak), 380, 110);

    Font_Selector.load("TurpisItalic.ttf", 55);
    Font_Selector.drawString(to_string(0000), 195, 133);

    Font_Selector.load("TurpisItalic.ttf", 26);
    Font_Selector.drawString(to_string(HintsRemaining), 132, 92);

    ofSetColor(0);
        ofFill();
        ofDrawRectRounded(500, 75, 340, 125, 20);
    ofSetColor(75);
        ofFill();
        ofSetCircleResolution(30);
        ofDrawEllipse(1030, 310, 350, 120);
        ofDrawEllipse(260, 690, 480, 180);
    ofSetColor(255);
        ofDrawRectRounded(502, 77, 336, 121, 18);
        ofDrawEllipse(1028, 308, 350, 120);
        ofDrawEllipse(256, 686, 480, 180);
    
    PokeBall.draw(120, 500, 250, 250);

    ofPushMatrix();
    ofTranslate(750, 500);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            if ((i * 3 + j + 1) > PokeTeam.size()) break;
            string PokeLocation = "sprites/" + to_string(PokeTeam[i * 3 + j].PokeNum) + "/front.png";
                
            StaticImgLoader.load(PokeLocation);
            ofSetColor(0);
            ofFill();
            ofDrawRectRounded((FrameX + Gap) * j, (FrameY + Gap) * i, FrameX, FrameY, 4);
            ofSetColor(255);
            ofFill();
            ofDrawRectRounded(((FrameX + Gap) * j) + 3, ((FrameY + Gap) * i) + 3, FrameX - 6, FrameY - 6, 3);
            StaticImgLoader.draw((FrameX + Gap) * j, (FrameY + Gap) * i, FrameX, FrameY);

            StaticImgLoader.load("sprites/Types/" + PokeTeam[i * 3 + j].strPokeTyping.first + ".png");
            StaticImgLoader.draw((FrameX + Gap) * j, (FrameY + Gap) * i + 5, 60, 20);
            StaticImgLoader.load("sprites/Types/" + PokeTeam[i * 3 + j].strPokeTyping.second + ".png");
            StaticImgLoader.draw((FrameX + Gap) * j + FrameX - 60, (FrameY + Gap) * i + FrameY - 25, 60, 20);
            ofSetColor(0);
            Font_Selector.load("TurpisItalic.ttf", 12);
            Font_Selector.drawString(PokeTeam[i * 3 + j].PokeName, (FrameX + Gap) * j+5, (FrameY + Gap) * i + FrameY - 5);
        }
    }
    ofPopMatrix();
    ofSetColor(255);
    Button.set(1210, 15, TopRightBtn.getWidth(), 50);

    ofPushMatrix();
    ofTranslate(680, 500);
        if (RevivePotion.first) {
            ofSetColor(230,230, 125);
            ofDrawRectangle(5, 0, 60, 100);
            ofSetColor(255);
            ofDrawRectangle(7, 2, 56, 96);
        } else if (RevivePotion.second) {
            ofSetColor(130,100,175);
            ofDrawRectangle(5, 100, 60, 100);
            ofSetColor(255);
            ofDrawRectangle(7, 102, 56, 96);
        }

        StaticImgLoader.load("sprites/Revive.png");
        StaticImgLoader.draw(0, 0, 70, 70);
        StaticImgLoader.load("sprites/Potion.png");
        StaticImgLoader.draw(10, 110, 50, 50);
        ofSetColor(0);
        Font_Selector.load("TurpisItalic.ttf", 24);
        Font_Selector.drawString(to_string(ReviveCount), 22, 90);
        Font_Selector.drawString(to_string(PotionCount), 22, 190);
    ofPopMatrix();
    ofSetColor(255);
    Button.set(685, 500, 60, 200);
    ButtonsCont.push_back(make_pair(AllButtons::RevivePotion, Button));



    if (GamePaused) {
        ButtonsCont.clear();
        TopRightBtn.load("Icon_Pause.png");
        TopRightBtn.draw(525, 200, 235, 300);
        Button.set(525, 200, 235, 300);
        ButtonsCont.push_back(make_pair(AllButtons::TopRightBtn, Button));
        TopRightBtn.load("Icon_Play.png");
    }
    TopRightBtn.draw(1210, 15, TopRightBtn.getWidth(), 50);
    Button.set(1210, 15, TopRightBtn.getWidth(), 50);
    ButtonsCont.push_back(make_pair(AllButtons::TopRightBtn, Button));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    cout << key << endl;
    if ((key == 113 || key == 81) && (CurrPage == Pages::MainMenu || GamePaused)) {
        OF_EXIT_APP(0);
    }
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
        //if (CurrPage == Pages::BattleScene && (RevivePotion.first || RevivePotion.second)) {
        //    RevivePotion.first = false;
        //    RevivePotion.second = false;
        //}
        break;
    case AllButtons::Click2Play:
        CurrPage = Pages::PickPokemon;
        TopRightBtn.load("Icon_Back.png");
        PokemonLeft = 6;
        //PokeSelected.clear();
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
            if (GamePaused) {
                GamePaused = false;
                TopRightBtn.load("Icon_Pause.png");
            }
            else GamePaused = true;
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
        PokemonLeft = 6; ReviveCount = 5; PotionCount = 10;
        RevivePotion = make_pair(false, false);
        GamePaused = false;

        for (int i: PokeSelected) {
            TempAPI.open("https://pokeapi.co/api/v2/pokemon/" + to_string(i+1));
            string Type2 = "none";
            if (TempAPI["types"].size() > 1) Type2 = TempAPI["types"][1]["type"]["name"].asString();
            string tempStr = TempAPI["name"].asString();
            Pokemon tempPokemon = {
                TempAPI["name"].asString(), i,
                TempAPI["types"][0]["type"]["name"].asString(),
                Type2
            };
            PokeTeam.push_back(tempPokemon);
            cout << i << " - ";
            cout << TempAPI["name"].asString() << endl;
        }
        break;
    case AllButtons::RevivePotion:
        y -= 550;
        y /= (ScreenY - 550) / 2;
        if (RevivePotion.first) {
            RevivePotion.first = false;
            if (PotionCount > 0 && y == 1) RevivePotion.second = true;
        } else if (RevivePotion.second) {
            RevivePotion.second = false;
            if (ReviveCount > 0 && y == 0) RevivePotion.first = true;
        }
        else if (y == 0 && ReviveCount > 0) RevivePotion.first = true;
        else if (y == 1 && PotionCount > 0) RevivePotion.second = true;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}