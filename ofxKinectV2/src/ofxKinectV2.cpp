//
//  ofxKinectV2.cpp
//  kinectExample
//
//  Created by Theodore Watson on 6/23/14.
//
//

#include "ofxKinectV2.h"

//--------------------------------------------------------------------------------
ofxKinectV2::ofxKinectV2(){
    bNewFrame  = false;
    bNewBuffer = false;
    bOpened    = false;
    lastFrameNo = -1;
    //set default distance range to 50cm - 600cm
       
    
}

//--------------------------------------------------------------------------------
ofxKinectV2::~ofxKinectV2(){
    close();
}

//--------------------------------------------------------------------------------
static bool sortBySerialName( const ofxKinectV2::KinectDeviceInfo & A, const ofxKinectV2::KinectDeviceInfo & B ){
    return A.serial < B.serial;
}

//--------------------------------------------------------------------------------
vector <ofxKinectV2::KinectDeviceInfo> ofxKinectV2::getDeviceList(){
    vector <KinectDeviceInfo> devices;
    
    int num = protonect.getFreenect2Instance().enumerateDevices();
    for (int i = 0; i < num; i++){
        KinectDeviceInfo kdi;
        kdi.serial = protonect.getFreenect2Instance().getDeviceSerialNumber(i);
        kdi.freenectId = i;
        devices.push_back(kdi);
    }
    
    ofSort(devices, sortBySerialName);
    for (int i = 0; i < num; i++){
        devices[i].deviceId = i;
    }
    
    return devices;
}

//--------------------------------------------------------------------------------
unsigned int ofxKinectV2::getNumDevices(){
    return getDeviceList().size();
}

//--------------------------------------------------------------------------------
bool ofxKinectV2::open(unsigned int deviceId){
    
    vector <KinectDeviceInfo> devices = getDeviceList();
    
    if( devices.size() == 0 ){
        ofLogError("ofxKinectV2::open") << "no devices connected!";
        return false;
    }
    
    if( deviceId >= devices.size() ){
        ofLogError("ofxKinectV2::open") << " deviceId " << deviceId << " is bigger or equal to the number of connected devices " << devices.size() << endl;
        return false;
    }
    
    string serial = devices[deviceId].serial;
    return open(serial);
}

//--------------------------------------------------------------------------------
bool ofxKinectV2::open(string serial){
    close();
    
    params.setName("kinectV2 " + serial);
    
    bNewFrame  = false;
    bNewBuffer = false;
    bOpened    = false;
    
    int retVal = protonect.openKinect(serial);
    
    if(retVal==0){
        lastFrameNo = -1;
        startThread(true);
    }else{
        return false;
    }
    
    bOpened = true;
    return true;
}

//--------------------------------------------------------------------------------
void ofxKinectV2::threadedFunction(){
    
    while(isThreadRunning()){
        protonect.updateKinect(rgbPixelsBack, depthPixelsBack);
        rgbPixelsFront.swap(rgbPixelsBack);
        depthPixelsFront.swap(depthPixelsBack);
        
        lock();
        bNewBuffer = true;
        unlock();
    }
}



