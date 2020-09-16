//
//  dustParticles.h
//  particlesForOF
//
//  Created by Matt Martin on 2/07/17.
//
//
#pragma once
#include "ofMain.h"

//bool drawAsPoints;

class dustParticle {
  
public:
    
    dustParticle();
    
    void move();
    void draw();
    
    ofPoint generateRandomPos(int x, int y, float mag);

    
    ofVec2f pos;
    ofVec2f vel;
    ofVec2f acc;
    ofVec2f target;
    
    float closeEnoughTarget;
    float maxSpeed;
    float maxForce;
    float particleSize;
    bool isKilled;
    
    ofColor startColor;
    ofColor targetColor;
    float colorWeight;
    float colorBlendRate;
    int indexPos;
};
