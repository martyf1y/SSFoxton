//
//  SSGUI.hpp
//  SSFoxtonV0_1
//
//  Created by Matt Martin on 19/09/17.
//
//

#ifndef SSGUI_hpp
#define SSGUI_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxParagraph.h"



class SSGUI {
    
public:
    
    void drawDebug();
    void drawMessageGUI(string message, ofTrueTypeFont messageFont, int messageX, int messageY, ofColor messageColor);
    bool button(string buttName, ofTrueTypeFont buttFont, int buttX, int buttY, int buttWidth, int buttHeight, int strokeCurve, bool mousePressed, ofColor highlight, ofColor lowlight, bool letsGiveItTheVariable, bool weWantThatTablet);
    void notButton(string buttName, ofTrueTypeFont buttFont, int buttX, int buttY, int buttWidth, int buttHeight, ofColor lowlight, ofColor lFontColor, bool withABorder, int borderSize, ofColor borderColor);
    
     bool imageButton(int myID, int tabletPicID, ofImage buttPic, int buttX, int buttY, int buttWidth, int buttHeight, int strokeSize, ofColor strokeColor, bool mousePressed, ofColor highlight, ofColor lowlight, ofColor imageCol, bool letsGiveItTheVariable, bool weWantThatTablet);
    void imageNotButton(ofImage buttPic, int buttX, int buttY, int buttWidth, int buttHeight, int strokeSize, ofColor strokeColor, ofColor lowlight, ofColor imageCol);

    void roundedRect(float x, float y, float w, float h, float r);
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);
    
    void setupDescriptionText(int mode);
    void drawDescriptionText(float x, float y, int fontSize);
    void drawNameOfShape(ofColor c);
    
    ofParameterGroup mainParams;
    ofParameterGroup params;
    
    ofxPanel gameParameters;
    ofxPanel kinectVisualParameter;
    bool debugMode = true;              // Allow setting to change

    // Score board images - allocated space for 2 teams
    ofImage scoreboardSurvival[3];
    ofImage scoreboardQuickplay[3];
    ofImage scoreboardCompetitive[3];
    // Description paragraph
    ofxParagraph descriptionParagraph;
    
    vector < string > shapeNamesListed; // from text file
    int numLanguages = 3;
    string shapeNameWithLanguages[3] = {"Waka","Kano","Canoe"} ;
    ofTrueTypeFont shapeNamesFont;
    
    int dimWidth = ofGetWidth();
    int dimHeight = ofGetHeight();
};


#endif /* SSGUI_hpp */
