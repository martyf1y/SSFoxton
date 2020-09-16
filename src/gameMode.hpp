//
//  gameMode.hpp
//  SSFoxtonV0_1
//
//  Created by Matt Martin on 18/09/17.
//
//

#ifndef gameMode_hpp
#define gameMode_hpp

#include <stdio.h>
#include "ofMain.h"
#include "SSGUI.hpp"
#include "ofxKinectV2.h"
#include "ofxCv.h"

class gameMode {
    
public:
    
    
    void gameModeSetup();
    void reset(int modeSelected, bool preset);
    void debugSetup();
    void loadTeamColour(int teamInt, int whichTeam);
    int modeSelect(ofTrueTypeFont modeFont, bool localLeftMouseReleased, int currentModeSelected);
    void competitiveSelect(ofTrueTypeFont modeFont, bool localMouseReleased);
    void survivalSelect(ofTrueTypeFont modeFont, bool localMouseReleased);
    void quickplaySelect(ofTrueTypeFont modeFont, bool localMouseReleased);
    void simonSaysSelect(ofTrueTypeFont modeFont, bool localMouseReleased);
    void activeGame(ofTrueTypeFont modeFont);
    ofImage loadAnImage(int changeImageType, int imageArrayNum, int moveX, int moveY, float localResize);
    void updateContentImage(int LocalCurrentImageNumber);
    void drawDebug();
    void drawGameGUI(ofTrueTypeFont modeFont, int localColorOpacity);
    void drawScores(ofTrueTypeFont modeFont);
    void gameTimeReset(int newGameEndtime, int moveToThisState, bool shouldWeShowTimer);
    void comparingTwoTeams();
    void checkForHighscoreImages(int incomingScore);
    void checkForHighscoreSurvival(int incomingScore);
    //// Kinect functions
    void updateKinect();
    void basicKinect(bool showDepth);
    void postRoundSetups(bool localWeHaveFilledIt);
    void depthMask(int colOpacity);
    
    SSGUI gameModeGUI;

    enum activeStates : uint16_t{
        onYourMarks = 0,
        playThatGame = 1,
        thatIsTime = 2,
    };
    
    int currentActiveStatesINT; // Going through the game states
    
    // Main variables
    int amountOfModes = 4;
    string modeName[4] = {"Competitive", "Survival", "Simon Says", "Quick Play"};
    bool modeButt[4] = {false, false, false, false};
    float adjustSizeX;
    float adjustSizeY;
    float screenAdjustBoth;
   // int dimWidth;
   // int dimHeight;
    
    // ------------------- Mask Things ------------------- //

    // ofImage     srcImg; currentContent
    // ofImage     dstImg; blankimage
    ofImage     brushImg;
    
    ofFbo       maskFbo; //depthImage
    ofFbo       fbo;
    ofFbo       maskOfImageWithBackground;
    ofShader    shader;
    
    
    // ------------------- Kinect Visual Things ------------------- //
    ofxKinectV2 kinect;
    ofImage imgDepth;
    ofImage blurDepthImg;
    int amountOfPictureChoice = 12; // Total amount of content pictures we have in folder
    
    // ------------------- Content images ------------------- //
   // ofImage currentContentImage, currentPixelCheckImage;
   // ofImage passiveImageContent[5];
    // originalContentImageForSizeAdjust = 0
    // originalSafeZoneImageForSizeAdjust = 1
   // ofImage originalContentImageForSizeAdjust, originalSafeZoneImageForSizeAdjust;
   // ofImage originalContentImageForPixelCheck;
    ofImage saveGameImage[2];
    vector <ofImage> contentImageArray;
    vector <ofImage> noFillImageArray;
    vector <ofImage> thumbnailImageArray;
    vector <ofImage> blacksImageArray;
   // vector <ofImage> bgImage;
    ofImage endOfRoundBackground;
    float previousSilhoetteAdjust;

    // ------------------- Sounds ------------------- //
    ofSoundPlayer soundCommandsArray[14];
    ofSoundPlayer currentCommandSound;
    ofSoundPlayer soundEffectArray[5];
    ofSoundPlayer currentEffectSound;
    ofSoundPlayer currentEffectSound2;
ofSoundPlayer timerSound;
    // Button color
    const ofColor buttonColorLow = ofColor(243, 146, 0); // orange
    const ofColor buttonColorHigh = ofColor(34, 143, 209); // dark blue
   // ofColor(21,169,216); // blue
    // ofColor(27,20,100); // dark blue
    //========================================================================
    // ------------------- Values to reset ------------------- //

