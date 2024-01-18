#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    /*
     API documentation can be found at: https://pokeapi.co/docs/v2
    */
    PokeAPI.open("https://pokeapi.co/api/v2/berry/cheri");//returns data on the Cheri Berry
    cout << PokeAPI.getRawString() << endl;

    PokeStreak = 0000;
    HintsRemaining = 0;
    HighScore = 0000;
    CurrPage = Pages::MainMenu;
    DifficultyOption = 0;

    DifficultyBtn.load("Icon_Diffculty_Easy.png");
    Click2Play.load("Click2Play.png");
    TopRightBtn.load("Icon_Close.png");
    DrawAnImage();
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    int CurrHeight = ofGetHeight(), CurrWidth = ofGetWidth();
    int PaddingPercent[4] = {0,0,0,0}; // 0-top 1-right 2-bottom 3-left - clockwise [moreso a nudge function but oh well... values are used as percentages]
    
    ofRectangle NewButton;
    Buttons.clear();

    switch (CurrPage) {
    case Pages::MainMenu:
        Accent1.set(255, 203, 5);
        Accent2.set(50, 105, 177);

        DrawBG(Accent1, Accent2);

        StaticImgLoader.load("PokeBattle_Logo.png");
        PaddingPercent[0] = 15; //set padding before going into function - it gets reset within the function... hurray pointers
        DrawImage(StaticImgLoader, 90, 0, 'c', 't', &PaddingPercent[0]); //fix tomorrow getting late late

        PaddingPercent[2] = 33;
        NewButton = DrawImage(Click2Play, 30, 0, 'c', 'b', &PaddingPercent[0]);
        Buttons.push_back(make_pair(AllButtons::Click2Play, NewButton));

        PaddingPercent[0] = 2;
        PaddingPercent[1] = 1;
        NewButton = DrawImage(TopRightBtn, 4, 0, 'r', 't', &PaddingPercent[0]);
        Buttons.push_back(make_pair(AllButtons::TopRightBtn, NewButton));

        PaddingPercent[1] = 1;
        PaddingPercent[2] = 33;
        NewButton = DrawImage(DifficultyBtn, 14, 0, 'r', 'b', &PaddingPercent[0]);
        Buttons.push_back(make_pair(AllButtons::DifficultyBtn, NewButton));

        break;

    case Pages::PickPokemon:
        Accent1.set(75, 75, 75);
        Accent2.set(75, 75, 75);

        DrawBG(Accent1, Accent2);

        StaticImgLoader.load("PokeBattle_Logo.png");
        PaddingPercent[0] = 3;
        PaddingPercent[3] = 5;
        DrawImage(StaticImgLoader, 50, 0, 'l', 't', &PaddingPercent[0]);

        PaddingPercent[0] = 2;
        PaddingPercent[1] = 1;
        NewButton = DrawImage(TopRightBtn, 6, 0, 'r', 't', &PaddingPercent[0]);
        Buttons.push_back(make_pair(AllButtons::TopRightBtn, NewButton));
        break;

    case Pages::BattleScene:
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
        break;

    default:
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

ofRectangle ofApp::DrawImage(ofImage aImage, int widthPercent, int heightPercent, char alignX, char alignY, int* PaddingPtr) {
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
        PaddingLeft = (CurrWidth - ((widthPercent * CurrWidth) / 100)) + PaddingLeft; //if right then get whole screen minus size of element
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
        PaddingTop = (CurrHeight - ((heightPercent * CurrHeight) / 100)) + PaddingTop;
        break;
    case 't': default:
        break;
    }   

    aImage.draw(PaddingLeft, PaddingTop, ((widthPercent * CurrWidth) / 100), ((heightPercent * CurrWidth) / 100));

    ofRectangle ImgLocation;
    ImgLocation.set(PaddingLeft, PaddingTop, ((widthPercent * CurrWidth) / 100), ((heightPercent * CurrWidth) / 100));
    return ImgLocation;
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
    if (button != 0) return; //check if left click return if not (no right clicking in my app)
    
    AllButtons Pressed = AllButtons::none;
    for (int i = 0; i < Buttons.size(); i++) {
        if (Buttons[i].second.inside(x, y)) {
            Pressed = Buttons[i].first; //its an enum so idk
            break;
        }
    }
    
    switch (Pressed) {
    case AllButtons::none: 
        return;
    case AllButtons::Click2Play:
        CurrPage = Pages::PickPokemon;
        TopRightBtn.load("Icon_Back.png");
        break;
    case AllButtons::TopRightBtn:
        switch (CurrPage) {
        case Pages::MainMenu:
            OF_EXIT_APP(0);
        case Pages::PickPokemon:
            CurrPage = Pages::MainMenu;
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
            //DifficultyBtn.load("Icon_Diffculty_Medium_Alt.png");
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
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}