//
//  gameMode.cpp
//  SSFoxtonV0_1
//
//  Created by Marty on 18/09/17.
//
//

#include "gameMode.hpp"

void gameMode::reset(int modeSelected, bool preset){
    yesWeFinallyHaveAModeVariable = modeSelected;
    if(preset){
        // ------------------- Big Game variables ------------------- //
        endofGameYes = false;
        thereAreTwoTeams = false;
        kinect.weHaveFilledIt = false;
        compareTheRound = false;
        areWeDoingANightMode = false;
        kinect.filledInShapePercentage = 0;
        // totCount = perCount = 0;
        kinect.gamePercentTrigger = 9999; // I put this in to prevent accidental triggers
        kinect.subCountAdder = 1; // For Survival
        kinect.postPixelCheckImage.setColor(ofColor(0,0,0,0)); // We clear the whole image before adding in the pixels
        
        previousSilhoetteAdjust = silhoetteAdjust;
        movingShadow = false; // Allows for the image to be different size and position
        beatenHighscore = false; // put here just in case
        
        
        // ------------------- Flashy cool visuals ------------------- //
        nightMode[0] = false;
        nightMode[1] = false;
        nightModeAmount = 0;
        difficultyOpacity = 0;
        
        // ------------------- Competitive settings ------------------- //
        firstToWhatNum = 2;
        teamTwoIsTrue = false;
        teamTwoHasWon = false;
        itsADraw = false;
        for(int i = 0; i < amountOfPictureChoice; i ++){
            secondRandImgNumGenerator[i] = randImgNumGenerator[i] = false;
        }
        
        // ------------------- Survival setting ------------------- //
        rapidFire = false;
        showRoundSettings = false;
        weirdAlphaVariable = 0;
        randomDifficultyMaker = 100;
        
        // ------------------- Simon Says setting ------------------- //
        takingPicture = false;
        fillInPicture = false;
        
        // ------------------- Quickplay settings ------------------- //
        currentImageNumber = 0;
        
        // ------------------- Game round settings ------------------- //
        gameTimeHasFinished = false;
        gameEndTime = 10000;
        roundEndTime = 5000;
        roundShowShapeTime = 8000; // The time to show how well a person did
        activeStartTime = ofGetElapsedTimeMillis();
        showTimer = true;
        showScore = false; // CHANGE
        showTeams = false;
        teamTotalScore[0] = 0; // This is the teams score
        teamTotalScore[1] = 0; // This is the teams score
        teamCurrentRound[0] = 0;
        teamCurrentRound[1] = 0;
        currentRound = 0;
        teamRealName[0] = "Team";
        teamRealName[1] = "Team";
        teamColourBackground[0] = ofColor(37, 72, 183);
        teamColourShadow[0] = ofColor(20);
        teamColourDepth[0] = ofColor(255,0, 0);
        teamColourCombined[0] = ofColor(255,215, 0);
        teamColourBackground[1] = ofColor(29, 178, 29);
        teamColourShadow[1] = ofColor(20);
        teamColourDepth[1] = ofColor(255,0, 0);
        teamColourCombined[1] = ofColor(255,215, 0);
        
        // SETUP the DESCRIPTIONS OF THE MODES:
        gameModeGUI.setupDescriptionText(modeSelected);
        
        if(modeSelected == 0){ // Competitive
            firstToWhatNum = 2;
            roundEndTime = 5000;
            roundShowShapeTime = 6000;
            thereAreTwoTeams = true;
            teamRealName[0] = teamNames[int(ofRandom(0, 21))];
            teamRealName[1] = teamNames[int(ofRandom(0, 21))];
            while(teamRealName[0] == teamRealName[1]){
                teamRealName[1] = teamNames[int(ofRandom(0, 21))];
            }
            teamColourInt[0] = int(ofRandom(0, 3));
            teamColourInt[1] = int(ofRandom(0, 3));
            while(teamColourInt[0] == teamColourInt[1]){
                teamColourInt[1] = int(ofRandom(0, 3));
            }
            loadTeamColour(teamColourInt[0], 0);
            loadTeamColour(teamColourInt[1], 1);
            currentScoreboardImage[0] = gameModeGUI.scoreboardCompetitive[teamColourInt[0]];
            currentScoreboardImage[1] = gameModeGUI.scoreboardCompetitive[teamColourInt[1]];
            //// Load first image here
            //currentImageNumber = ofRandom(0, amountOfPictureChoice);
            for(int i = 0; i < amountOfPictureChoice; i ++){ //Reset
                randImgNumGenerator[i] = false;
            }
            int totalFour = 0;
            while(totalFour<3){
                currentImageNumber = ofRandom(0, amountOfPictureChoice);
                if(randImgNumGenerator[currentImageNumber] == false){
                    secondRandImgNumGenerator[currentImageNumber] = randImgNumGenerator[currentImageNumber] = true;
                    totalFour+=1;
                }
            }
            
            areWeDoingANightMode = true;
            showTeams = true;
            showScore = true;
            movingShadow = true;
        }
        else if(modeSelected == 1){ // Survival
            roundShowShapeTime = 6000;
            roundEndTime = 30000;
            kinect.gamePercentTrigger = 50; // Start small
            thereAreTwoTeams = false;
            teamRealName[0] = teamNames[int(ofRandom(0, 21))];
            teamColourInt[0] = int(ofRandom(0, 3));
            loadTeamColour(teamColourInt[0], 0);
            currentScoreboardImage[0] = gameModeGUI.scoreboardSurvival[teamColourInt[0]];
            showTeams = false;
            rapidFire = true;
            movingShadow = true;
            
        }
        else if(modeSelected == 2){ // Simon Says
            firstToWhatNum = 2;
            roundEndTime = 6000;
            roundShowShapeTime = 6000;
            thereAreTwoTeams = true;
            teamRealName[0] = teamNames[int(ofRandom(0, 21))];
            teamRealName[1] = teamNames[int(ofRandom(0, 21))];
            while(teamRealName[0] == teamRealName[1]){
                teamRealName[1] = teamNames[int(ofRandom(0, 21))];
            }
            teamColourInt[0] = int(ofRandom(0, 3));
            teamColourInt[1] = int(ofRandom(0, 3));
            while(teamColourInt[0] == teamColourInt[1]){
                teamColourInt[1] = int(ofRandom(0, 3));
            }
            loadTeamColour(teamColourInt[0], 0);
            loadTeamColour(teamColourInt[1], 1);
            currentScoreboardImage[0] = gameModeGUI.scoreboardCompetitive[teamColourInt[0]];
            currentScoreboardImage[1] = gameModeGUI.scoreboardCompetitive[teamColourInt[1]];
            showTeams = true;
            showScore = true; // Change
        }
        else if(modeSelected == 3){ // Quick Play
            currentImageNumber = 0;
            compareTheRound = true;
            roundEndTime = 7000; // CHANGE
            firstToWhatNum = 0;
            showScore = true;
            movingShadow = false;
            teamColourInt[0] = int(ofRandom(0, 3));
            loadTeamColour(teamColourInt[0], 0);
            currentScoreboardImage[0] = gameModeGUI.scoreboardQuickplay[teamColourInt[0]];
        }
        else if(modeSelected == 99){ // standard reset
            
        }
    }
    else{
        if(modeSelected == 0){ // Competitive
            kinect.subCountAdder = 0.1;
            currentRound = 1;
            teamTotalScore[0] = 0; // This is the teams score
            teamTotalScore[1] = 0; // This is the teams score
            gameTimeReset(3000, onYourMarks, false); // Resets our timer things and goes to next state
            
            for(int i = 0; i < amountOfPictureChoice; i ++){
                randImgNumGenerator[i] = secondRandImgNumGenerator[i];
            }
            
            /// Selects a correct image
            currentImageNumber = ofRandom(0, amountOfPictureChoice);
            while(randImgNumGenerator[currentImageNumber] == false){
                currentImageNumber = ofRandom(0, amountOfPictureChoice);
            }
            randImgNumGenerator[currentImageNumber] = false;
            
            updateContentImage(currentImageNumber); // This gets the new image
            currentCommandSound = soundCommandsArray[1]; // Team 1 get ready
            currentCommandSound.play();
        }
        
        else if(modeSelected == 1){ // Survival
            roundEndTime = 30000;
            kinect.subCountAdder = 0.05;
            teamTotalScore[0] = 0; // This is the teams score
            gameTimeReset(3000, onYourMarks, false); // Resets our timer things and goes to next state
            kinect.gamePercentTrigger = 40; // Start small
            randomDifficultyMaker = 100;
            showRoundSettings = true;
            showScore = true;
            currentRound = 0;
            weirdAlphaVariable = 0;
            gameStaller = ofGetElapsedTimeMillis();
            // ------------------- RANDOM IMAGES FOR SURVIVAL MODE ------------------- //
            int getTotalList = 0;
            currentImageNumber = ofRandom(0, amountOfPictureChoice);
            for(int i = 0; i < amountOfPictureChoice; i ++){
                randImgNumGenerator[i] = false;
            }
            randImgNumGenerator[currentImageNumber] = true; // First one in the pack
            updateContentImage(currentImageNumber); // This gets the new image
            currentCommandSound = soundCommandsArray[0]; // get ready
            currentCommandSound.play();
        }
        else if(modeSelected == 2){ // Simon Says
            currentRound = 1;
            kinect.subCountAdder = 0.1;
            showTeams = true;
            takingPicture = true;
            teamTotalScore[0] = 0; // This is the teams score
            teamTotalScore[1] = 0; // This is the teams score
            gameTimeReset(4000, onYourMarks, false); // Resets our timer things and goes to next state
            //////// NO IMAGES TO LOAD /////////
            kinect.contentImage.clear();
            currentCommandSound = soundCommandsArray[2]; // team 2 get ready
            currentCommandSound.play();
        }
        else if(modeSelected == 3){ // Quick Play
            gameTimeReset(3000, onYourMarks, false); // Resets our timer things and goes to next state
            updateContentImage(currentImageNumber); // This gets the new image
            kinect.subCountAdder = 0.1;
            currentCommandSound = soundCommandsArray[0];
            currentCommandSound.play();
        }
    }
}

