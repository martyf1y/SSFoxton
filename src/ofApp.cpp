#include "ofApp.h"

/*
 The different sections to our structure
 
 Setup - Initial setup and when we need to reset modes
 Timer - Includes any timer triggers
 Modes - Different game modes, most likely in a class
 - display buttons with their own set positions and whatnot
 File information - anything that needs to be recorded or displayed
 GUI/Buttons - Display things like titles, buttons, debug,
 */


//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofSetFrameRate(30);
    // ofAddListener(ofEvents().mouseReleased, this, &ofApp::mouseRes);
     gameModeObject.adjustSizeX = float(dimWidth)/1920;
     gameModeObject.adjustSizeY = float(dimHeight)/1620;
    gameModeObject.screenAdjustBoth = gameModeObject.adjustSizeX * gameModeObject.adjustSizeY * 2;

    // Set up server & port * * * * * * * * * * * * * * * * * * * *
    bool connected = server.setup(serverPort);
    cout<< "Connected as server: " << connected << endl;
    
    try {
        ofBuffer buffer = ofBufferFromFile("7.data/clientIP.txt");
        for (auto line : buffer.getLines()){
            authorisedIPAddress = line; // just get the last thing in the text file
            cout<< "Client should be on: " << authorisedIPAddress << endl;
        }
    } catch (exception& e) {
        cout << "ERROR: can't read / load client IP text file" << endl;
    }
    // * * * * * * * * * * * * * * * * * * * *
    
    ofSetVerticalSync(true);
    gameModeObject.gameModeSetup();
    // Setup Kinect
    ofSetLogLevel(OF_LOG_VERBOSE);
        
    /* Setup GUI, fonts and Debug
     Setup Content
     sounds
     silhoette images
     colour palette
     backgroud
     button images and textures
     Setup file information
     Check tablet is connected
     */
    loadImages(12);    // Load content images, the number is the total amount we have
    loadSounds();    // Load content sounds
    loadFont();
    loadScoreboardImages();
    loadHighscoreImages();
    loadHighscoreSurvival();
    backgroundCol = ofColor(63,67,120); // Purple
    depthCol = ofColor(255,203, 55); // Yellow
    resetPassive();
    currentGameState = passive;
}

