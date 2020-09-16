//
//  SSGUI.cpp
//  SSFoxtonV0_1
//
//  Created by Matt Martin on 19/09/17.
//
//

#include "SSGUI.hpp"

void SSGUI::drawMessageGUI(string message, ofTrueTypeFont messageFont, int messageX, int messageY, ofColor messageColor){
    
    ofRectangle messageBounds;
    messageBounds = messageFont.getStringBoundingBox(message, 0, 0);
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
  //  ofSetColor(backgroundColor);
  //  ofDrawRectangle(messageX, messageY, messageBounds.width + 50, messageBounds.height + 20);
    ofSetColor(messageColor);
    messageFont.drawString(message, messageX-messageBounds.width/2, messageY+messageBounds.height/2);
    ofPopStyle();
}

bool SSGUI::button(string buttName, ofTrueTypeFont buttFont, int buttX, int buttY, int buttWidth, int buttHeight, int strokeCurve, bool mousePressed, ofColor highlight, ofColor lowlight, bool letsGiveItTheVariable, bool weWantThatTablet){
    
   
    bool buttPressed = false;
    ofPushStyle();
    if(ofGetMouseX() > buttX - buttWidth*0.5 &&
       ofGetMouseX() < buttX + buttWidth*0.5 &&
       ofGetMouseY() > buttY - buttHeight*0.5 &&
       ofGetMouseY() < buttY + buttHeight*0.5 ){
        ofSetColor(highlight);
        if(mousePressed){
            cout << "Button Pressed" << endl;
            buttPressed = true;
        }
    }
    else{
        ofSetColor(lowlight);
    }
    ofRectangle buttBounds;
    buttBounds = buttFont.getStringBoundingBox(buttName, 0, 0);
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    //ofDrawRectangle(buttX, buttY, buttWidth, buttHeight);
    roundedRect(buttX-buttWidth*0.5, buttY-buttHeight*0.5, buttWidth, buttHeight, strokeCurve);
    ofSetColor(0);

    buttFont.drawString(buttName, buttX-buttBounds.width*0.5, buttY+buttBounds.height*0.5);
    
    ofPopStyle();
    if(weWantThatTablet){
        return letsGiveItTheVariable;
    }
    return buttPressed;
}
void SSGUI::notButton(string buttName, ofTrueTypeFont buttFont, int buttX, int buttY, int buttWidth, int buttHeight, ofColor lowlight, ofColor lFontColor, bool withABorder, int borderSize, ofColor borderColor){
    
    ofPushStyle();
    
    
    ofRectangle buttBounds;
    buttBounds = buttFont.getStringBoundingBox(buttName, 0, 0);
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    //ofDrawRectangle(buttX, buttY, buttWidth, buttHeight);
    if(withABorder){
        ofSetColor(borderColor);
        roundedRect(buttX-buttWidth*0.5-borderSize*0.5, buttY-buttHeight*0.5-borderSize*0.5, buttWidth+borderSize, buttHeight+borderSize, borderSize);
    }
    ofSetColor(lowlight);
    roundedRect(buttX-buttWidth*0.5, buttY-buttHeight*0.5, buttWidth, buttHeight, borderSize);
    ofSetColor(lFontColor);
    
    buttFont.drawString(buttName, buttX-buttBounds.width*0.5, buttY+buttBounds.height*0.5);
    
    ofPopStyle();
}

bool SSGUI::imageButton(int myID, int tabletPicID, ofImage buttPic, int buttX, int buttY, int buttWidth, int buttHeight, int strokeSize, ofColor strokeColor, bool mousePressed, ofColor highlight, ofColor lowlight, ofColor imageCol, bool letsGiveItTheVariable, bool weWantThatTablet){
    bool buttPressed = false;
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(strokeColor);
    roundedRect(buttX-buttWidth*0.5-strokeSize*0.5, buttY-buttHeight*0.5-strokeSize*0.5, buttWidth+strokeSize, buttHeight+strokeSize, strokeSize);

    // detect tablet image selection
    if (weWantThatTablet) {
        if (myID == tabletPicID) {
            ofSetColor(highlight);
            buttPressed = true;
        } else {
            ofSetColor(lowlight);
        }
    } else {
        // detect mouse selection from click
        if (ofGetMouseX() > buttX - buttWidth*0.5 &&
         ofGetMouseX() < buttX + buttWidth*0.5 &&
         ofGetMouseY() > buttY - buttHeight*0.5 &&
         ofGetMouseY() < buttY + buttHeight*0.5 ){
         ofSetColor(highlight);
         if(mousePressed){
            cout << "Button Pressed" << endl;
            buttPressed = true;
         }
        }
        else{
            ofSetColor(lowlight);

        }
    }

    roundedRect(buttX-buttWidth*0.5, buttY-buttHeight*0.5, buttWidth, buttHeight, strokeSize);
    
    ofSetColor(imageCol);
     if(buttPic.isAllocated()){
    buttPic.draw(buttX, buttY, buttWidth, buttHeight);
     }
    ofPopStyle();
   
    return buttPressed;

    
}

