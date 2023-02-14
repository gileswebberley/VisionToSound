#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(20);//equiv to 1200bpm
    //filepath in setUpInput for playback, blank for webcam
    myIn.setupInput();
    perfectFit = myPixelator.setGridPixels(myIn.getWidth(),myIn.getHeight());
    gridMax = myPixelator.getMaxColsRows();
    if(ignoring){
        //if pausing the reaction of grid sqs make the
        //appropriate vector (size,initial value)
        //no, make it zero else it won't play when first reactive
        ignoreTracker.assign(gridMax.x*gridMax.y,0);
        cout<<"IGNORING.."<<ignoreTracker.size()<<" squares for "<<ignoreTracker.at(1)<<" beats\n";
    }
    myPixelator.setDrawBoundsL(30,playThreshold);
    mySequencer.seqStart();
    //myListener.setupSpectrumGraph(10,myIn.getHeight(),myIn.getWidth()-20,300);
}

//--------------------------------------------------------------
void ofApp::update(){
    //update the input to make it all work
    myIn.updateInput();
    //myListener.updateSpectrum();
    //myListener.updateTempo();
    //if it's a new frame then read the pixelator grid
    //is it a beat of the sequencer
    if(mySequencer.isClick()){
        //inputselector.getPixelRead() returns a const reference to the current stored frame
        const ofPixels vf = myIn.getPixelRead();
        myPixelator.readGrid(&vf);
        cout<<"thats a beat....\n";
        //play a note if it's brighter than playThreshold
        //REMEMBER IT'S RANGE IS [1..gridMax.x/.y]
        for(int r=1; r<=gridMax.y;r++){
            for(int c=1; c<=gridMax.x;c++){
                //get the grid square info and play it's note
                try{//cos it throws an exception if out of bounds
                    //get <r,g,b,l> so tg.at(3) is our brightness value
                    vector<int> tg = myPixelator.getGridSquare(c,r);
                    //want to add a routine so it doesn't play the same
                    //note for a variable amount of beats?
                    int realn = (r-1)+(c-1);
                    //check if it's ready to play again
                    if(ignoring && ignoreTracker.at(realn)>0){
                        //this has a lovely effect, each square has a given number of beats muted
                        ignoreTracker.at(realn) -= 1;
                        //if this is a square that is muted go onto the next one
                        continue;
                    }else{//this square is not muted for this beat...
                        //> for highlights
                        if(tg.at(3)>playThreshold){
                            //if it's bright enough play a note
                            //reset ignore if it plays, random element so everything doesn't just play at once
                            int* seed = &c;
                            ofSeedRandom(*(seed));
                            ignoreTracker.at(realn) = ofRandom(1,ignoreCnt);
                            //note is based on position, scrolls through samples so always in bounds
                            currentNote = GwMusic::NOTES(realn);
                            //adds life to it, volume based on brightness
                            float vol = ofNormalize(tg.at(3),playThreshold/2,255);
                            //..and stereo position based on column
                            float pos = ofMap(c,1,gridMax.x,-1.0,1.0);
                            mySequencer.playNote(currentNote, vol,pos);
                        }
                    }

                }catch (Pixelator::pixelator_range_error& e) {
                    cout<<e.what();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    myIn.drawInput();
    myPixelator.draw();
    //myListener.drawSpectrum();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '&& mySequencer.seqPlaying()){
        mySequencer.seqStop();
    }else{
        mySequencer.seqStart();
    }
}
