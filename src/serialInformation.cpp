//
//  serialInformation.cpp
//  SSFoxtonV0_2
//
//  Created by Mini on 24/09/17.
//
//

#include "ofApp.h"


void ofApp::sendReceiveTCPMessages() {
    
    
    // If not connected, try again in 30 seconds - problems if close and reopen program too quickly. probably because we are not closing server coorrectly.
    if (!server.isConnected() && (ofGetElapsedTimeMillis() - lastConnected) > 30000) {
            lastConnected = ofGetElapsedTimeMillis();
            // if not connected, this is a problem, so keep trying:
            bool connected = false;
            
            connected = server.setup(8080);
            cout << "Connected as Server: " << connected << endl;
    }
    
    int clientNum = server.getNumClients();

    // Authenticate static IP address
    if (server.getNumClients() > 0 && server.getClientIP(clientNum-1) != authorisedIPAddress)  {
        
        cout << "BOOT this guy" << endl; // also would be useful to log the attempt ??
        server.disconnectClient(clientNum-1);
        cout << "Number of Clients: " << clientNum << endl;
        return;
    }
    
    for(int i = 0; i < server.getLastID(); i++) { // getLastID is UID of all clients
        
        if( server.isClientConnected(i) ) { // check and see if it's still around
           // cout << "WE ARE HERE" << endl;
            // maybe the client is sending something
            // if client isn't openframeworks, add a [/TCP] terminator at the end of the message
            
            string s = server.receive(i);
            if (s.length() > 0) {
                msgReceived = s;
                handleReceivedData(msgReceived);
                bTouchTablet = true;
            }
            
            //maybe we want to send something?
            if (needToUpdateClient) {
                cout << "send data to client" << endl;
                needToUpdateClient = false;
                server.send(i,msgToSend);
            }
        }
    }
}

void ofApp::handleReceivedData(string s) {
    
    // Get client input.
    
    // Erase the bullshit newlines
    ofStringReplace(s,"\n","");
    
    // Split string into array
    auto input = ofSplitString(s, ",");
    
    if (input.size() == 2) { // sanity check - expecting 2 values
        
        /* * * * * * * * * *
         input[0]; String description of setting/parameter
         input[1]; Integer value
         * * * * * * * * * * */
        
        inputSetting = input[0];
        inputValue = ofToInt(input[1]);
        
         cout << input[0] << "," << input[1] << endl;
        
        changeSetting(inputSetting, inputValue);
        
    }
}

void ofApp::changeSetting(string setting, int value) {
    
    if(inputSetting == "START"){
        startButt = true;
    }
    
    if(inputSetting == "MODE"){
        
        for (int i=0; i<gameModeObject.amountOfModes; i++) { // reset all buttons to false, we have a new selection
            gameModeObject.modeButt[i] = false;
        }
        
        gameModeObject.modeButt[inputValue] = true;
        //mode = (Mode) inputValue;
        
       // selection_Mode = inputValue;
        
    }
    
    if(inputSetting == "TIME"){
        
       // selection_Time = inputValue;
    }
    
    if(inputSetting == "ROUNDS"){
        
       // for (int i=0; i<numRounds; i++) { // reset all buttons to false, we have a new selection
       //     buttonState_Rounds[i] = false;
       // }
       // buttonState_Rounds[inputValue] = true;
        cout << "yes" << endl;
        gameModeObject.firstToWhatNum = inputValue+1;
    }
    
    if(inputSetting == "IMAGE"){
        
        gameModeObject.tabletPicID = inputValue;
        
    }
    
    if(inputSetting == "QUIT"){
        
        // Quit
        doWeWantToQuit = inputValue;
        cout << "quit !! "  << doWeWantToQuit << endl;
    }
    
    if(inputSetting == "QUITYESNO"){
        
        // Quit - yes or no
        if (inputValue == 1) { // YES QUIT
            timeToQuitYes = true;
            
        } else if (inputValue == 0) { // NO DON't
            timeToQuitNo = true;
        }
    }
    
    if(inputSetting == "BACK"){
        goBack = true;
    }
    
    if(inputSetting == "AGAIN"){
        playAgainButt = true;
    }
    
    if(inputSetting == "BEGIN"){
        startingGame = true;
    }
    
}