//--------------------------------------------------------------------------------
void ofxKinectV2::update(int totalShadowForPixel){
   
    if( ofGetFrameNum() != lastFrameNo ){
        bNewFrame = false;
        lastFrameNo = ofGetFrameNum();
    }
    if( bNewBuffer ){
        
        lock();
        rgbPix = rgbPixelsFront;
        bNewBuffer = false;
        rawDepthPixels = depthPixelsFront;
        
        unlock();
        
        if( rawDepthPixels.size() > 0 ){
            if( depthPix.getWidth() != rawDepthPixels.getWidth() ){
                depthPix.allocate(rawDepthPixels.getWidth(), rawDepthPixels.getHeight(), OF_IMAGE_COLOR_ALPHA);
            }
            
            float * pixelsF         = rawDepthPixels.getData();
            unsigned char * pixels  = depthPix.getData();
            
            int contPosX = 0;
            int contPosY = 0;
            int XMap = 0;
            int YMap = 0;
            float fillInX = float(kinectDepthWidth)/float(contentImage.getWidth());
            float fillInY = kinectDepthHeight/contentImage.getHeight();
            int localTotCount = 0; // Simon Says
            int contentImageSizeX = contentImage.getWidth();
            int contentImageSizeY = contentImage.getHeight();

            ofColor shapeContentColSecond;
           // if(fillInPixelsNow){
            //postPixelCheckImage.setColor(ofColor(255,255,255,0)); // We clear the whole image before adding in the pixels
           // }
            totalDepthCount = 0; // This is total depth. It is here so we can use it to reset the main timer
            
            if(fillInPixelsNow || getPicture){ // Simon says and everything else
                perCount = 0; // Galore
                subCount = 0; // For more
            }
            
            for(int i = 0; i < rawDepthPixels.size(); i++){
                if(rawDepthPixels[i] < minDistance){
                    depthPix.setColor(i*4, ofColor(0,0,0,0));
                }
                else if(rawDepthPixels[i] >= minDistance && rawDepthPixels[i] <= maxDistance){
                    depthPix.setColor(i*4, ofColor(255,255,255,255));
                    
                    if(doThePassiveCheck || fillInPixelsNow || getPicture){
                        contPosX = (i)%kinectDepthWidth; // We go from index to x and y MAY NEED A -1 IF WE GET OUT OF BOUNDS
                        contPosY = (i)/kinectDepthWidth; // 424
                        
                        if(doThePassiveCheck || fillInPixelsNow){ // Passive and main check
                            // Make the image to compare against at the same point
                            XMap = ofMap(contPosX, 0, kinectDepthWidth, 0, contentImageSizeX);
                         //   XMap = float(contPosX)*fillInX;
                           // cout << XMap << endl;
                            YMap = ofMap(contPosY, 0, kinectDepthHeight, 0, contentImageSizeY);
                        }
                    }
                    // ------------------- PASSIVE PIXEL CHECK ------------------- //
                    if(doThePassiveCheck){
                            shapeContentColSecond = contentImage.getColor(XMap, YMap);
                            if(shapeContentColSecond.a != 0 && arrayDustParticles.size()<4000){
                                // Hey! We got something!
                                if(!depthParticleBool[i]){ // So we only get one particle at a time you know...
                                    nextWord(XMap, YMap, i, shapeContentColSecond);
                                    depthParticleBool[i] = true;
                                }
                            }
                    }
                    // ------------------- OUR NEW PIXEL CHECK ------------------- //
                    else if(fillInPixelsNow){
                         shapeContentColSecond = safeZonePixelCheckImage.getColor(XMap, YMap);
                        // We first check the actual area to see if they got it
                        if(shapeContentColSecond.a != 0 && shapeContentColSecond != ofColor(255, 0, 0, 255)){
                            // Hey! We got something!
                            
                           // for(int pixX = -1; pixX <= 2; pixX++){
                               // for(int pixY = -1; pixY <= 2; pixY++){
                                 //   shapeContentColSecond = contentImage.getColor(XMap +pixX, YMap +pixY);
                                 //   postPixelCheckImage.setColor(XMap +pixX, YMap +pixY, shapeContentColSecond);// x, y
                           //     }
                          //  }
                            perCount ++;
                        }
                        else if(shapeContentColSecond == ofColor(255, 0, 0, 255)){
                            //// NOTHING, NOTHING AT ALL!!!
                        }
                        else{ // OUT OF BOUNDS YO!
                            subCount +=subCountAdder;
                        }
                    }
                    else if(getPicture){
                        
                       if(contPosX < XMapHigh && contPosX > XMapLow &&
                           contPosY < kinectDepthHeight && contPosY > YMapLow){
                            localTotCount ++; // A bit of a cheat, but this is roughly the difference in size between images
                        }
                    }
                    totalDepthCount ++; // Gives full count of space MOVE TO LOADIMAGE AREA
                }
                else if(rawDepthPixels[i] > maxDistance){
                    depthPix.setColor(i*4, ofColor(0,0,0,0));
                }
            }
            
            // ------------------- PIXEL CHECK SCORE CALCULATION ------------------- //
            if(fillInPixelsNow){
                postPixelCheckImage.update(); // Is now outside so we show the right info if not updated
                // If its less than 2% or 4,341.76 pixels then we dont think it is good enough
                totalShadowForPixel = ofMap(totalShadowForPixel, 0, contentImage.getWidth()*contentImage.getHeight(), 0, rawDepthPixels.size());
               // float pixelMultiplier = (float)totalDepthCount/(float)totalShadowForPixel;
                filledInShapePercentage = (float)(perCount - subCount); // Calculation can change a bit. Right now it is out of total depth we add or subtract based on area in silhoette. Not the best as it can be a small amount to get 100%
                //    cout << filledInShapePercentage << endl;
                // THIS IS A VERY WEIRD CALCULATION, NOT VERY ACCURATE BUT WORKS OK. SHOULD USE LOG
                filledInShapePercentage = ofMap(filledInShapePercentage, 0, totalShadowForPixel, 0, 100);
                if(filledInShapePercentage<0){
                    filledInShapePercentage = 0;
                }
                if (filledInShapePercentage >= gamePercentTrigger || weHaveFilledIt) { // Added the OR for survivial to stay good
                    weHaveFilledIt=true;
                }
                else{
                    weHaveFilledIt = false;                }
                
            }
            else if(getPicture){
                // If its less than 2% or 4,341.76 pixels then we dont think it is good enough
                filledInShapePercentage = ((float)localTotCount / (float)totalPictureArea) * 100;
                //cout << totalArea << "  " << localTotCount << " " << kinect.filledInShapePercentage << endl;
            }
        }
        bNewFrame = true;
        doThePassiveCheck = false;
        fillInPixelsNow = false; // This never needs to stay true as it should only be checked when we want to
        getPicture = false;
    }
}