void SSGUI::imageNotButton(ofImage buttPic, int buttX, int buttY, int buttWidth, int buttHeight, int strokeSize, ofColor strokeColor, ofColor lowlight, ofColor imageCol){
    
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(strokeColor);
    roundedRect(buttX-buttWidth*0.5-strokeSize*0.5, buttY-buttHeight*0.5-strokeSize*0.5, buttWidth+strokeSize, buttHeight+strokeSize, strokeSize*0.5);
    
    // detect tablet image selection
    
    
    ofSetColor(lowlight);
    roundedRect(buttX-buttWidth*0.5-strokeSize*0.25, buttY-buttHeight*0.5-strokeSize*0.25, buttWidth+strokeSize*0.5, buttHeight+strokeSize*0.5, strokeSize*0.5);
    
    ofSetColor(imageCol);
    if(buttPic.isAllocated()){
    buttPic.draw(buttX, buttY, buttWidth, buttHeight);
    }
    ofPopStyle();
    
}

void SSGUI::roundedRect(float x, float y, float w, float h, float r) {
    
    // reference: https://forum.openframeworks.cc/t/cannot-draw-a-basic-roundrectangle/4448/2
    
    ofBeginShape();
    ofVertex(x+r, y);
    ofVertex(x+w-r, y);
    quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
    ofVertex(x+w, y+h-r);
    quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);
    ofVertex(x+r, y+h);
    quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
    ofVertex(x, y+r);
    quadraticBezierVertex(x, y, x+r, y, x, y+r);
    ofEndShape();
}

void SSGUI::quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
    float cp1x = prevX + 2.0/3.0*(cpx - prevX);
    float cp1y = prevY + 2.0/3.0*(cpy - prevY);
    float cp2x = cp1x + (x - prevX)/3.0;
    float cp2y = cp1y + (y - prevY)/3.0;
    
    // finally call cubic Bezier curve function
    ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
};




void SSGUI::setupDescriptionText(int mode) {
    string describeIt = "Blank";
    if(mode == 0){
   //     describeIt = "Compete against another player or team - best out of 3 rounds wins! A new random shape appears each round and teams take turns scoring for the highest percentage.";
        describeIt = "Compete against another player or team - best out of 3 rounds wins!";
    }
    else if(mode == 1){
        describeIt = "Fill in shape after shape in this non-stop game mode before the timer runs out!";
       // describeIt = "Fill in shape after shape in this non-stop game mode. Complete each shape to the required percentage before the timer runs out!";

    }
    else if(mode == 2){
        describeIt = "Each team takes a turn to create a shape on screen for the opposing team to fill in as best they can.";
        //        describeIt = "Create your own shadow shape and have a buddy fill it in! Each team takes a turn to create a weird and wonderful shape on screen for the opposing team to fill in as best they can.";

    }
    else if(mode == 3){
        describeIt = "Short and sweet. Pick a shape, fill it in, get a score. Try to beat the highscore!";
    }
    descriptionParagraph.setWidth(dimWidth/2.2); // change
    descriptionParagraph.setText(describeIt);
    descriptionParagraph.setColor(ofColor(0,255));
    descriptionParagraph.setIndent(0);
    descriptionParagraph.setLeading(10);
    descriptionParagraph.setSpacing(15);
    
    // cout << "setting up description text for mode: " << mode << endl;
    
}

void SSGUI::drawDescriptionText(float x, float y, int fontSize) {
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(30,125);
    ofDrawRectangle(x, y+dimHeight*0.008, descriptionParagraph.getWidth()+dimWidth*0.02, descriptionParagraph.getHeight()+dimHeight*0.022);
    ofSetColor(255,255);
    ofDrawRectangle(x-dimWidth*0.007, y, descriptionParagraph.getWidth()+dimWidth*0.02, descriptionParagraph.getHeight()+dimHeight*0.022);
    ofSetColor(255,255);
    descriptionParagraph.draw(x+dimWidth*0.007,y+dimHeight*0.035);
    ofPopStyle();
    
}


void SSGUI::drawNameOfShape(ofColor c) {
    
    // Display each name, starting at position nameY and moving down
    float nameX = dimWidth-dimWidth*0.05;
    
    //Set up a bounding box
    ofRectangle textBounds;
    textBounds = shapeNamesFont.getStringBoundingBox(shapeNameWithLanguages[0], 0,0);
    int gap = textBounds.height+textBounds.height*0.5;
    float nameY = dimHeight*0.18-(textBounds.height*1.5);

    // Draw name
    for (int i=0; i< numLanguages; i++) {
        textBounds = shapeNamesFont.getStringBoundingBox(shapeNameWithLanguages[i], 0,0);
        
        ofSetColor(c);
        shapeNamesFont.drawString(shapeNameWithLanguages[i], nameX-textBounds.width, nameY);
        
        nameY += gap;
    }
    
}


