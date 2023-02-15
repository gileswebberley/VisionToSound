#pragma once

#include "ofMain.h"
#include "pixelator.h"
#include "gwsequencer.h"
//#include "kelvinconverter.h"
//#include "gwfiltration.h"
#include "inputselector.h"
#include "gwlistening.h"

class ofApp : public ofBaseApp{
    //ofColor krgbFilter, krgb;
    //does the image cleanly divide into grid
    bool perfectFit{false}, ignoring{true};
    //make VID_FILE for playing a mov, and change setupInput to pass a filepath
    InputSelector myIn{IS_TYPES::VID_DEVICE};
    string vidFile{"water_reflections.mov"};
    //set pixelator cols, rows
    Pixelator myPixelator{6,4};
    //gridMax.x = column count
    //gridMax.y = row count
    ofPoint gridMax;
    //[0..255) how bright to trigger a sound
    int playThreshold{190};
    //set sequencer bpm, samples folder
    GwSequencer mySequencer{120,"drums_one"};
    //a default value of C
    NOTES currentNote{NOTES::C};
    //beats to wait between playing same note
    int ignoreCnt{4};
    vector<int> ignoreTracker;
    //GwFiltration myFilter;
    GwListening myListener;

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
};