// Makes all particles draw the next word
void ofxKinectV2::nextWord(int x, int y, int indexPos, ofColor thisIsTheColour) {
    // Draw word in memory
    // Only continue if the pixel is not blank
    
    dustParticle newParticle;
    // cout << particleIndex<< particleCount << "YENS" << endl;
    // Create a new particle
    newParticle = dustParticle();
    
    ofPoint randomPos = newParticle.generateRandomPos(KdimWidth*0.5, KdimHeight*0.5, (KdimWidth+KdimHeight)*0.5);
    newParticle.pos.x = x;
    newParticle.pos.y = y;
    newParticle.target.x = randomPos.x;
    newParticle.target.y = randomPos.y;
    
    newParticle.maxSpeed = ofRandom(2.0, 3.0);
    newParticle.maxForce = newParticle.maxSpeed*0.025;
    newParticle.particleSize = ofRandom(2, 8);
    newParticle.colorBlendRate = ofRandom(0.00005, 0.0003);
    newParticle.indexPos = indexPos;
    newParticle.startColor = thisIsTheColour;
    arrayDustParticles.push_back(newParticle);
    //   }
    
}



//--------------------------------------------------------------------------------
bool ofxKinectV2::isFrameNew(){
    return bNewFrame;
}

//--------------------------------------------------------------------------------
ofPixels ofxKinectV2::getDepthPixels(){
    return depthPix;
}

//--------------------------------------------------------------------------------
ofFloatPixels ofxKinectV2::getRawDepthPixels(){
    return rawDepthPixels;
}

//--------------------------------------------------------------------------------
ofPixels ofxKinectV2::getRgbPixels(){
    return rgbPix;
}

//--------------------------------------------------------------------------------
void ofxKinectV2::close(){
    if( bOpened ){
        waitForThread(true);
        protonect.closeKinect();
        bOpened = false;
    }
}


