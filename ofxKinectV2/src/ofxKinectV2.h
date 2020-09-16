//
//  ofxKinectV2.h
//  kinectExample
//
//  Created by Theodore Watson on 6/23/14.
//
//

#pragma once

#include "ofProtonect.h"
#include "ofMain.h"
#include "dustParticles.h"

class ofxKinectV2 : public ofThread{

    public:
    
        struct KinectDeviceInfo{
            string serial;
            int deviceId;   //if you have the same devices plugged in device 0 will always be the same Kinect
            int freenectId; //don't use this one - this is the index given by freenect2 - but this can change based on order device is plugged in
        };

        ofxKinectV2();
        ~ofxKinectV2(); 
        
        //for some reason these can't be static - so you need to make a tmp object to query them
        vector <KinectDeviceInfo> getDeviceList();
        unsigned int getNumDevices();
    
        bool open(string serial);
        bool open(unsigned int deviceId = 0);
        void update(int totalShadowForPixel);
        void nextWord(int x, int y, int indexPos, ofColor thisIsTheColour);
        void close();
    
    // ------------------- OUR VARIABLES ------------------- //
            // -------------------  ------------------- //
            // -------------------  ------------------- //
        bool isFrameNew();
        ofImage postPixelCheckImage; // The pixels revealed used to display on screen
        ofImage contentImage;
        ofImage safeZonePixelCheckImage;
    ofImage shadowImageForStart;
        bool weHaveFilledIt = false;
        bool fillInPixelsNow = false; // For the game pixel check
        bool doThePassiveCheck = false;
        bool getPicture = false;
        int amountOfPassiveImages = 1;
        bool depthParticleBool[217088];
    int totalDepthCount, perCount;
        float subCount;
        int filledInShapePercentage;
        int gamePercentTrigger; // triggerperAmount
        float subCountAdder = 1;
  const  int KdimWidth= ofGetWidth();
  const  int KdimHeight= ofGetHeight();
        vector <dustParticle> arrayDustParticles;

    const int kinectDepthWidth = 512;
    const int kinectDepthHeight = 424;
    const int XMapHigh = kinectDepthWidth*0.5 + kinectDepthWidth/3;
    const int XMapLow = kinectDepthWidth*0.5 - kinectDepthWidth/3;
    const int YMapLow = kinectDepthWidth-(kinectDepthWidth/3*2);
    const int totalPictureArea = (XMapHigh-XMapLow)*(kinectDepthHeight-YMapLow);
    const int totalDepth = kinectDepthWidth * kinectDepthHeight;
        ofPixels getDepthPixels();
        ofPixels getRgbPixels();
        ofFloatPixels getRawDepthPixels();
    
        ofParameterGroup params;
        ofParameter <float> minDistance;
        ofParameter <float> maxDistance;

    protected:
        void threadedFunction();

        ofPixels rgbPix;
        ofPixels depthPix;
        ofFloatPixels rawDepthPixels;
    
        bool bNewBuffer;
        bool bNewFrame;
        bool bOpened;
    
        ofProtonect protonect; 
    
        ofPixels rgbPixelsBack;
        ofPixels rgbPixelsFront;
        ofFloatPixels depthPixelsBack;
        ofFloatPixels depthPixelsFront;
        int lastFrameNo; 
};
