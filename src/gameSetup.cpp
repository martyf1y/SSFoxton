//
//  gameSetup.cpp
//  SSFoxtonV0_1
//
//  Created by Mini on 2/09/17.
//
//

#include "ofApp.h"


void ofApp::resetPassive(){
    gameModeObject.currentEffectSound = gameModeObject.soundEffectArray[0];
    
    if(gameModeObject.currentCommandSound.isPlaying()) {
        gameModeObject.currentCommandSound.stop();
    }
    if(gameModeObject.currentEffectSound.isPlaying()) {
        gameModeObject.currentEffectSound.stop();
    }
    if(gameModeObject.currentEffectSound2.isPlaying()) {
        gameModeObject.currentEffectSound2.stop();
    }
    if(gameModeObject.timerSound.isPlaying()) {
        gameModeObject.timerSound.stop();
    }
      //  gameModeObject.passiveImageContent[i].clear();
        gameModeObject.kinect.contentImage.clear();
        gameModeObject.kinect.contentImage.allocate(dimWidth, dimHeight, OF_IMAGE_COLOR_ALPHA);
        int randPassive = ofRandom(0,gameModeObject.amountOfPictureChoice); // NO MOUNTAINS! Ok we can have mountains...
      //  int randPassive = 2;
       // int heightPassive = ofRandom(0, dimHeight*0.13);
      //  float scalePassive = ofRandom(0.5, 0.9);
    float scalePassive = 0.75;

        // 2 5 6 7 8 9 10
        // 0 1 3 6
  //  if(scalePassive>0.75){
  //      heightPassive = 0;
  //  }
        int heightPassive = 0;
        // gameModeObject.blacksImageArray[randPassive]
        gameModeObject.kinect.contentImage = gameModeObject.loadAnImage(3, randPassive, 0, heightPassive, scalePassive);
    // gameModeObject.kinect.giveTheImagesToMe(gameModeObject.currentContentImage, i);
    for (int i=0; i<gameModeObject.gameModeGUI.numLanguages; i++) {
        gameModeObject.gameModeGUI.shapeNameWithLanguages[i] = ofSplitString(gameModeObject.gameModeGUI.shapeNamesListed[randPassive], ",")[i];
    }
    for (int x = gameModeObject.kinect.arrayDustParticles.size ()-1; x > -1; x--) {
        // Remove all pixels
        gameModeObject.kinect.arrayDustParticles.erase(gameModeObject.kinect.arrayDustParticles.begin()+x);
        // delete particles;
    }
    for(int i = 0; i < 217088; i++){
        gameModeObject.kinect.depthParticleBool[i] = false;
    }
}


void ofApp::loadImages(int totsContent){
    
    
    titleImg.load("0.title/title.png");
    titleImg.resize(titleImg.getWidth()*gameModeObject.adjustSizeX*0.5, titleImg.getHeight()*gameModeObject.adjustSizeY*0.5);
    gameModeObject.endOfRoundBackground.load("1.background/" + ofToString(0) + "bg.png");
    
    gameModeObject.competitiveMenuImages.load("0.title/sil_teamsThumb.png");
    gameModeObject.simonMenuImages[0].load("0.title/silhouetteTemp1_versus.png");
    gameModeObject.simonMenuImages[1].load("0.title/silhouetteTemp2_Coop.png");
    
    ruminationBackground.load("1.background/" + ofToString(1) + "bg.png");

    ofImage newContentImage;
    for(int i = 1; i <= totsContent; i++){
        newContentImage.load("2.content/" + ofToString(i) + ".png");
        newContentImage.resize(dimWidth, dimHeight);
        gameModeObject.contentImageArray.push_back(newContentImage);
        newContentImage.load("3.freeZone/" + ofToString(i) + "_Safe.png");
        newContentImage.resize(dimWidth, dimHeight);
        gameModeObject.noFillImageArray.push_back(newContentImage);
        newContentImage.load("4.thumbnail/" + ofToString(i) + "Thumb.png");
        int newX = newContentImage.getWidth()*gameModeObject.adjustSizeX;
        int newY = newContentImage.getHeight()*gameModeObject.adjustSizeY;
        
        newContentImage.resize(newX, newY); // This keeps them the correct size in relation to everything else
        gameModeObject.thumbnailImageArray.push_back(newContentImage);
        newContentImage.load("5.blacks/" + ofToString(i) + ".png");
        newContentImage.resize(dimWidth, dimHeight);
        gameModeObject.blacksImageArray.push_back(newContentImage);
    }
}

void ofApp::loadSounds(){
     for(int i = 0; i < 14; i++){
         gameModeObject.soundCommandsArray[i].load("8.sounds/" + ofToString(i)+".mp3");
     }
    // 0.get ready
    // 1.team 1 get ready
    // 2.team 2 get ready
    // 3.team 1 make a shadow
    // 4.team 2 make a shadow
    // 5.team 1 fill in the shape
    // 6.team 2 fill in the shape
    // 7.team 1 wins!
    // 8.team 2 wins!
    // 9.game over
    // 10.go
    // 11.the round
    // 12.the game
    // 13.it is a draw

    
    
    for(int i = 0; i < 5; i++){
        gameModeObject.soundEffectArray[i].load("9.soundEffects/" + ofToString(i)+".mp3");
    }
    gameModeObject.timerSound = gameModeObject.soundEffectArray[2];
    // 0.clikity clikity passive
    // 1.whoosh
    // 2.clock tick
    // 3.completedshadow
    // 4.polaroidcapture
}

