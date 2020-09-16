//
//  modeSetups.cpp
//  SSFoxtonV0_1
//
//  Created by Mini on 24/09/17.
//
//

#include "gameMode.hpp"


void gameMode::debugSetup(){
    
    gameModeGUI.kinectVisualParameter.setup("OFX CV", "settings.xml", ofGetWidth() - 250, ofGetHeight()/2);
    //  ofxCVPanel.setSize(300, 1000);
    gameModeGUI.kinectVisualParameter.add(shouldWeBlur.set("Use Blur", true));
    gameModeGUI.kinectVisualParameter.add(useGaussian.set("Use Gaussian", true));
    gameModeGUI.kinectVisualParameter.add(radius.set("Radius", 2, 0, 100));
    gameModeGUI.kinectVisualParameter.add(useLinger.set("Linger Shadow", false));
    gameModeGUI.kinectVisualParameter.add(learningTime.set("Learning Time", 0.5, 0, 30));
    gameModeGUI.kinectVisualParameter.add(thresholdValue.set("Threshold Value", 22, 0, 255));
    gameModeGUI.kinectVisualParameter.add(useEdge.set("Edge of Shadow", false));
    gameModeGUI.kinectVisualParameter.add(withDepth.set("With Depth", false));
    gameModeGUI.kinectVisualParameter.add(crazyLand.set("Crazy Land", false));
    gameModeGUI.kinectVisualParameter.add(crazyLandWindow.set("Crazy Window", 15, 1, 30));
    gameModeGUI.kinectVisualParameter.add(crazyLandStrength.set("Crazy Strength", 5, 1, 30));
    
    
    gameModeGUI.gameParameters.setup("Kinect 4 life", "settings.xml", 10, ofGetHeight()/2);
    gameModeGUI.gameParameters.add(dimWidth.set("Screen Width", dimWidth, 300, 1920));
    gameModeGUI.gameParameters.add(dimHeight.set("Screen Height", dimHeight, 300, 1620));
    gameModeGUI.gameParameters.add(gameEndTime.set("Play Time", gameEndTime, 2000, 300000));
    gameModeGUI.gameParameters.add(triggerPerAmount.set("Percentage Silhouette Trigger", triggerPerAmount, 5, 70));
    gameModeGUI.gameParameters.add(silhoetteAdjust.set("Adjust Size", silhoetteAdjust, 0.1, 1));
    gameModeGUI.gameParameters.add(tabletInteract.set("Tablet Interaction", false));
}

void gameMode::loadTeamColour(int teamInt, int whichTeam){ // Choosing the team colours
    
    if(teamInt == 0){ // BLUE
        teamColourBackground[whichTeam] = ofColor(37, 72, 150); // BLUE
        teamColourShadow[whichTeam] = ofColor(0);
        teamColourDepth[whichTeam] = ofColor(255, 0, 255); // PINK
        teamColourCombined[whichTeam] = ofColor(255,255, 255); // WHITE
    }
    else if(teamInt == 1){ // GREEN
        teamColourBackground[whichTeam] = ofColor(29, 146, 29); // GREEN
        teamColourShadow[whichTeam] = ofColor(0);
        teamColourDepth[whichTeam] = ofColor(255, 255, 0); // YELLOW
        teamColourCombined[whichTeam] = ofColor(255,255, 255); // WHITE
    }
    else if(teamInt == 2){ // ORANGE
        teamColourBackground[whichTeam] = ofColor(243, 146, 0); // ORANGE
        teamColourShadow[whichTeam] = ofColor(0);
        teamColourDepth[whichTeam] = ofColor(0,255, 255); // TEAL
        teamColourCombined[whichTeam] = ofColor(255,255, 255); // WHITE
    }
}