//--------------------------------------------------------------
void ofApp::update(){
    (gameModeObject.tabletInteract?ofHideCursor():ofShowCursor());
    sendReceiveTCPMessages(); // Receive things and send

    // This is to keep all dimensions the same. 
    dimWidth = gameModeObject.gameModeGUI.dimWidth = gameModeObject.dimWidth;
    dimHeight = gameModeObject.gameModeGUI.dimHeight = gameModeObject.dimHeight;
    
    mainTimerGoing(120000); // This timer resets us back to Passive mode // CHANGE
    
    switch ( currentGameState ) {
        case passive:{
            gameModeObject.kinect.doThePassiveCheck = true; // Gets the Passive filler all going
            

          //  float hue = fmodf(ofGetElapsedTimef()+200,255);
          //  bgColor.setHsb( hue, 255, 255);
            
            if(startButt){
                currentGameState = selective;
                //changeColor();
                startButt = false;
             //   currBackground = bgImage[1];
                for (int x = gameModeObject.kinect.arrayDustParticles.size ()-1; x > -1; x--) {
                    // Remove all pixels
                        gameModeObject.kinect.depthParticleBool[gameModeObject.kinect.arrayDustParticles[x].indexPos] = false;
                        gameModeObject.kinect.arrayDustParticles.erase(gameModeObject.kinect.arrayDustParticles.begin()+x);
                        // delete particles;
                }
            }

        }break;
        case selective:{
            ////// So far the background colour changes, not much else, all in draw
           // float hue = fmodf(ofGetElapsedTimef()+600,255);
           // bgColor.setHsb( hue, 255, 255);
            
        }break;
        case active:{
            //// This is the timer for the game mode. Every time the gameTimeHasFinished is false we want to use it again.
            if(!gameModeObject.gameTimeHasFinished){
                gameModeObject.gameTimeHasFinished = gameTimerGoing(gameModeObject.gameEndTime);
            }
            else{
                gameModeObject.activeStartTime = ofGetElapsedTimeMillis(); // Look we keep the timer fresh!
                gameModeObject.showTimer = false;
            }
            
            
            
            
        }break;
        case ruminative:{
        }break;
        default:{
            cout<<"Error, where are we in the update cases?\n";
        }break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
   // cout << ofGetLastFrameTime() << endl;
    ofBackground(backgroundCol);
    ofSetRectMode(OF_RECTMODE_CENTER);

  //  ofSetColor(255);
  //  currBackground.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);
       switch ( currentGameState ) {
        case passive:{
           //   ofSetColor(backgroundCol);  // Passive background
           //   currBackground.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);


            
            //// Include basic depth
            ofPushStyle();
            ofSetColor(depthCol);
            gameModeObject.basicKinect(false); // Kinect
            ofPopStyle();
            // pixels particles;

            if(previousPassiveParticleSize < (gameModeObject.kinect.arrayDustParticles.size())){
                previousPassiveParticleSize = gameModeObject.kinect.arrayDustParticles.size()+200;
               // cout << "E" << endl;
                gameModeObject.currentEffectSound.play();
            }
            if(previousPassiveParticleSize-400 >= (gameModeObject.kinect.arrayDustParticles.size())){
                previousPassiveParticleSize = gameModeObject.kinect.arrayDustParticles.size();
                
            }
            for (int x = gameModeObject.kinect.arrayDustParticles.size()-1; x > -1; x--) {
                // Simulate and draw pixels
                gameModeObject.kinect.arrayDustParticles[x].move();
                gameModeObject.kinect.arrayDustParticles[x].draw();
                // Remove any dead pixels out of bounds
                if (gameModeObject.kinect.arrayDustParticles[x].pos.x < 0 || gameModeObject.kinect.arrayDustParticles[x].pos.x > dimWidth || gameModeObject.kinect.arrayDustParticles[x].pos.y < 0 || gameModeObject.kinect.arrayDustParticles[x].pos.y > dimHeight) {
                    gameModeObject.kinect.depthParticleBool[gameModeObject.kinect.arrayDustParticles[x].indexPos] = false;
                    gameModeObject.kinect.arrayDustParticles.erase(gameModeObject.kinect.arrayDustParticles.begin()+x);
                    // delete particles;
                }
            }
            ofSetColor(255); // ofColor(0,243,146);
                gameModeObject.kinect.contentImage.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);
            gameModeObject.gameModeGUI.drawNameOfShape(ofColor(0));

          //  ofSetColor(243,146,0, 190);
           // gameModeObject.kinect.postPixelCheckImage.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);
            
            
                       /// include random images
            /// have cool fire pixel thing happening
            
            
            // Once this button is tapped we move onto the selective screen
          //  if(!gameModeObject.tabletInteract){
                ofSetColor(0);
                int strokeSize = 8*gameModeObject.screenAdjustBoth;
                gameModeObject.gameModeGUI.roundedRect(dimWidth*0.25-dimWidth*0.225-strokeSize*0.5, dimHeight*0.4-dimHeight*0.035-strokeSize*0.5, dimWidth*0.45+strokeSize, dimHeight*0.07+strokeSize, strokeSize);
               startButt = gameModeObject.gameModeGUI.button("Touch tablet to start", screenModeFont, dimWidth*0.25, dimHeight*0.4,  dimWidth*0.45, dimHeight*0.07, strokeSize, leftMouseReleased, buttonColorHigh, buttonColorLow, startButt, gameModeObject.tabletInteract);
           // gameModeObject.gameModeGUI.notButton("Select a game mode on the tablet", screenModeFont, dimWidth*0.75, dimHeight*0.4, dimWidth*0.45, dimHeight*0.07, ofColor(225), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

            
            gameModeObject.gameModeGUI.roundedRect(dimWidth*0.75-dimWidth*0.225-strokeSize*0.5, dimHeight*0.4-dimHeight*0.035-strokeSize*0.5, dimWidth*0.45+strokeSize, dimHeight*0.07+strokeSize, strokeSize);
            startButt = gameModeObject.gameModeGUI.button("Touch tablet to start", screenModeFont, dimWidth*0.75, dimHeight*0.4,  dimWidth*0.45, dimHeight*0.07, strokeSize, leftMouseReleased, buttonColorHigh, buttonColorLow, startButt, gameModeObject.tabletInteract);
         //   }
           
            // -------------------Title ------------------- //
            ofSetColor(255);
            titleImg.draw(dimWidth*0.25, dimHeight/6);

        }break;
        case selective:{
         //   ofSetColor(backgroundCol); // This is the background
         //   bgImage[2].draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);
            
            //// Include basic depth
            ofPushStyle();
            ofSetColor(depthCol);
            gameModeObject.basicKinect(false); // Kinect
            ofPopStyle();
            
            int localCheckMode = currentModeINT;
            currentModeINT = gameModeObject.modeSelect(screenModeFont, leftMouseReleased, currentModeINT);
            
            if(localCheckMode != currentModeINT){ // Move onto next case
                gameModeObject.reset(currentModeINT, true);
            }
            
            switch ( currentModeINT) { // Use an int to select case mode
                case competitive:{
                    gameModeObject.competitiveSelect(screenModeFont, leftMouseReleased);
                }break;
                case survival:{
                    gameModeObject.survivalSelect(screenModeFont, leftMouseReleased);
                }break;
                case simonSays:{
                    gameModeObject.simonSaysSelect(screenModeFont, leftMouseReleased);
                }break;
                case quickPlay:{
                    gameModeObject.quickplaySelect(screenModeFont, leftMouseReleased);
                }break;
                default:{
                    
                  //  cout<<"Error, where are we in the modes?\n";
                }break;
            }
            //////////////// BUTTONS ///////////////////
            if(currentModeINT!=99){
                 if(!gameModeObject.tabletInteract){
                int strokeSize = 8*gameModeObject.screenAdjustBoth;
                ofSetColor(0);
                gameModeObject.gameModeGUI.roundedRect(dimWidth*0.25-dimWidth*0.035-strokeSize*0.5, dimHeight*0.9-dimHeight*0.035-strokeSize*0.5, dimWidth*0.07+strokeSize, dimHeight*0.07+strokeSize, strokeSize);
                startingGame = gameModeObject.gameModeGUI.button("Play!", screenModeFont, dimWidth*0.25, dimHeight*0.9,  dimWidth*0.07, dimHeight*0.07, strokeSize, leftMouseReleased, buttonColorHigh, buttonColorLow, startingGame, gameModeObject.tabletInteract);
                 }
            }
            else{
              //  gameModeGUI.drawMessageGUI(, modeFont, dimWidth*0.75, dimHeight*0.05, ofColor(255,255,255));
                gameModeObject.gameModeGUI.notButton("Select a game mode on the tablet", screenModeFont, dimWidth*0.75, dimHeight*0.4, dimWidth*0.45, dimHeight*0.07, ofColor(225), ofColor(0), true, 8*gameModeObject.screenAdjustBoth, ofColor(0));
                gameModeObject.gameModeGUI.notButton("Select a game mode on the tablet", screenModeFont, dimWidth*0.25, dimHeight*0.4, dimWidth*0.45, dimHeight*0.07, ofColor(225), ofColor(0), true, 8*gameModeObject.screenAdjustBoth, ofColor(0));

            }
            if(startingGame){
                gameModeObject.reset(currentModeINT, false); // Put in all the info to the game here
                currentGameState = active;
                gameModeObject.activeStartTime = ofGetElapsedTimeMillis(); // Look we keep the timer fresh!
                startingGame = false;
                gameModeObject.showTimer = true;
            }
            // -------------------Title ------------------- //
            ofSetColor(255);
            if(titleImg.isAllocated()){
            titleImg.draw(dimWidth*0.25, dimHeight/6);
            }
        }break;
            
            //========================================================================
                    ////////////////// ACTIVE MODE IS HERE /////////////////////
            //========================================================================
        case active:{

          //  ofSetColor(backgroundCol);
          //  ofSetRectMode(OF_RECTMODE_CENTER);
           // ofDrawRectangle(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // The main space
            
           // ofSetColor(depthCol);
            gameModeObject.activeGame(screenModeFont);
            
            /////// THIS IS END OF GAME YALL!!! ////////
            if(gameModeObject.endofGameYes){
                cout << "GAME OVER!" << endl;
                currentGameState = ruminative;
                // changeColor();
                gameModeObject.endofGameYes = false;
                needToUpdateClient=true;
                msgToSend="GAMEOVER";
               // currBackground = bgImage[3];
            }
            
            if(doWeWantToQuit){
                
                int strokeSize = 8*gameModeObject.screenAdjustBoth;
                ofSetColor(0);
                gameModeObject.gameModeGUI.roundedRect(dimWidth*0.5-dimWidth*0.225-strokeSize*0.5, dimHeight*0.4-dimHeight*0.035-strokeSize*0.5, dimWidth*0.45+strokeSize, dimHeight*0.07+strokeSize, strokeSize);
             //   startButt = screenGameGUI.button("Play!", screenModeFont, dimWidth*0.25, dimHeight*0.85,  dimWidth*0.45, dimHeight*0.07, leftMouseReleased, buttonColorHigh, buttonColorLow, startButt, gameModeObject.tabletInteract);
                gameModeObject.gameModeGUI.notButton("Quit current game?", screenTitleFont, dimWidth*0.5, dimHeight*0.4, dimWidth*0.45, dimHeight*0.07, ofColor(225,0,0), ofColor(0), true, 8*gameModeObject.screenAdjustBoth, ofColor(0));
                
                gameModeObject.gameModeGUI.roundedRect(dimWidth*0.25-dimWidth*0.075-strokeSize*0.5, dimHeight*0.6-dimHeight*0.04-strokeSize*0.5, dimWidth*0.15+strokeSize, dimHeight*0.08+strokeSize, strokeSize);
                timeToQuitYes = gameModeObject.gameModeGUI.button("Yes", screenModeFont, dimWidth*0.25, dimHeight*0.6, dimWidth*0.15, dimHeight*0.08, strokeSize, leftMouseReleased, buttonColorHigh, ofColor(255), timeToQuitYes, gameModeObject.tabletInteract);
              
                gameModeObject.gameModeGUI.roundedRect(dimWidth*0.75-dimWidth*0.075-strokeSize*0.5, dimHeight*0.6-dimHeight*0.04-strokeSize*0.5, dimWidth*0.15+strokeSize, dimHeight*0.08+strokeSize, strokeSize);
                timeToQuitNo = gameModeObject.gameModeGUI.button("No", screenModeFont, dimWidth*0.75, dimHeight*0.6, dimWidth*0.15, dimHeight*0.08, strokeSize, leftMouseReleased, buttonColorHigh, ofColor(255), timeToQuitNo, gameModeObject.tabletInteract);
                
                if(timeToQuitYes){
                    timeToQuitYes  = false;
                    currentGameState = passive;
                    cout << "Passive Timer Reached" << endl;
                    needToUpdateClient=true;
                    msgToSend="BACKTOPASSIVE";
                    currentModeINT = 99;
                    // currBackground = bgImage[0];
                    gameModeObject.reset(99, true);
                    resetPassive();
                    doWeWantToQuit = false;
                    
                    
                    ///// RESET EVERYTHING HERE //////
                }
                if(timeToQuitNo){
                    timeToQuitNo = false;
                    doWeWantToQuit = false;
                }
            }
            
        }break;
        case ruminative:{
            // Background image
            ofSetColor(255, 255, 255);
            if(ruminationBackground.isAllocated()){
            ruminationBackground.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);
            }
            ofSetColor(depthCol);
            ofPushStyle();
            ofSetColor(depthCol);
            gameModeObject.basicKinect(false); // Kinect
            ofPopStyle();
          
            //////////////// BUTTONS ///////////////////
            ofSetColor(0);
            int strokeSize = 8*gameModeObject.screenAdjustBoth;
            gameModeObject.gameModeGUI.roundedRect(dimWidth*0.25-dimWidth*0.15-strokeSize*0.5, dimHeight*0.5-dimHeight*0.05-strokeSize*0.5, dimWidth*0.3+strokeSize, dimHeight*0.10+strokeSize, strokeSize);
            goBack = gameModeObject.gameModeGUI.button("Select a different game mode", screenModeFont, dimWidth*0.25, dimHeight*0.5, dimWidth*0.3, dimHeight*0.10, strokeSize, leftMouseReleased, buttonColorHigh, buttonColorLow, goBack, gameModeObject.tabletInteract);
            
            gameModeObject.gameModeGUI.roundedRect(dimWidth*0.75-dimWidth*0.15-strokeSize*0.5, dimHeight*0.5-dimHeight*0.05-strokeSize*0.5, dimWidth*0.3+strokeSize, dimHeight*0.10+strokeSize, strokeSize);
            playAgainButt = gameModeObject.gameModeGUI.button("Play again?", screenModeFont, dimWidth*0.75, dimHeight*0.5, dimWidth*0.3, dimHeight*0.10, strokeSize, leftMouseReleased, buttonColorHigh, buttonColorLow, playAgainButt, gameModeObject.tabletInteract);
            
            //playAgainButt = screenGameGUI.button(, screenModeFont, dimWidth*0.25, dimHeight*0.5, dimWidth*0.35, dimHeight*0.15, leftMouseReleased, buttonColorHigh, buttonColorLow, playAgainButt, gameModeObject.tabletInteract);
          //  goBack = screenGameGUI.button(, screenModeFont, dimWidth*0.75, dimHeight*0.5, dimWidth*0.35, dimHeight*0.15, leftMouseReleased, buttonColorHigh, buttonColorLow, goBack, gameModeObject.tabletInteract);

            if(playAgainButt){
                playAgainButt = false;
                cout << "Play again yes" << endl;
                gameModeObject.reset(currentModeINT, false);
                currentGameState = active;
                gameModeObject.activeStartTime = ofGetElapsedTimeMillis(); // Look we keep the timer fresh!
                // Put in all the info to the game here
            }
            else if(goBack){
                goBack = false;
                 cout << "Go back thanks" << endl;
                currentGameState = selective;
                gameModeObject.activeStartTime = ofGetElapsedTimeMillis(); // Look we keep the timer fresh!
                currentModeINT = 99;
                gameModeObject.reset(99, true);
             //   currBackground = bgImage[1];
                //// Remember to add in a reset of values if we end up going back
            }
            //////////////// END BUTTONS ////////////////
            // -------------------Title ------------------- //
            ofSetColor(255);
            if(titleImg.isAllocated()){
            titleImg.draw(dimWidth*0.25, dimHeight/6);
            }
        }break;
        default:{
            cout<<"Error, where are we in the draw cases?\n";
        }break;
    }
    
    leftMouseReleased = false; // This is to stop the mouse from staying true.
   
    //screenGameGUI.drawMessageGUI("This is the Screen", screenModeFont, dimWidth/2, dimHeight/1.05, ofColor(155,155,155), dimWidth, dimHeight);
        if (debugMode) {
        gameModeObject.drawDebug();
    
    ofPushStyle();
    ofSetColor(0);
    ofDrawRectangle(0, dimHeight-dimHeight*0.1, dimWidth*0.1, dimHeight*0.02);
    ofSetColor(255);
    ofDrawBitmapString(ofGetFrameRate(),10,dimHeight-dimHeight*0.1);
    ofDrawRectangle(dimWidth/2, dimHeight*0.33333, dimWidth, 8);
    ofDrawRectangle(dimWidth/2, dimHeight*0.66666, dimWidth, 8);
    ofDrawRectangle(dimWidth/2, dimHeight/2, 8, dimHeight);
    ofPopStyle();
        }
}




void ofApp::mouseReleased(int x, int y, int button){
    
    if(button == 0){
        // cout << "MouseReleased" << endl;
        leftMouseReleased = true;
        timerMouseReleased = true;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'd') {
        debugMode = !debugMode;
        if(debugMode){
            ofShowCursor();
        }
        else {
           // ofHideCursor();
        }
    }
    if (key == 'q' && currentGameState == active) {
        doWeWantToQuit = !doWeWantToQuit;
    }
    if(key == ' ' && currentGameState == passive){
        resetPassive();
    }
}