void ofApp::loadFont(){
    
    screenTitleFont.load("Atlas Grotesk Web-Bold.ttf", 32*gameModeObject.screenAdjustBoth);
    screenModeFont.load("AtlasGrotesk-Regular.otf", 18*gameModeObject.screenAdjustBoth);
    
    /// DO NOT CHANGE THESE. THEY BELONG TO MANY THINGS
    gameModeObject.scoreFont.load("AtlasGrotesk-Regular.otf", 13*gameModeObject.screenAdjustBoth);
    gameModeObject.guiScoreFont.load("AtlasGrotesk-Regular.otf", 60*gameModeObject.screenAdjustBoth);
    gameModeObject.guiRoundFont.load("AtlasGrotesk-Regular.otf", 20*gameModeObject.screenAdjustBoth);
    gameModeObject.scoreNameFonts.load("AtlasGrotesk-Regular.otf", 12*gameModeObject.screenAdjustBoth);
    gameModeObject.gameModeGUI.descriptionParagraph.setFont(ofxSmartFont::add("AtlasGrotesk-Regular.otf",gameModeObject.descriptionFontSize,"AtlasGrotesk-Medium"));
    
    gameModeObject.timerFont.load("Atlas Grotesk Web-Bold.ttf", 32*gameModeObject.screenAdjustBoth);

    gameModeObject.gameModeGUI.shapeNamesFont.load("AtlasGrotesk-Regular.otf", 28*gameModeObject.screenAdjustBoth);
}


void ofApp::loadScoreboardImages(){
    
    int numTeams = 3; // if you change number of teams, make sure to change array sizes
    
    for(int i = 0; i < numTeams; i++){
           gameModeObject.gameModeGUI.scoreboardQuickplay[i].load("6.scoreboard/minimal_"+ ofToString(i) + ".png");
        int newX = gameModeObject.gameModeGUI.scoreboardQuickplay[i].getWidth()*gameModeObject.adjustSizeX;
        int newY = gameModeObject.gameModeGUI.scoreboardQuickplay[i].getHeight()*gameModeObject.adjustSizeY;
        gameModeObject.gameModeGUI.scoreboardQuickplay[i].resize(newX, newY); // This keeps them the correct size in relation to everything else
        
        gameModeObject.gameModeGUI.scoreboardSurvival[i].load("6.scoreboard/round_"+ ofToString(i) + ".png");
        
         newX = gameModeObject.gameModeGUI.scoreboardSurvival[i].getWidth()*gameModeObject.adjustSizeX;
         newY = gameModeObject.gameModeGUI.scoreboardSurvival[i].getHeight()*gameModeObject.adjustSizeY;
        gameModeObject.gameModeGUI.scoreboardSurvival[i].resize(newX, newY); // This keeps them the correct size in relation to everything else
        
           gameModeObject.gameModeGUI.scoreboardCompetitive[i].load("6.scoreboard/full_"+ ofToString(i) + ".png");
        newX = gameModeObject.gameModeGUI.scoreboardCompetitive[i].getWidth()*gameModeObject.adjustSizeX;
        newY = gameModeObject.gameModeGUI.scoreboardCompetitive[i].getHeight()*gameModeObject.adjustSizeY;
        gameModeObject.gameModeGUI.scoreboardCompetitive[i].resize(newX, newY); // This keeps them the correct size in relation to everything else
        

    }
    gameModeObject.winnerCup.load("6.scoreboard/winCupShadow.png");

    // The names for the shapes
    ofBuffer buffer = ofBufferFromFile("7.data/languageNames.txt");
    for (auto line : buffer.getLines()){
        
        gameModeObject.gameModeGUI.shapeNamesListed.push_back(line);
           cout << line << endl;    // check the text
    }
}

void ofApp::loadHighscoreImages() {
 
    // load image highscores from text file on start up
    
    int count = 0; // to keep track of how many lines in file (hopefully 12)
    vector<string> array; // temp vector used in loop
    
    try {
        ofBuffer buffer = ofBufferFromFile("7.data/highscoresImages.txt");
       // cout << buffer.size() << endl;
            for (auto line : buffer.getLines()){
                array.push_back(line);
                gameModeObject.highscoreCurrentImage[count] = ofToInt(array[count]);
                count ++;
            }
    } catch(exception& e){
        cout << "ERROR: couldn't load image highscores" << endl;
    }
}

void ofApp::loadHighscoreSurvival(){

    // load survival highscore from text file on start up
    try {
        ofBuffer buffer = ofBufferFromFile("7.data/highscoreSurvival.txt");
      //  cout << buffer.size() << endl;
        for (auto line : buffer.getLines()){
            gameModeObject.highscoreCurrentSurvival = ofToInt(line); // just get the last thing in the text file
            cout << "highscore survival: " << line << endl;
        }
    } catch(exception& e){
        cout << "ERROR: couldn't load survival highscore" << endl;
    }
    try {
        ofBuffer buffer = ofBufferFromFile("7.data/highscoreSurvivalDate.txt");
        //  cout << buffer.size() << endl;
        for (auto line : buffer.getLines()){
            gameModeObject.highscoreCurrentDate = line; // just get the last thing in the text file
            cout << "highscore survival date: " << line << endl;
        }
    } catch(exception& e){
        cout << "ERROR: couldn't load survival highscore" << endl;
    }
    
}