int gameMode::modeSelect(ofTrueTypeFont modeFont, bool localLeftMouseReleased, int currentModeSelected){
    // These are all the buttons for selecting what mode we are to go to next, when one is selected we move on
    
    
    int buttonWidth = dimWidth*0.2;
    int buttonXPosition = dimWidth*0.75-dimWidth*0.125;
    int strokeSize = 8*screenAdjustBoth;
    int buttonYPosition = dimHeight/6 -(dimHeight/6)*0.25;
    for(int i = 0; i < amountOfModes; i++){
       
        ofSetColor(0);
        ofSetRectMode(OF_RECTMODE_CENTER);
        gameModeGUI.roundedRect(buttonXPosition-buttonWidth/2-strokeSize*0.5, buttonYPosition-dimHeight*0.035-strokeSize*0.5, buttonWidth+strokeSize, dimHeight*0.07+strokeSize, strokeSize);
        
        modeButt[i] = gameModeGUI.button(modeName[i], modeFont, buttonXPosition, buttonYPosition, buttonWidth, dimHeight*0.07, strokeSize, localLeftMouseReleased, buttonColorHigh, currentModeSelected == i?ofColor(34, 143, 209):buttonColorLow, modeButt[i], tabletInteract);
        if(i == 1){
            buttonYPosition = dimHeight/6 -(dimHeight/6)*0.25;
            buttonXPosition += dimWidth*0.25;
        }
        else{
        buttonYPosition += dimHeight/11;
        }
        if(currentModeSelected == i){
            //// ADD IN DESCRIPTION HERE
            gameModeGUI.drawDescriptionText(dimWidth*0.52, dimHeight*0.72, descriptionFontSize);
        }
    }
    
    for(int i = 0; i < amountOfModes; i++){
        if(modeButt[i]){
            
            return i;
        }
    }
    return currentModeSelected;
    
}

void gameMode::competitiveSelect(ofTrueTypeFont modeFont, bool localMouseReleased){
   
    int compWidthSize = competitiveMenuImages.getWidth()*0.9;
    int compHeightSize = competitiveMenuImages.getHeight()*0.9;

    int compXBegin = dimWidth*0.25;
    int compYBegin = dimHeight*0.535;
    
    for(int x = 0; x < 2; x++){
        gameModeGUI.notButton("Team " + ofToString(x+1) + ": " + teamRealName[x], guiRoundFont, compXBegin, dimHeight*0.38, dimWidth*0.4, dimHeight*0.055, teamColourBackground[x], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));

        
        gameModeGUI.imageButton(999,999,competitiveMenuImages, compXBegin, compYBegin, compWidthSize, compHeightSize, dimWidth/192, ofColor(0), localMouseReleased, teamColourBackground[x], ofColor(0), teamColourDepth[x], false, true);
            compXBegin += dimWidth*0.5;
        
        
    }
    
    
    int quickHeightSize = thumbnailImageArray[0].getHeight()*0.8;
    int quickWidthSize = thumbnailImageArray[0].getWidth()*0.8;
    
    int quickXBegin = dimWidth*0.25 - dimWidth*0.15;
    int quickYBegin = dimHeight*0.77;
    int picPos = 0;
    for(int x = 0; x < amountOfPictureChoice; x++){
        
        if(randImgNumGenerator[x] == true){
            
            
            gameModeGUI.imageButton(-999,999,thumbnailImageArray[x], quickXBegin, quickYBegin, quickWidthSize, quickHeightSize, dimWidth/192, ofColor(0), localMouseReleased, ofColor(255), ofColor(255), ofColor(255), false, tabletInteract);
            picPos +=1;
            quickXBegin += dimWidth*0.15;
        }
    }
}

