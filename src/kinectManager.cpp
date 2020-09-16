//
//  kinectManager.cpp
//  SSFoxtonV0_1
//
//  Created by Mini on 22/09/17.
//
//

#include "gameMode.hpp"


void gameMode::gameModeSetup(){
    
    kinect.open(0);
    kinect.postPixelCheckImage.allocate(dimWidth, dimHeight, OF_IMAGE_COLOR_ALPHA);
    kinect.postPixelCheckImage.setColor(ofColor(0,0,0,0));
    debugSetup();
    gameModeGUI.gameParameters.add(kinect.minDistance.set("minDistance", 2100, 0, 12000)); // 2100
    gameModeGUI.gameParameters.add(kinect.maxDistance.set("maxDistance", 3300, 0, 12000)); // 3300
    
    for(int i = 0; i < amountOfPictureChoice; i++){
        randImgNumGenerator[i] = tempRandImgNumGeneratorTwo[i] = false;
    }
    
    
    maskFbo.allocate(dimWidth,dimHeight);
    fbo.allocate(dimWidth,dimHeight);
    maskOfImageWithBackground.allocate(dimWidth,dimHeight);
    
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/alphamask.vert","shaders_gles/alphamask.frag");
#else
    if(ofIsGLProgrammableRenderer()){
        string vertex = "#version 150\n\
        \n\
        uniform mat4 projectionMatrix;\n\
        uniform mat4 modelViewMatrix;\n\
        uniform mat4 modelViewProjectionMatrix;\n\
        \n\
        \n\
        in vec4  position;\n\
        in vec2  texcoord;\n\
        \n\
        out vec2 texCoordVarying;\n\
        \n\
        void main()\n\
        {\n\
        texCoordVarying = texcoord;\
        gl_Position = modelViewProjectionMatrix * position;\n\
        }";
        string fragment = "#version 150\n\
        \n\
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        in vec2 texCoordVarying;\n\
        \
        out vec4 fragColor;\n\
        void main (void){\
        vec2 pos = texCoordVarying;\
        \
        vec3 src = texture(tex0, pos).rgb;\
        float mask = texture(maskTex, pos).r;\
        \
        fragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
        shader.bindDefaults();
        shader.linkProgram();
    }else{
        string shaderProgram = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        \
        void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec3 src = texture2DRect(tex0, pos).rgb;\
        float mask = texture2DRect(maskTex, pos).r;\
        \
        gl_FragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        shader.linkProgram();
    }
#endif
    
    // Lets clear the FBOs
    // otherwise it will bring some junk with it from the memory
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    maskOfImageWithBackground.begin();
    ofClear(0,0,0,255);
    maskOfImageWithBackground.end();
    
}

void gameMode::depthMask(int colOpacity){
    // ------------------- The mask ------------------- //
    maskFbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255,255,255,255);
    if(blurDepthImg.isAllocated()){
    blurDepthImg.draw(dimWidth*0.5,dimHeight*0.5,dimWidth,dimHeight);
    }
    maskFbo.end();
    
    // ------------------- The image with depth colour and combined colour ------------------- //
    maskOfImageWithBackground.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(teamColourDepth[teamTwoIsTrue?1:0]);
    ofDrawRectangle(dimWidth*0.5,dimHeight*0.5, dimWidth, dimHeight);
    ofSetColor(teamColourCombined[teamTwoIsTrue?1:0]);
    if(kinect.contentImage.isAllocated()){
    kinect.contentImage.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // Silhouette
    }
    maskOfImageWithBackground.end();
    
    // ------------------- The masking effect ------------------- //
    fbo.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent for default
    ofClear(0, 0, 0, 0);
    //ofSetColor(teamColourDepth[teamTwoIsTrue?1:0]);
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo.getTexture(), 1 );
    
    /// SRC or image underneath (reveal, depth and shape combined)
    if(maskOfImageWithBackground.isAllocated()){
    maskOfImageWithBackground.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight); // Depth and reveal
    }
    shader.end();
    fbo.end();
    
    // ------------------- WHAT IS ACTUALLY DRAWN ------------------- //
    ofSetColor(255);
    if(fbo.isAllocated()){
    fbo.draw(dimWidth*0.5,dimHeight*0.5,dimWidth, dimHeight); // THIS IS THE MASK
    }
}


void gameMode::updateKinect(){
    
    kinect.update(totalShadowAmount);
    if( kinect.isFrameNew() ){
        imgDepth.setFromPixels(kinect.getDepthPixels()); // converts kinect depth to image
        imgDepth.update();
        
        if(shouldWeBlur) {  /////// BLUR UPDATE
            ofxCv::copy(imgDepth, blurDepthImg); // copies image for ofxcv to use, its also the actual displayed image and is required to blur
            if(useGaussian){
                ofxCv::GaussianBlur(blurDepthImg, radius);
            } else {
                ofxCv::blur(blurDepthImg, radius);
            }
            blurDepthImg.update();
        }
    }
}

void gameMode::basicKinect(bool dontShowDepth){
  //  ofSetColor(randomColDepth);
    updateKinect(); // Updates things relating to the kinect
    ofSetRectMode(OF_RECTMODE_CENTER);
    if (shouldWeBlur && blurDepthImg.isAllocated() && !dontShowDepth){
        blurDepthImg.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);
    }
    else if(imgDepth.isAllocated() && !dontShowDepth){
        imgDepth.draw(dimWidth/2,dimHeight/2, dimWidth, dimHeight);
    }

}

