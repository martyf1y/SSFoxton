//
//  gameSetups.cpp
//  SSFoxtonV0_2
//
//  Created by Mini on 1/10/17.
//
//

#include "gameMode.hpp"

void gameMode::gameTimeReset(int newGameEndtime, int moveToThisState, bool shouldWeShowTimer){
    gameTimeHasFinished = false;
    gameEndTime = newGameEndtime;
    showTimer = shouldWeShowTimer;
    activeStartTime = ofGetElapsedTimeMillis(); // Look we keep the timer fresh!
    currentActiveStatesINT = moveToThisState;
}

void gameMode::updateContentImage(int LocalCurrentImageNumber) {
    // We reset these values in case the game modes change
    silhoetteAdjust = previousSilhoetteAdjust = 1;
    int randHeight = 0;
    int randWidth = 0;
    if(movingShadow){ // Alters the image to be more interesting
        float randSize = ofRandom(0.4, 0.7);
        silhoetteAdjust = previousSilhoetteAdjust = randSize;
        int randSide = ofRandom(0,2);
        randHeight = ofRandom(0, dimHeight*0.15);
        int widthLimit = dimWidth*0.5 - (dimWidth*randSize)*0.5;
        randWidth = ofRandom(widthLimit*0.4, widthLimit);
        (randSide==1?randWidth*=-1:randWidth);
    }
    else{
        silhoetteAdjust = previousSilhoetteAdjust = 0.9;
        if(LocalCurrentImageNumber == 1 || LocalCurrentImageNumber == 5
           || LocalCurrentImageNumber == 9 || LocalCurrentImageNumber == 11){
            silhoetteAdjust = previousSilhoetteAdjust = 0.8;
        }
    }
    
   // originalContentImageForSizeAdjust = contentImageArray[LocalFindImage];
   //  originalSafeZoneImageForSizeAdjust = noFillImageArray[LocalFindImage]; // Ain't we just so nice to them?
    kinect.safeZonePixelCheckImage.clear();
    // originalSafeZoneImageForSizeAdjust = 1
    kinect.safeZonePixelCheckImage = loadAnImage(1, LocalCurrentImageNumber, randWidth, randHeight, silhoetteAdjust);
    kinect.contentImage.clear(); // Reset image before we cahnge it. MAY CAUSE ISSUES CHECK THIS!!!
    // originalContentImageForSizeAdjust = 0
    kinect.contentImage = loadAnImage(0, LocalCurrentImageNumber, randWidth, randHeight, silhoetteAdjust); // Resizes the image and sets original image
    kinect.shadowImageForStart.clear(); // 
    kinect.shadowImageForStart = loadAnImage(3, LocalCurrentImageNumber, randWidth, randHeight, silhoetteAdjust); // Resizes the image and sets original

    for (int i=0; i<gameModeGUI.numLanguages; i++) {
        gameModeGUI.shapeNameWithLanguages[i] = ofSplitString(gameModeGUI.shapeNamesListed[LocalCurrentImageNumber], ",")[i];
    }

}

// Yes it is weird having this here but it fits a lot better than in main
//--------------------------------------------------------------//
ofImage gameMode::loadAnImage(int changeImageType, int imageArrayNum, int moveX, int moveY, float localResize) {
        // WHERE THE IMAGE GETS BIGGER/SMALLER
    // So we now source the image inside the function as we know the array num and what type of image. Much better this way
    // originalContentImageForSizeAdjust = 0
    // originalSafeZoneImageForSizeAdjust = 1
    // saveGameImage[(teamTwoIsTrue?1:0)] = 2
    // gameModeObject.blacksImageArray[randPassive] = 3
    ofImage contentToBResized;
    // contentToBResized.allocate(dimWidth, dimHeight, )
    if(changeImageType == 0){
        contentToBResized = contentImageArray[imageArrayNum];
    }
    else if(changeImageType == 1){
        contentToBResized = noFillImageArray[imageArrayNum];
    }
    else if(changeImageType == 2){
        contentToBResized = saveGameImage[(teamTwoIsTrue?1:0)];
    }
    else if(changeImageType == 3){
        contentToBResized = blacksImageArray[imageArrayNum];
    }

    if(takingPicture){
        contentToBResized.crop(dimWidth/2 - dimWidth/6*2, dimHeight-(dimHeight/3*2), dimWidth/3*2, dimHeight/3*2);
    }
    else{
        // WHERE THE IMAGE GETS BIGGER/SMALLER
        contentToBResized.resize(dimWidth*localResize, dimHeight*localResize);
    }

    ofImage resizedLoadImage;
    resizedLoadImage.allocate(dimWidth, dimHeight, OF_IMAGE_COLOR_ALPHA);

    resizedLoadImage.setColor(ofColor(0,0,0,0));

    //--------------------------------------------------------------//
    // The values telling us the beginning points
    int beginHereX = resizedLoadImage.getWidth()*0.5 - contentToBResized.getWidth()*0.5;
   // int endHereX = resizedLoadImage.getWidth()/2 + contentToBResized.getWidth()/2;
    int beginhereY = resizedLoadImage.getHeight() - contentToBResized.getHeight();
    totalShadowAmount = 0;
    for(int x = 0; x < contentToBResized.getWidth(); x++){
        for(int y = 0; y < contentToBResized.getHeight(); y++){
           
            ofColor conCol = contentToBResized.getColor(x, y);// This is sourcing the image colour at a certain location
            if(conCol.a != 0){                
                resizedLoadImage.setColor(x+beginHereX+moveX, y+beginhereY - moveY, conCol);
                totalShadowAmount++; // Add up each pixel
            }
        }
    }
    resizedLoadImage.update();
    return resizedLoadImage;
}