void gameMode::survivalSelect(ofTrueTypeFont modeFont, bool localMouseReleased){
    
    gameModeGUI.notButton("Highscore", gameModeGUI.shapeNamesFont, dimWidth*0.25, dimHeight*0.4, dimWidth*0.4, dimHeight*0.07, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
    
    gameModeGUI.notButton(ofToString(highscoreCurrentSurvival), guiScoreFont, dimWidth*0.25, dimHeight*0.55, dimWidth*0.15, dimHeight*0.12, teamColourBackground[0], ofColor(0), true, 15*screenAdjustBoth, ofColor(0));
    
    gameModeGUI.notButton("Created on:", gameModeGUI.shapeNamesFont, dimWidth*0.75, dimHeight*0.4, dimWidth*0.4, dimHeight*0.07, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
    gameModeGUI.notButton(highscoreCurrentDate, guiScoreFont, dimWidth*0.75, dimHeight*0.55, dimWidth*0.35, dimHeight*0.12, teamColourBackground[0], ofColor(0), true, 15*screenAdjustBoth, ofColor(0));
}

void gameMode::simonSaysSelect(ofTrueTypeFont modeFont, bool localMouseReleased){
    
    
    int compWidthSize = competitiveMenuImages.getWidth()*0.9;
    int compHeightSize = competitiveMenuImages.getHeight()*0.9;
    
    int compXBegin = dimWidth*0.125;
    int compYBegin = dimHeight*0.535;
    
    for(int x = 0; x < 2; x++){
        gameModeGUI.notButton("Team " + ofToString(x+1) + ": " + teamRealName[x], guiRoundFont, dimWidth*0.25+(x*dimWidth*0.5), dimHeight*0.38, dimWidth*0.4, dimHeight*0.055, teamColourBackground[x], ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
        
        
        gameModeGUI.imageButton(999,999,simonMenuImages[x], compXBegin, compYBegin, compWidthSize, compHeightSize, dimWidth/192, ofColor(0), localMouseReleased, teamColourBackground[x==0?1:0], ofColor(0), ofColor(0), false, true);
        gameModeGUI.imageButton(999,999,simonMenuImages[x], compXBegin+dimWidth*0.25, compYBegin, compWidthSize, compHeightSize, dimWidth/192, ofColor(0), localMouseReleased, teamColourBackground[x], ofColor(0), teamColourCombined[x], false, true);
        compXBegin += dimWidth*0.5;
        
        
    }
    
    
    
}


void gameMode::quickplaySelect(ofTrueTypeFont modeFont, bool localMouseReleased){
    //// SO THIS RIGHT NOW IS REALLY SLOW COS ALL THE PICS. USE SMALLER IMAGES FOR FINAL
   // gameModeGUI.notButton("Select a shadow!", modeFont, dimWidth*0.75, dimHeight*0.26, dimWidth*0.45, dimHeight*0.07, ofColor(255), ofColor(0), true, 8*screenAdjustBoth, ofColor(0));
    
    bool thisImage[3] = {false, false, false};
    int quickHeightSize = thumbnailImageArray[0].getHeight()*0.75;
    int quickWidthSize = thumbnailImageArray[0].getWidth()*0.75;
    
    int quickXBegin = dimWidth*0.125-quickWidthSize*0.25;
    int quickXOrigin = dimWidth*0.125-quickWidthSize*0.25;
    int quickYBegin = dimHeight*0.5-dimHeight/12;
    //int quickYBegin = dimHeight/4.5;
    // ofEnableSmoothing();
    for(int i = 0; i < amountOfPictureChoice; i ++){
        if(i%6 == 0 && i !=0){
            quickYBegin = dimHeight*0.5+dimHeight/14;
            quickXBegin = quickXOrigin;
        }
        if(i == 3 || i == 9){
             quickXBegin = dimWidth*0.625-quickWidthSize*0.25;
         }
        thisImage[i] = gameModeGUI.imageButton(i, tabletPicID, thumbnailImageArray[i], quickXBegin, quickYBegin, quickWidthSize, quickHeightSize, dimWidth/192, (currentImageNumber==i?ofColor(205, 20, 20):ofColor(0)), localMouseReleased, teamColourBackground[0], (currentImageNumber==i?teamColourBackground[0]:ofColor(255)), ofColor(255), thisImage, tabletInteract);
        ofPushStyle();
        ofSetColor(currentImageNumber==i?ofColor(205, 20, 20):ofColor(0));
        ofDrawCircle(quickXBegin+quickWidthSize*0.5, quickYBegin+quickHeightSize*0.4, dimWidth*0.02);
        ofSetColor(currentImageNumber==i?teamColourBackground[0]:ofColor(225));
        ofDrawCircle(quickXBegin+quickWidthSize*0.5, quickYBegin+quickHeightSize*0.4, dimWidth/55);
        
        gameModeGUI.drawMessageGUI(ofToString(highscoreCurrentImage[i])+ "%", scoreFont, quickXBegin+quickWidthSize*0.5, quickYBegin+quickHeightSize*0.4, ofColor(0));
        ofPopStyle();
        quickXBegin += dimWidth*0.125+quickWidthSize*0.25;
        
        if(thisImage[i]){
            currentImageNumber = i;
            //cout << "playing with image " << currentImageNumber;
        }
        
    }
}
