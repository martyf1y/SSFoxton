#pragma once

#include "ofMain.h"
//#include "TabletApp.h"
#include "gameMode.hpp"
#include "SSGUI.hpp"
#include <ofxNetwork.h>

class ofApp : public ofBaseApp{
    
public:
    
    // Setup function
    void loadFont();
    void loadImages(int totsContent);
    void loadScoreboardImages();
    void loadHighscoreImages();
    void loadHighscoreSurvival();
    void loadSounds();
    void resetPassive();
    
    void setup();
    void update();
    void draw();
    void keyReleased(int key);
    void mouseReleased(int x, int y, int button);

    void mainTimerGoing(int passiveEndTime);
    bool gameTimerGoing(int activeEndTime);
    
    //========================================================================
    // ------------------- SERVER THINGS ------------------- //

    // ------------------- Custom functions ------------------- //
    void handleReceivedData(string s);
    void sendReceiveTCPMessages();
    void changeSetting(string setting, int value);
    void roundedRect(float x, float y, float w, float h, float r);
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);
    void drawButton(string buttonName, ofTrueTypeFont buttFont, int buttX, int buttY,
                    int buttWidth, int buttHeight, bool selected, ofColor highlight, ofColor lowlight);
   
    // ------------------- Server setup and incoming data ------------------- //
    ofxTCPServer server;
    
    int serverPort = 8080;
    string authorisedIPAddress = "192.168.1.171";
    
    long lastConnected = 0;
    
    string inputSetting = "";
    int inputValue = 0;
    string msgReceived = "";
    string msgToSend = "HELLO";
    bool needToUpdateClient = false;
    bool bTouchTablet = false;
    
    
    
    //========================================================================

    
    
    /// OUR CLASSES/OBJECTS
 //   shared_ptr<TabletApp> tabletShare; // Sharing the tablet info with the main app
    gameMode gameModeObject; // hopefuly this will change back after tablet is done
    
    // Main variables / global variables
    int dimWidth = ofGetWidth();
    int dimHeight = ofGetHeight();
    
    bool debugMode = false;

    ///////// OUR STATES /////////
    enum gameState {
        passive,
        selective,
        active,
        ruminative
    };
    
    gameState currentGameState;
    
    enum gameModeState : uint16_t {
        
        competitive = 0, // each person has to complete a set of images within a short amount of time.
        survival = 1, // A person has to complete images after image, each time the space allowed outside and inside the silhoette gets harder.
        simonSays = 2, // This mode takes a picture of silhouette and the next person has to try match it.
        quickPlay = 3 // Fastest way to get in a game, select an image and complete it.
    };
    
    int previousPassiveParticleSize = 0;
    
    ofImage titleImg;
    ofImage ruminationBackground;

    
   //  ofColor backgroundC;
    ofTrueTypeFont screenTitleFont, screenModeFont; // font with macrons
    
    /////// Timer things ///////
    float   passiveStartTime;
    bool    bPassiveTimerReached; // used as a trigger when we hit the timer
    
    // Colour palette - safest way to generate background colour
    ofColor backgroundCol;
    ofColor depthCol;
    // Button color
    const ofColor buttonColorLow = ofColor(243, 146, 0); // blue
    const ofColor buttonColorHigh = ofColor(34, 143, 209); // dark blue
    bool timeToQuitYes = false;
    bool timeToQuitNo = false;

    /// Button interaction
    bool leftMouseReleased = false;
    bool timerMouseReleased = false;;
    bool startButt = false; // The button for getting out of passive mode.
    bool goBack = false;
    bool playAgainButt = false; // The button for getting out of passive mode.
    bool startingGame = false;
    bool newGameButt = false;
    bool doWeWantToQuit = false;
    //////////// Values to reset ////////////
    //========================================================================
 
    int currentModeINT = 99;
};