void gameMode::activeGame(ofTrueTypeFont modeFont){
    ofEnableAlphaBlending();
    
    if(currentActiveStatesINT != thatIsTime){ // This is the background team colour
        if(!takingPicture){
            ofSetColor(teamColourBackground[teamTwoIsTrue?1:0]);
        }
        else{
            ofSetColor(teamColourBackground[teamTwoIsTrue?0:1]);
        }
        ofDrawRectangle(dimWidth/2, dimHeight/2, dimWidth, dimHeight);
    }
    else{ // The mountain background in between games when you see the scores
        ofSetColor(255);
        if(endOfRoundBackground.isAllocated()){
        endOfRoundBackground.draw(dimWidth/2, dimHeight/2, dimWidth, dimHeight);
        }
    }
    
    switch ( currentActiveStatesINT ) {
            // ------------------- The pre game to get players ready ------------------- //
        case onYourMarks:{
            ofSetRectMode(OF_RECTMODE_CENTER);
            int time = (gameEndTime - (ofGetElapsedTimeMillis()-activeStartTime));
            int colorOpacity = ofMap(time, gameEndTime, 0, 255, 0); // Simon says colour adjust
            if(colorOpacity<0){colorOpacity = 0;}
            
            if(fillInPicture){ // Simon says transition
                ofSetColor(teamColourBackground[teamTwoIsTrue?0:1],colorOpacity);
                ofDrawRectangle(dimWidth/2, dimHeight/2, dimWidth, dimHeight);
            }
            
            ofPushStyle();
            ofSetColor(teamColourDepth[teamTwoIsTrue?1:0]);
            basicKinect(!takingPicture?true:false); // The kinect depth is here
            ofPopStyle();
            
            if(!takingPicture){
                //ofSetColor(teamColourCombined[teamTwoIsTrue?1:0], 255-colorOpacity); // Show the pixel comparison
                if(fillInPicture){
                    ofSetColor(teamColourBackground[teamTwoIsTrue?1:0], colorOpacity);
                    ofFill();
                    ofDrawRectangle(dimWidth/2,dimHeight-(dimHeight/6*2), dimWidth/3*2, dimHeight/3*2); // The capture zone
                }
                ofSetColor(colorOpacity,255); // Show the pixel comparison
                 if(kinect.shadowImageForStart.isAllocated()){
                kinect.shadowImageForStart.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // Silhouette
                 }
                depthMask(colorOpacity);
            }
            drawGameGUI(modeFont, colorOpacity); // NEVER FORGET THERE ARE SHADOWS IN HERE!!!
            
            if(gameTimeHasFinished){
                cout << "On Your Marks Check- Team 1 : " << teamRealName[0] << " Team 2 : " << teamRealName[1] << endl;
                showRoundSettings = false;
                if(takingPicture){ // Simon says for image pic take
                    gameTimeReset(3500, playThatGame, false);
                }
                else{
                    gameTimeReset(roundEndTime, playThatGame, true); // Resets our timer things and goes to next state
                }
                if(areWeDoingANightMode){ // Night Mode yo!
                    int rand = ofRandom(0, 8);
                    (rand == 1?nightMode[teamTwoIsTrue?1:0]=true:nightMode[teamTwoIsTrue?1:0]=false);
                    nightModeAmount=0;
                }
                gameStaller = ofGetElapsedTimeMillis();
                if(!takingPicture){
                    if(fillInPicture){ // Team fill it in!
                        currentCommandSound = soundCommandsArray[teamTwoIsTrue?6:5];
                        currentCommandSound.play();
                    }
                    else{ // GO!
                        currentCommandSound = soundCommandsArray[12];
                        currentCommandSound.play();
                    }
                }
                else{ // Opposing team makes a shadow
                    currentCommandSound = soundCommandsArray[teamTwoIsTrue?3:4];
                    currentCommandSound.play();
                }
            }
            
            
        }break;
            
        case playThatGame:{
            
            if(!takingPicture){ // This is for simon says first state
                // ------------------- DOING THE PIXEL CHECK ------------------- //
                // REMINDER THAT THIS RETURNS TRUE EVERYTIME IF IT IS ALREADY TRUE!!
                kinect.fillInPixelsNow = true; // This changes the kinect update function
                // We compare the two images and see if they are the same
                
                
                // ------------------- SHOWING THE SILHOUETTE ------------------- //
                ofSetRectMode(OF_RECTMODE_CENTER);
                (kinect.weHaveFilledIt&&weirdAlphaVariable<=0?ofSetColor(0, 225, 30, 255):ofSetColor(teamColourShadow[teamTwoIsTrue?1:0]));
                if(kinect.contentImage.isAllocated()){
                kinect.contentImage.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // Silhouette
                }
                
                
                // ------------------- TRANSITION ------------------- //
                if(weirdAlphaVariable > 0){ // For that really important totally needed transition
                    kinect.weHaveFilledIt = false;
                    int time = ofGetElapsedTimeMillis()-gameStaller;
                    weirdAlphaVariable= ofMap(time, 0, 1000, 255, 0);
                    if(weirdAlphaVariable<0){weirdAlphaVariable=0;}
                    
                    ofSetColor(0, 0, 0, weirdAlphaVariable); // Show the black silhoette
                    if(kinect.contentImage.isAllocated()){
                    kinect.contentImage.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // Silhouette
                    }
                }

                // ------------------- NIGHT MODE DISPLAY ------------------- //
                if(nightMode[teamTwoIsTrue?1:0]){
                    ofSetColor(0, nightModeAmount); // Show the night
                    ofDrawRectangle(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // The Night space
                    (nightModeAmount<255?nightModeAmount+=8:nightModeAmount=255);
                }
            }
                // ------------------- SIMON SAYS ------------------- //
            else { // Simon Says
                kinect.getPicture = true; // This changes the kinect update function
                int picTimer = ofGetElapsedTimeMillis() - activeStartTime;
                int picAlpha= ofMap(picTimer, 0, gameEndTime, 0, 255);
                ofFill();
                ofSetColor(teamColourBackground[teamTwoIsTrue?1:0], picAlpha);
                ofDrawRectangle(dimWidth/2,dimHeight-(dimHeight/6*2), dimWidth/3*2, dimHeight/3*2); // The capture zone
            }
            if(weirdAlphaVariable == 0){ // Not to be seen during transitions
                // ------------------- KINECT DEPTH SENSOR UPDATE ------------------- //
                ofPushStyle();
                ofSetColor(teamColourDepth[teamTwoIsTrue?1:0]);
                basicKinect(kinect.fillInPixelsNow); // The kinect is here
                ofPopStyle();

                // ------------------- SHOWING THE PIXEL CHECK ------------------- //
                if(!takingPicture && kinect.postPixelCheckImage.isAllocated()){
                    depthMask(0);
                }
                else if(takingPicture){
                    if(kinect.filledInShapePercentage<4){
                        activeStartTime = ofGetElapsedTimeMillis(); // Look we keep the timer fresh!
                        gameTimeHasFinished = false;
                    }
                }
            }
            // ------------------- GAMEPLAY GUI ------------------- //
            if((gameTimeHasFinished && flashModeAmount == 0)|| (kinect.weHaveFilledIt && rapidFire && flashModeAmount ==0)){ // Once timer is done we move in two different directions based on gamemode
                gameStaller = ofGetElapsedTimeMillis()-100;
                flashModeAmount = 1;
                if(gameTimeHasFinished && rapidFire){
                }
                else{
                    currentEffectSound = soundEffectArray[1];
                    currentEffectSound.play();
                }
                if((kinect.weHaveFilledIt && rapidFire && flashModeAmount ==0)){
                currentEffectSound2 = soundEffectArray[3];
                currentEffectSound2.play();
                }
            }
            
            if(flashModeAmount>0){
                int time = ofGetElapsedTimeMillis()-gameStaller; // First num is cancelled out by the wait
                flashModeAmount = ofMap(time, 0, 600, 0, 255);
            }
           
            if(flashModeAmount<255){
                drawGameGUI(modeFont, 0);
            }
            
            // ------------------- TRANSITION ------------------- //
            postRoundSetups(kinect.weHaveFilledIt); // The transition onto the next part
            
            if(weirdAlphaVariable > 0){
                ofSetColor((rapidFire && gameTimeHasFinished?ofColor(204,10,0):255), weirdAlphaVariable); // A flash based on what mode we are in
                ofDrawRectangle(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // The whiteout
            }
            
            
            
        }break;
            
        case thatIsTime:{ // See the image and the score for a few seconds
            
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(255);
            drawScores(modeFont); // Here are the scores for this game, totally changeable components
            
            if(gameTimeHasFinished){
                if(rapidFire){ // Survival
                    // Only one player now...
                    cout << "end of game for survival" << endl;
                    endofGameYes = true; // Lets GTFO outta here!
                    // currentActiveStatesINT = onYourMarks;
                    ////////////// END OF GAME!! //////////////
                    
                }
                else if(thereAreTwoTeams){ // Competitive
                    comparingTwoTeams(); // This function does the team calculations
                }
                else if (teamTotalScore[(teamTwoHasWon?1:0)] >= firstToWhatNum){ // Used for quickplay, probably need to make it a bit nicer, cleaner
                    endofGameYes = true; // Lets GTFO outta here!
                    // currentActiveStatesINT = onYourMarks;
                    /////////////////////////////////
                    // ------------------- END OF GAME!! ------------------- //
                }
            }
        }break;
    }
}

void gameMode::drawDebug(){
    //Display debug stats
    ofPushStyle();
    int posY = 200;
    ofSetColor(255,180);
    ofDrawRectangle(dimWidth/6, posY+ dimHeight/10, dimWidth/3, dimHeight/5);
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("Area filled in and subtract: " + ofToString(kinect.perCount) + " - " + ofToString(kinect.subCount) + " = " + ofToString(kinect.perCount-kinect.subCount), 15, posY+20);
    ofDrawBitmapString("Multiplier amount: " + ofToString(kinect.totalDepthCount) + " and " + ofToString(totalShadowAmount) + " = " + ofToString((float)kinect.totalDepthCount/(float)totalShadowAmount), 15, posY+40);
    ofDrawBitmapString("Counter up: " + ofToString(ofGetElapsedTimeMillis() - activeStartTime), 15, posY+60);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 15, posY+80);
    ofDrawBitmapString("Resolution: Width - " + ofToString(ofGetWidth()) + " Height - " + ofToString(ofGetHeight()), 15, posY+100);
    
    gameModeGUI.kinectVisualParameter.draw();
    gameModeGUI.gameParameters.draw();
    ofPopStyle();
    
}

