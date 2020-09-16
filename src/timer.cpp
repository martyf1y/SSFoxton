//
//  timer.cpp
//  SSFoxtonV0_1
//
//  Created by Matt Martin on 20/09/17.
//
//

#include <stdio.h>
#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::mainTimerGoing(int passiveEndTime){
    //  currentGameState = tabletShare->currentGameState;
    
    if((timerMouseReleased) ||
       (bTouchTablet) ||
       (currentGameState == passive) ||
       (gameModeObject.kinect.totalDepthCount > 2000 && currentGameState == active)){ // Have taken out active art of this so timer can reset if players ahve left
        passiveStartTime = ofGetElapsedTimeMillis();  // reset the passive timer
        timerMouseReleased = false;
        bTouchTablet = false;
    }
    else{
       
    float timer = ofGetElapsedTimeMillis() - passiveStartTime;
    
    if(timer >= passiveEndTime) {
        currentGameState = passive;
        cout << "Passive Timer Reached" << endl;
        needToUpdateClient=true;
        msgToSend="BACKTOPASSIVE";
        currentModeINT = 99;
        gameModeObject.reset(99, true);
    //    currBackground = bgImage[0];
        resetPassive();
        doWeWantToQuit = false;
        ///// RESET EVERYTHING HERE //////
    }
    }
}

//--------------------------------------------------------------
bool ofApp::gameTimerGoing(int activeEndTime){

    float timer = ofGetElapsedTimeMillis() - gameModeObject.activeStartTime;
    
    if(timer >= activeEndTime) {
        //// please reset the timer within the game mode, not here.
        cout << "Active Timer Reached" << endl;
        
        return true;
    }
    return false;
}
