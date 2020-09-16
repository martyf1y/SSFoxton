//
//  dustParticles.cpp
//  particlesForOF
//
//  Created by Matt Martin on 2/07/17.
//
//

#include "dustParticles.h"

//------------------------------------------------------------------
dustParticle::dustParticle(){
        pos.set(0, 0);
        vel.set(0, 0);
        target.set(0, 0);
        acc.set(0, 0);
    
    closeEnoughTarget = 50;
    maxSpeed = 4.0;
    maxForce = 0.1;
    particleSize = 5; // irrelevant
    isKilled = false;
    
    startColor = ofColor(ofRandom(0.0, 255.0), ofRandom(0.0, 255.0), ofRandom(0.0, 255.0));
    
    targetColor = ofColor(227, 140, 45);
    //float hue = fmodf(ofGetElapsedTimef(),255);
   // targetColor.setHsb( hue, 255, 255);
     colorWeight = 0;
     colorBlendRate = 0.0001;
    
    
}

void dustParticle::move() {
   // float frameAdjust = 60/ofGetFrameRate();
    target.y -= ofRandom(2, 10);
    // Check if particle is close enough to its target to slow down
  //  float proximityMult = 1.0;
    float distance = ofDist(pos.x, pos.y, target.x, target.y);
   // if (distance < closeEnoughTarget) {
   //     proximityMult = distance/closeEnoughTarget;
   // }
    
    // Add force towards target
    ofVec2f towardsTarget(target.x, target.y);
 //   towardsTarget.ofSub(pos);
    towardsTarget -= pos;
    towardsTarget.normalize();
    towardsTarget *= maxSpeed;
    
    ofVec2f steer(towardsTarget.x, towardsTarget.y);
    steer -= vel;
    steer.normalize();
    steer *= maxForce;
    acc += steer;
    
    // Move particle
    vel += acc;
    pos += vel;
   // pos.y -= ofRandom(10);;
    acc *= 0;
    

}

void dustParticle::draw() {
    // Draw particle
    
    
    ofColor currentColor = startColor.lerp(targetColor, colorWeight);
   
        ofSetColor(currentColor);
        ofDrawEllipse(pos.x, pos.y, particleSize, particleSize);
    
    // Blend towards its target color
    if (colorWeight < 1.0) {
        colorWeight = MIN(colorWeight+colorBlendRate, 1.0);
    }
}

// Picks a random position from a point's radius
ofPoint dustParticle::generateRandomPos(int x, int y, float mag) {
    ofVec2f sourcePos(x, y);
    ofVec2f randomPos(ofRandom(0, x*2), ofRandom(0, y*2));
    
    ofVec2f direction = randomPos - sourcePos;
    direction.normalize();
    direction *= mag;
    sourcePos += direction;
    
    return sourcePos;
}