void gameMode::postRoundSetups(bool localWeHaveFilledIt){
    // ------------------- TRANSITION TO FADEOUT (HAPPENS FIRST)------------------- //
    if(gameTimeHasFinished || (localWeHaveFilledIt && rapidFire)){ // Once timer is done we move in two different directions based on gamemode
        
        // ------------------- SAVE IMAGE ------------------- //
        if(flashModeAmount>=255 || takingPicture){
            if(takingPicture){ // Simon Says initial picture, goes to team who is
                flashModeAmount=255; // Kinda gross, we use this to transition straight into the next part
                ofImage simonDepth = imgDepth;
                simonDepth.resize(dimWidth, dimHeight);
                saveGameImage[(teamTwoIsTrue?1:0)] = simonDepth;} // Simon says
            else if(!rapidFire){ // Competitive and Simon Says final compare capture image
                saveGameImage[(teamTwoIsTrue?1:0)].grabScreen(0, 0, dimWidth, dimHeight);
                currentEffectSound = soundEffectArray[4]; // Polaroid
                currentEffectSound.play();
            }
           // saveGameImage[(teamTwoIsTrue?1:0)].save(ofGetTimestampString() + ".png");
        }
        else if(!takingPicture){ // Simon Says
        // ------------------- MAKE A FLASH!! ------------------- //
        ofSetColor((rapidFire && !localWeHaveFilledIt?ofColor(204,10,0):255), flashModeAmount); // A flash based on what mode we are in
        ofDrawRectangle(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // The whiteout
        }
       
    }
    else if(!localWeHaveFilledIt && weirdAlphaVariable == 0){ // Bit gross that I am using this, maybe come back to bite
     //   gameStaller = ofGetElapsedTimeMillis(); // Helps with the transition time
    }
    
    // ------------------- Post FADEOUT! (HAPPENS SECOND) ------------------- //
    if(flashModeAmount>=255){ // End of transitions, now see what is happening next
        if(currentCommandSound.isPlaying()) {
            currentCommandSound.stop();
        }
        if(rapidFire){
            if(gameTimeHasFinished){ // No more levels, only losers here
                // The team have finished the level, may have lost, we will find out...
                currentCommandSound = soundCommandsArray[11]; // GAME OVER
                currentCommandSound.play();
                showRoundSettings = true;
                showScore = false;
                compareTheRound = true; // Single player survival

                // Hopefully the right place to check for survival highscore...
                checkForHighscoreSurvival(currentRound);
                kinect.postPixelCheckImage.setColor(ofColor(0,0,0,0)); // We clear the whole image before adding in the pixels
                gameTimeReset(roundShowShapeTime, thatIsTime, false); // Resets our timer things
            }
            else{ // We are still going bro! Bring another shadow!
                cout << "Another Round " << endl;
                currentRound ++; // This is the score of pictures they have.
                gameStaller = ofGetElapsedTimeMillis();
                gameEndTime += 3500; // Add in time for transition overlap
                difficultyOpacity = 0;
                int gameChanger = ofRandom(0, randomDifficultyMaker);
                if(gameChanger <= 3){
                    randomDifficultyMaker = 150 + currentRound * 4;
                    kinect.subCountAdder +=0.2; // Increase subtraction but only every so often
                    difficultyOpacity = 255;
                    kinect.gamePercentTrigger += 4 - (4 - 30/currentRound);
                }
                else{
                    if(currentRound < 30){
                    randomDifficultyMaker *=(0.5+currentRound*0.01);// Increases chance of getting hit
                    }
                    else{
                        randomDifficultyMaker *=0.85;
                    }
                }
                if(currentRound%12==0){
                    for(int i = 0; i < amountOfPictureChoice; i ++){
                        randImgNumGenerator[i] = false;
                    }
                }
                currentImageNumber = ofRandom(0, amountOfPictureChoice);
                while(randImgNumGenerator[currentImageNumber] == true){
                    currentImageNumber = ofRandom(0, amountOfPictureChoice);
                }
                randImgNumGenerator[currentImageNumber] = true;
                updateContentImage(currentImageNumber); // Get that new picture!
                weirdAlphaVariable = 255; // HEY! this is how we do a slow transition into the next round. NOT GOOD CODE
            }
        }
        else if(takingPicture){  ///// SIMON SAYS, REMEMBER THIS
            // saveGameImage[(teamTwoIsTrue?1:0)] = 2
            kinect.shadowImageForStart = kinect.safeZonePixelCheckImage = kinect.contentImage = loadAnImage(2, NULL, 0, 0, 1); // Resizes the image and sets original image
            takingPicture = false;
            fillInPicture = true;
            currentCommandSound = soundCommandsArray[teamTwoIsTrue?2:1]; // Fill in team get ready!
            currentCommandSound.play();
            gameTimeReset(4000, onYourMarks, false); // Resets our timer things
        }
        else{ ////// Mostly for competitive mode here. Maybe change it to a function
            ///// USED FOR SIMON SAYS TOO POST IMAGE TRY
            // ------------------- MOVE ONTO QUICK RECAP ------------------- //
            
             // Competitive and simon says
                teamCurrentRound[(teamTwoIsTrue?1:0)] = kinect.filledInShapePercentage; // This is the score that team got, we use it for many things not just teams.
            
                // Hey check if the Quickplay score has beaten the highscore.. (and dont check competitive)
                if (!thereAreTwoTeams) { // not ideally a secure check...
                    checkForHighscoreImages(kinect.filledInShapePercentage);
                }
            if(fillInPicture){ // Simon says, we just swap who got what
                takingPicture = true;
                fillInPicture = false;
            }
            kinect.postPixelCheckImage.setColor(ofColor(0,0,0,0)); // We clear the whole image to stop weird things happening
            gameTimeReset(roundShowShapeTime, thatIsTime, false); // Resets our timer things
        }
        flashModeAmount = 0;
        kinect.weHaveFilledIt = false;
        kinect.filledInShapePercentage = 0;
    }

}

void gameMode::comparingTwoTeams(){
    
    if(compareTheRound){ // Here we are to see what has happened in the round
        compareTheRound = false;
        currentRound ++; // Matters for competitive
        nightMode[0]= false;
        nightMode[1]= false;
        
        if(teamTotalScore[(teamTwoHasWon?1:0)] >= firstToWhatNum){
            endofGameYes = true; // Lets GTFO outta here!
            currentActiveStatesINT = onYourMarks;
            /////////////////////////////////
            // ------------------- END OF GAME!! ------------------- //
        }
        else{
            //We do another round!
            
            if(itsADraw){
                currentRound --;
            }
            else if(yesWeFinallyHaveAModeVariable == 0){ // Competitive this 'else' is to prevent an image from becoming true when we are down a round
                currentImageNumber = ofRandom(0, amountOfPictureChoice);
                while(randImgNumGenerator[currentImageNumber] == false){
                    currentImageNumber = ofRandom(0, amountOfPictureChoice);
                }
                randImgNumGenerator[currentImageNumber] = false;
                updateContentImage(currentImageNumber);
                
                
            }
            if(yesWeFinallyHaveAModeVariable == 0){
            currentCommandSound = soundCommandsArray[1]; // next team get ready
            currentCommandSound.play();
            }
            else{
                currentCommandSound = soundCommandsArray[2]; // next team get ready
                currentCommandSound.play();
            }
            gameTimeReset(3000, onYourMarks, false); // Resets our timer things
            itsADraw = false;
            
        }
    }
    
    else if(teamTwoIsTrue){
        kinect.postPixelCheckImage.setColor(ofColor(0,0,0,0)); // We clear the whole image before adding in the pixels
        gameTimeReset(8000, thatIsTime, false); // Resets our timer things
        compareTheRound = true;
        teamTwoIsTrue = false;
        // If it is the end of the round find out who won
        if(teamCurrentRound[0] > teamCurrentRound[1]){
            teamTwoHasWon = false;
            teamTotalScore[0] +=1;
            if(teamTotalScore[0] >= firstToWhatNum){
            currentCommandSound = soundCommandsArray[9]; // team one wins the game
            currentCommandSound.play();
            }
            else{
                currentCommandSound = soundCommandsArray[7]; // team one wins the round
                currentCommandSound.play();
            }
        }
        else if(teamCurrentRound[0] < teamCurrentRound[1]){
            teamTwoHasWon = true;
            teamTotalScore[1] +=1;
            if(teamTotalScore[1] >= firstToWhatNum){
                currentCommandSound = soundCommandsArray[10]; // team two wins the game
                currentCommandSound.play();
            }
            else{
                currentCommandSound = soundCommandsArray[8]; // team two wins the round
                currentCommandSound.play();
            }
        }
        else if(teamCurrentRound[0] == teamCurrentRound[1]){
            itsADraw = true;
            currentCommandSound = soundCommandsArray[13]; // team one wins the game
            currentCommandSound.play();
        }
    }
    else{ // We gotta do another round for team two
        cout << "TEAM 1 Done, Now team two" << endl;
        
        gameTimeReset(3000, onYourMarks, true); // Resets our timer things
        teamTwoIsTrue = true;
        if(takingPicture){ // SImon says
            kinect.contentImage.clear(); // We clear the images so they dont pop up
            kinect.safeZonePixelCheckImage.clear();
            currentCommandSound = soundCommandsArray[1]; // next team get ready
            currentCommandSound.play();
        }
        else{
            currentCommandSound = soundCommandsArray[2]; // next team get ready
            currentCommandSound.play();
        }
    }
    
}


void gameMode::checkForHighscoreImages(int incomingScore) {
    
    // CHECK
    beatenHighscore = false;
    
    if (incomingScore > highscoreCurrentImage[currentImageNumber]) {
        beatenHighscore=true;
        highscoreCurrentImage[currentImageNumber] = incomingScore;
    }
    
    // SAVE
    if (beatenHighscore) {
        
        // turn highscore array into string
        string highscoreText;
        for (int i=0; i<amountOfPictureChoice; i++) {
            highscoreText.append(ofToString(highscoreCurrentImage[i]));
            if (i < amountOfPictureChoice-1) {
                highscoreText.append("\n"); // push onto new line until the last line
            }
        }
        
        try {   // Open file
            ofFile file;
            file.open("7.data/highscoresImages.txt",ofFile::WriteOnly);
 
            // Overwrite all data within file: (with new lines)
            file << highscoreText;
        } catch(exception& e){
            cout << "ERROR: couldn't save highscores" << endl;
        }
    }
}

void gameMode::checkForHighscoreSurvival(int incomingScore) {
   
    // CHECK
    beatenHighscore = false;
    
    if (incomingScore > highscoreCurrentSurvival) {
        beatenHighscore=true;
        highscoreCurrentSurvival = incomingScore;
        string year = ofToString(ofGetYear());
        highscoreCurrentDate = ofToString(ofGetDay()) + "." +  ofToString(ofGetMonth()) + "." + year.substr(2,2);
    }
    
    // SAVE
    if (beatenHighscore) {
        try {   // Open file
            ofFile file;
            file.open("7.data/highscoreSurvival.txt",ofFile::WriteOnly);
            
            // Overwrite all data within file: (one line)
            file << ofToString(incomingScore);
            
        } catch(exception& e){
            cout << "ERROR: couldn't save survival highscore" << endl;
        }
        try {   // Open file
            ofFile file;
            file.open("7.data/highscoreSurvivalDate.txt",ofFile::WriteOnly);
            
            // Overwrite all data within file: (one line)
            file << highscoreCurrentDate;
            
        } catch(exception& e){
            cout << "ERROR: couldn't save survival highscore" << endl;
        }
 
    }
    
}

//========================================================================
//========================================================================
//========================================================================
// ------------------- GUI GAME MODE UPDATES ------------------- //


void gameMode::drawGameGUI(ofTrueTypeFont modeFont, int localColorOpacity){
    ofPushStyle();
    if(currentActiveStatesINT == onYourMarks){
        
        
        if(!takingPicture){ // Not simon says
            
          // ofSetColor(fillInPicture?0:localColorOpacity, 255);
           // kinect.shadowImageForStart.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // Silhouette

            if(!fillInPicture){
            gameModeGUI.drawNameOfShape(ofColor(0));
            }
           
        }
        else{ //  simon says
            ofSetColor(1, 20, 100, 255);
            ofPushStyle();
            // ofDrawRectangle(dimWidth/2,dimHeight-(dimHeight/6*2), dimWidth/3*2, dimHeight/3*2); // The capture zone
            ofSetColor(teamColourBackground[teamTwoIsTrue?1:0]);
            int stroke = 10*adjustSizeX;
            ofDrawRectangle(dimWidth*0.5, dimHeight-(dimHeight/3*2), dimWidth/3*2+stroke, stroke);
            ofDrawRectangle(dimWidth*0.5-dimWidth/3, dimHeight-(dimHeight/6*2), stroke, dimHeight/3*2);
            ofDrawRectangle(dimWidth*0.5+dimWidth/3, dimHeight-(dimHeight/6*2), stroke, dimHeight/3*2);
            ofPopStyle();
            
        }
        if(thereAreTwoTeams){
            if(!takingPicture && !fillInPicture){
            gameModeGUI.notButton("Team " + (teamRealName[teamTwoIsTrue?1:0]), modeFont, dimWidth*0.25, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(225,localColorOpacity), ofColor(0,localColorOpacity), true, 8*screenAdjustBoth, ofColor(0,localColorOpacity));
                  gameModeGUI.notButton("Get ready!", modeFont, dimWidth*0.75, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(225,localColorOpacity), ofColor(0,localColorOpacity), true, 8*screenAdjustBoth, ofColor(0,localColorOpacity));
               }
            else if(takingPicture){
                gameModeGUI.notButton("Team " + (teamRealName[teamTwoIsTrue?0:1]), modeFont, dimWidth*0.25, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(225), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                gameModeGUI.notButton("Prepare your shadow!", modeFont, dimWidth*0.75, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(225), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
            }
            else if(fillInPicture){
                gameModeGUI.notButton("Team " + (teamRealName[teamTwoIsTrue?1:0]), modeFont, dimWidth*0.25, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(teamColourBackground[teamTwoIsTrue?1:0],localColorOpacity), ofColor(0,localColorOpacity), true, 8*screenAdjustBoth, ofColor(0,localColorOpacity));
                gameModeGUI.notButton("Get into place!", modeFont, dimWidth*0.75, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(teamColourBackground[teamTwoIsTrue?1:0],localColorOpacity), ofColor(0,localColorOpacity), true, 8*screenAdjustBoth, ofColor(0,localColorOpacity));
            }
            
        }
        else{
            gameModeGUI.notButton("GET READY!", modeFont, dimWidth*0.25, dimHeight*0.5, dimWidth*0.45, dimHeight*0.07, ofColor(225,localColorOpacity), ofColor(0,localColorOpacity), true, 8*screenAdjustBoth, ofColor(0,localColorOpacity));
            gameModeGUI.notButton("GET READY", modeFont, dimWidth*0.75, dimHeight*0.5, dimWidth*0.45, dimHeight*0.07, ofColor(225,localColorOpacity), ofColor(0,localColorOpacity), true, 8*screenAdjustBoth, ofColor(0,localColorOpacity));
        }
    }
    
    
    if(rapidFire){ // Turn this into a difficulty changed thing
       // gameModeGUI.drawMessageGUI("Completion percent required: " + ofToString(kinect.gamePercentTrigger), modeFont, dimWidth*0.5, dimHeight*0.9, ofColor(240,198,28));
    }
      string teamTypeValue = (rapidFire?"Score":"Round"); // Swaps between the two types of values, time and score
    if(showScore && !takingPicture){
        ofSetColor(255);
        if(currentScoreboardImage[(teamTwoIsTrue?1:0)].isAllocated()){
        currentScoreboardImage[(teamTwoIsTrue?1:0)].draw(currentScoreboardImage[(teamTwoIsTrue?1:0)].getWidth()*.51, currentScoreboardImage[(teamTwoIsTrue?1:0)].getHeight()*.51, currentScoreboardImage[(teamTwoIsTrue?1:0)].getWidth(), currentScoreboardImage[(teamTwoIsTrue?1:0)].getHeight());
        }
      
        // ------------------- Actual Scores here ------------------- //
        gameModeGUI.drawMessageGUI(ofToString((flashModeAmount<255&&currentActiveStatesINT==playThatGame?kinect.filledInShapePercentage:0)), guiScoreFont, 317*adjustSizeX, 172*adjustSizeY, teamColourBackground[teamTwoIsTrue?1:0]);

        gameModeGUI.drawMessageGUI("%", modeFont, 415*adjustSizeX, 110*adjustSizeY, teamColourBackground[teamTwoIsTrue?1:0]);
        gameModeGUI.drawMessageGUI("Time", scoreNameFonts,  90*adjustSizeX, 158*adjustSizeY, teamColourBackground[teamTwoIsTrue?1:0]);
        gameModeGUI.drawMessageGUI("Percent", scoreNameFonts,  170*adjustSizeX, 61*adjustSizeY, teamColourBackground[teamTwoIsTrue?1:0]);
        if(yesWeFinallyHaveAModeVariable != 3){
            gameModeGUI.drawMessageGUI(ofToString(currentRound), guiRoundFont,  516*adjustSizeX, 232*adjustSizeY, ofColor(0,0,0,255));
        gameModeGUI.drawMessageGUI(teamTypeValue, scoreNameFonts,  523*adjustSizeX, 164*adjustSizeY, teamColourBackground[teamTwoIsTrue?1:0]);
        }
        
        string scaleA;
        if(currentActiveStatesINT == playThatGame){
        int timer =  (gameEndTime - (ofGetElapsedTimeMillis()-activeStartTime))*0.001;
        int moveTimeX = 0;
        ofSetRectMode(OF_RECTMODE_CENTER);
        if(timer < 10){
            moveTimeX = +8;
            if(previousTrigger != int(timer)){
                scaleInt = 1.5;
                if(timer >= 0 && timer < 5 && !gameTimeHasFinished)
                timerSound.play();
            }
            if(scaleInt > 0 && !gameTimeHasFinished){
                scaleInt -= 0.05;
            }
            else{
                scaleInt = 0;
            }
        }
        else{
            scaleInt = 1;
        }
        previousTrigger = timer;
         scaleA = ofToString(previousTrigger+1); // Plus one cause we need to see the start time and not 0
            if(gameTimeHasFinished){
                scaleA = ofToString(0);
            }
        }
        else{
            scaleA = ofToString(roundEndTime*0.001);
        }
        ofSetColor(teamColourBackground[teamTwoIsTrue?1:0]);
        ofPushMatrix();
        ofRectangle bounds = timerFont.getStringBoundingBox(scaleA, 0, 0);
        ofTranslate(112*adjustSizeX, 246*adjustSizeY, 0);
        ofScale(scaleInt, scaleInt, 1.0);
        timerFont.drawString(scaleA, -bounds.width/2, bounds.height/2 );
        ofPopMatrix();
        
       //  cout << " X: " << ofGetMouseX() << endl;
      //  cout << " Y: " << ofGetMouseY() << endl;
        
        // ------------------- DRAW THE NAMES ------------------- //
        if(!fillInPicture){
            gameModeGUI.drawNameOfShape(ofColor(0));
        }
    }
    
    if(showTeams && !takingPicture){
        gameModeGUI.drawMessageGUI(teamRealName[(teamTwoIsTrue?1:0)] , modeFont, 361*adjustSizeX, 318*adjustSizeY, ofColor(0,0,0));
    }
    if(nightMode[teamTwoIsTrue?1:0]){
        gameModeGUI.notButton("NIGHT MODE!", modeFont, dimWidth*0.25, dimHeight*0.28, dimWidth*0.2, dimHeight*0.07, ofColor(200,25,30), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
        gameModeGUI.notButton("NIGHT MODE!", modeFont, dimWidth*0.75, dimHeight*0.28, dimWidth*0.2, dimHeight*0.07, ofColor(200,25,30), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

       // gameModeGUI.drawMessageGUI("NIGHT MODE!", modeFont, dimWidth*0.5, dimHeight*0.2, ofColor(200,25,30));
    }
    
    if(difficultyOpacity >0){
        int time = ofGetElapsedTimeMillis()-gameStaller;
        difficultyOpacity = ofMap(time, 500, 3000, 255, 0);
        if(difficultyOpacity<0){difficultyOpacity=0;}
        
        gameModeGUI.notButton("DIFFICULTY INCREASED!", modeFont, dimWidth*0.25, dimHeight*0.4, dimWidth*0.28, dimHeight*0.07, ofColor(200,25,30, difficultyOpacity), ofColor(0,difficultyOpacity), true, 8*screenAdjustBoth, ofColor(0,difficultyOpacity));
        gameModeGUI.notButton("DIFFICULTY INCREASED!", modeFont, dimWidth*0.75, dimHeight*0.4, dimWidth*0.28, dimHeight*0.07, ofColor(200,25,30, difficultyOpacity), ofColor(0,difficultyOpacity), true, 8*screenAdjustBoth, ofColor(0,difficultyOpacity));
    }
    //========================================================================
    // ------------------- SIMON SAYS UPDATES ------------------- //
    if(takingPicture && currentActiveStatesINT == playThatGame){
        
     gameModeGUI.notButton("Team " + (teamRealName[teamTwoIsTrue?0:1]), modeFont, dimWidth*0.25, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(200,25,30), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
        gameModeGUI.notButton("Make a shadow!", modeFont, dimWidth*0.75, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(200,25,30), ofColor(255), true, 8*screenAdjustBoth, ofColor(0));
        
        ofPushStyle();
        ofSetColor(teamColourBackground[teamTwoIsTrue?1:0]);
        int stroke = 10*adjustSizeX;
        ofDrawRectangle(dimWidth*0.5, dimHeight-(dimHeight/3*2), dimWidth/3*2+stroke, stroke);
        ofDrawRectangle(dimWidth*0.5-dimWidth/3, dimHeight-(dimHeight/6*2), stroke, dimHeight/3*2);
        ofDrawRectangle(dimWidth*0.5+dimWidth/3, dimHeight-(dimHeight/6*2), stroke, dimHeight/3*2);
        ofPopStyle();
    }
    else if(fillInPicture && currentActiveStatesINT == playThatGame){
        gameModeGUI.notButton("Team " + (teamRealName[teamTwoIsTrue?1:0]), modeFont, dimWidth*0.25, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(200,25,30), ofColor(255), true, 8*screenAdjustBoth, ofColor(0));
        gameModeGUI.notButton("Fill in the shape!", modeFont, dimWidth*0.75, dimHeight*0.28, dimWidth*0.35, dimHeight*0.07, ofColor(200,25,30), ofColor(255), true, 8*screenAdjustBoth, ofColor(0));

        

    }
    ofPopStyle();
}


// ------------------- POST GAME FUNCTION ------------------- //
// ------------------- POST GAME FUNCTION ------------------- //
// ------------------- POST GAME FUNCTION ------------------- //
// ------------------- POST GAME FUNCTION ------------------- //
void gameMode::drawScores(ofTrueTypeFont modeFont){
    int strokeRoundness = dimHeight/36;
    int strokeSize = dimHeight/27;
    if(!compareTheRound){
        
        ofSetColor(0,0,0);
        gameModeGUI.notButton("Team " + teamRealName[(teamTwoIsTrue?1:0)] , guiRoundFont, dimWidth*(0.25+(teamTwoIsTrue?1:0)*0.5), dimHeight*0.18, dimWidth*0.4, dimHeight*0.05, teamColourBackground[teamTwoIsTrue?1:0], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
       
        if(!rapidFire){
            gameModeGUI.notButton("Round " + ofToString(currentRound), guiRoundFont, dimWidth*(0.25+(teamTwoIsTrue?1:0)*0.5), dimHeight*0.1, dimWidth*0.3, dimHeight*0.05, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
            
            gameModeGUI.imageNotButton(saveGameImage[(teamTwoIsTrue?1:0)], dimWidth*0.5, dimHeight*0.5, dimWidth*0.4, dimHeight*0.4, strokeSize, ofColor(0), (nightMode[teamTwoIsTrue?1:0]?0:teamColourBackground[teamTwoIsTrue?1:0]), ofColor(255));
        }

        
        gameModeGUI.notButton(ofToString(teamCurrentRound[teamTwoIsTrue?1:0]) + (!rapidFire?"%":""), guiScoreFont, dimWidth*(0.25+(teamTwoIsTrue?1:0)*0.5), dimHeight*0.78, dimWidth*0.2, dimHeight*0.10, teamColourBackground[teamTwoIsTrue?1:0], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

   
    }
    
    else { // We draw both team scores right here
        if(thereAreTwoTeams){
            
            if(!itsADraw){
                gameModeGUI.notButton("Round " + ofToString(currentRound), guiRoundFont, dimWidth*0.25, dimHeight*0.1, dimWidth*0.3, dimHeight*0.05, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                gameModeGUI.notButton("Round " + ofToString(currentRound), guiRoundFont, dimWidth*0.75, dimHeight*0.1, dimWidth*0.3, dimHeight*0.05, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

                gameModeGUI.notButton("Team " + teamRealName[(teamTwoHasWon?1:0)], guiRoundFont, dimWidth*(0.25+(teamTwoHasWon?1:0)*0.5), dimHeight*0.88, dimWidth*0.4, dimHeight*0.05, teamColourBackground[teamTwoHasWon?1:0], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                if(teamTotalScore[(teamTwoHasWon?1:0)] >= firstToWhatNum){
                    gameModeGUI.notButton("won the game!", guiRoundFont, dimWidth*(0.25+(teamTwoHasWon?1:0)*0.5), dimHeight*0.95, dimWidth*0.4, dimHeight*0.05, ofColor(255,215,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                }
                else{
                gameModeGUI.notButton("won the round!", guiRoundFont, dimWidth*(0.25+(teamTwoHasWon?1:0)*0.5), dimHeight*0.95, dimWidth*0.4, dimHeight*0.05, ofColor(255,215,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                }
            }
            

            
            // ------------------- Two teams are compared side by side for most game modes ------------------- //
            
            for(int i = 0; i < 2; i++){
                gameModeGUI.notButton("Team " + teamRealName[i] , guiRoundFont, dimWidth*(0.25+i*0.5), dimHeight*0.18, dimWidth*0.4, dimHeight*0.05, teamColourBackground[i], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
               
                (teamTotalScore[i]>=1?ofSetColor(255):ofSetColor(0));
                if(winnerCup.isAllocated()){
                winnerCup.draw(dimWidth*0.45 + (dimWidth*i*0.1), dimHeight*0.45, winnerCup.getWidth()*0.4*adjustSizeX, winnerCup.getHeight()*0.4*adjustSizeY);
                (teamTotalScore[i]>=2?ofSetColor(255):ofSetColor(0));
                winnerCup.draw(dimWidth*0.45 + (dimWidth*i*0.1), dimHeight*0.55, winnerCup.getWidth()*0.4*adjustSizeX, winnerCup.getHeight()*0.4*adjustSizeY);
                }
                
                int buttWidth = dimWidth*0.20;
            //    ofSetColor(255,0,0);
            //    gameModeGUI.roundedRect(dimWidth*(0.25+i*0.5)-buttWidth*0.5-strokeSize*0.5, dimHeight*0.8-(dimHeight*0.05)-strokeSize*0.5, buttWidth+strokeSize, dimHeight*0.10+strokeSize, strokeRoundness);
                ofColor borderColor = ofColor(0);
                if(!itsADraw) {
                    if(teamTwoHasWon && i == 1){
                        borderColor = ofColor(255,215,0);
                    }
                    else if(!teamTwoHasWon && i == 0){
                        borderColor = ofColor(255,215,0);
                    }
                }
                gameModeGUI.imageNotButton(saveGameImage[i], dimWidth*(0.25+i*0.5), dimHeight*0.5, dimWidth*0.25, dimHeight*0.25, strokeSize, (itsADraw?ofColor(255):((teamTwoHasWon&&i==1)||(!teamTwoHasWon&&i==0)?ofColor(255,255,0):ofColor(0))), (nightMode[i]?0:teamColourBackground[i]), ofColor(255));

            
                gameModeGUI.notButton(ofToString(teamCurrentRound[i]) + "%", guiScoreFont, dimWidth*(0.25+i*0.5), dimHeight*0.78, buttWidth, dimHeight*0.10, teamColourBackground[i], borderColor, true, 8*screenAdjustBoth, ofColor(0));

                if(itsADraw){
                    gameModeGUI.notButton("It is a draw!", guiRoundFont, dimWidth*0.25, dimHeight*0.88, dimWidth*0.4, dimHeight*0.05, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                    
                    gameModeGUI.notButton("No points scored.", guiRoundFont, dimWidth*0.75, dimHeight*0.88, dimWidth*0.4, dimHeight*0.05, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                    
                    gameModeGUI.notButton("Repeat the round!", guiRoundFont, dimWidth*(0.25+i*0.5), dimHeight*0.95, dimWidth*0.4, dimHeight*0.05, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

                }
            }
        }
        else{ // Quick play and survival
            if(!rapidFire){ // Quickplay
                gameModeGUI.imageNotButton(saveGameImage[(teamTwoIsTrue?1:0)], dimWidth*0.5, dimHeight*0.5, dimWidth*0.4, dimHeight*0.4, strokeSize, ofColor(0), (nightMode[teamTwoIsTrue?1:0]?0:teamColourBackground[teamTwoIsTrue?1:0]), ofColor(255));

                gameModeGUI.notButton(ofToString(teamCurrentRound[0]) + "%", guiScoreFont, dimWidth*0.25, dimHeight*0.8, dimWidth*0.2, dimHeight*0.10, teamColourBackground[0], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                gameModeGUI.notButton(ofToString(teamCurrentRound[0]) + "%", guiScoreFont, dimWidth*0.75, dimHeight*0.8, dimWidth*0.2, dimHeight*0.10, teamColourBackground[0], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

                if(beatenHighscore && ofGetSeconds()%2==0){
                    gameModeGUI.notButton("NEW HIGHSCORE!", gameModeGUI.shapeNamesFont, dimWidth*0.25, dimHeight*0.9, dimWidth*0.4, dimHeight*0.07, ofColor(255,215,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                    gameModeGUI.notButton("NEW HIGHSCORE!", gameModeGUI.shapeNamesFont, dimWidth*0.75, dimHeight*0.9, dimWidth*0.4, dimHeight*0.07, ofColor(255,215,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                }
            }
            else{
                gameModeGUI.notButton("End of game!", guiRoundFont, dimWidth*0.25, dimHeight*0.25, dimWidth*0.4, dimHeight*0.07, ofColor(225,0,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                gameModeGUI.notButton("End of game!", guiRoundFont, dimWidth*0.75, dimHeight*0.25, dimWidth*0.4, dimHeight*0.07, ofColor(225,0,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

                gameModeGUI.notButton("You survived against ", gameModeGUI.shapeNamesFont, dimWidth*0.25, dimHeight*0.4, dimWidth*0.4, dimHeight*0.07, ofColor(225), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                gameModeGUI.notButton(ofToString(currentRound) + " shadow" + (currentRound==1?"":"s"), gameModeGUI.shapeNamesFont, dimWidth*0.25, dimHeight*0.5, dimWidth*0.25, dimHeight*0.10, teamColourBackground[0], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                gameModeGUI.notButton("You survived against ", gameModeGUI.shapeNamesFont, dimWidth*0.75, dimHeight*0.4, dimWidth*0.4, dimHeight*0.07, ofColor(225), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                gameModeGUI.notButton(ofToString(currentRound) + " shadow" + (currentRound==1?"":"s"), gameModeGUI.shapeNamesFont, dimWidth*0.75, dimHeight*0.5, dimWidth*0.25, dimHeight*0.10, teamColourBackground[0], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

                if(beatenHighscore && ofGetSeconds()%2==0){
                   // gameModeGUI.notButton("NEW HIGHSCORE!", guiRoundFont, dimWidth*0.5, dimHeight*0.88, dimWidth*0.7, dimHeight*0.05, ofColor(255,215,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                 gameModeGUI.notButton("NEW HIGHSCORE!", gameModeGUI.shapeNamesFont, dimWidth*0.25, dimHeight*0.6, dimWidth*0.4, dimHeight*0.07, ofColor(255,215,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                 gameModeGUI.notButton("NEW HIGHSCORE!", gameModeGUI.shapeNamesFont, dimWidth*0.75, dimHeight*0.6, dimWidth*0.4, dimHeight*0.07, ofColor(255,215,0), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
                }
            }
        }
    }
}