    // ------------------- Big Game variables ------------------- //
    bool endofGameYes = false; // This is my eject bool. It stops the game right away.
    bool thereAreTwoTeams = false;
    bool compareTheRound = false;
    bool areWeDoingANightMode = false;
    bool nightMode[2] = {false, false};
    int yesWeFinallyHaveAModeVariable;
    //int totCount, perCount;
    int gameStaller; // This is just a time staller to stop things going too fast.
    int totalShadowAmount = 1; // This is for the scoring to find the shadow pixel size
    
    // ------------------- Highscore data ------------------- //
 
    int highscoreCurrentImage[12] = {0,0,0,0, 0,0,0,0, 0,0,0,0};    // for each full sized original shape (quickplay only) // initialised array for safety
    int highscoreCurrentSurvival = 1; // for the number of levels completed in survival mode
    string highscoreCurrentDate = "";
    bool beatenHighscore;
    
    // ------------------- Flashy cool visuals ------------------- //
    int flashModeAmount = 0;
    int nightModeAmount = 0;
    int difficultyOpacity = 0;
    int previousTrigger = 0;
    float   scaleInt = 1;

    // ------------------- Competitive settings ------------------- //
    int firstToWhatNum = 3; // How many times a team has to win, this is also used to get the total survival rounds a team has to complete.
    bool randImgNumGenerator[12];
    bool secondRandImgNumGenerator[12];
    bool teamTwoIsTrue = false;
    bool teamTwoHasWon = false;
    bool itsADraw = false;
    

    // ------------------- Survival setting ------------------- //
    bool rapidFire = false;
    bool showRoundSettings = false;
    bool tempRandImgNumGeneratorTwo[12]; // Works better than I thought, just swap this with the original when team two start
    int weirdAlphaVariable = 0;
    int randomDifficultyMaker = 100; // This will change every round to get smaller until it get hit
    
    // ------------------- Simon Says settings ------------------- //
    bool takingPicture;
    bool fillInPicture;
    
    // ------------------- Quickplay settings ------------------- //
    int currentImageNumber = 0;
    int tabletPicID = 0;
    
    // ------------------- Game round settings ------------------- //
    bool gameTimeHasFinished = false;
    int roundEndTime; // How long the countdown goes for
    int roundShowShapeTime; // How long we look at the image
    float activeStartTime; // when do want to stop the Active timer
    bool showTimer = false;
    bool showScore = false;
    bool showTeams = false;
    int teamTotalScore[2] = {0, 0}; // Easier to have two variables than anything else. This is total of rounds
    float teamCurrentRound[2] = {0, 0}; // This is things like percent amount for a round
    int currentRound;
    string teamNames[21] = {"Foxton", "Flax Town", "Te Awahou", "Windy Mills", "Horo Heroes", "Dutchies", "Fizzy Foxes", "River Loop", "Tara Rangers", "Harakeke Heroines",
        "Flaxy Wonders", "Grey Nomads", "Bright Bulbs", "Oranje Boven", "Tasman", "River Rockers", "Birds of a Shadow", "Kurawiniwini", "Pepepe", "Lounge Lizards", "Manawatu"};
    string teamRealName[2] = {"Team", "Team"};
    int teamColourInt[2];
    ofColor teamColourBackground[2];
    ofColor teamColourShadow[2];
    ofColor teamColourDepth[2];
    ofColor teamColourCombined[2];
    ofImage currentScoreboardImage[2];
    ofImage competitiveMenuImages;
    ofImage simonMenuImages[2];
    ofImage winnerCup;
    //========================================================================
    
    // ------------------- Gameplay parameters ------------------- //
    ofParameter <int> dimWidth = ofGetWidth();                    // The screen size
    ofParameter <int> dimHeight = ofGetHeight();
    ofParameter <float> minDistance;
    ofParameter <float> maxDistance;
    ofParameter <int> triggerPerAmount = 25;              // The amount to trigger the game
    ofParameter <float> silhoetteAdjust = 0.7; // this changes the image size
    ofParameter <bool> tabletInteract = true;
    ofParameter <bool> movingShadow = false;


    ofParameter <int> gameEndTime; // THe time for the birds to show

    // ------------------- Visual parameters ------------------- //
    ofParameter<int> radius;
    ofParameter<bool> shouldWeBlur;
    ofParameter<bool> useGaussian;
    ofParameter<bool> useLinger;
    ofParameter<bool> withDepth;
    ofParameter<float> learningTime, thresholdValue;
    ofParameter<bool> useEdge;
    ofParameter<bool> crazyLand;
    ofParameter<int> crazyLandWindow;
    ofParameter<int> crazyLandStrength;
    int descriptionFontSize = ofGetWidth()/80;
    ofTrueTypeFont scoreFont, guiScoreFont, guiRoundFont, scoreNameFonts, timerFont;
};

#endif /* gameMode_hpp */
