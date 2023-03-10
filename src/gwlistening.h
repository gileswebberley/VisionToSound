#ifndef GWLISTENING_H
#define GWLISTENING_H
#include "ofMain.h"

class GwListening
{
    //not sure so I'm not going to make them constants
    int bands{256},bandBass{0},bassSampleSize{2};
    int bandMidLow{bandBass+bassSampleSize},midLowSampleSize{5};
    int bandMid{bandMidLow+midLowSampleSize+1},midSampleSize{((bands-1)-bandMid)/2};
    int bandTop{bandMid+midSampleSize},topSampleSize{(bands-1)-bandTop};
    int specBarX, specBarY, specBarH, specBarW;
    int minTempoTime{2};
    float specBarCol, colCorrect;
    ofColor specBg{46}, posBand{156}, negBand{128}, topColour;
    float tempoCount, tempo, tempoTolerance, tempoClock;
    bool tempoSampling, tempoUp, tempoCaught;
    float bass,midL, mid, top,bm,mlm,mm,tm;//, time0;
    float peakDropRate{0.985};
    float maxBassIn, minBassIn, maxMidIn, minMidIn;
    float minTopIn, maxTopIn, minMidLIn, maxMidLIn;
    //these are the scaling factors that are mapped to
    float maxBassOut{400},minBassOut{50},maxMidOut{2},minMidOut{0.1};
    float maxTopOut{1}, minTopOut{0.5};
    std::vector<float> spectrum;
    std::vector<float> levelisedSpectrum;
    void updateRMS();
public:
    GwListening();
    //set up the default values as above
    void setToDefault();
    void setupSpectrumGraph(int x, int y, int w, int h);
    const std::vector<float>* getLevelisedSpectrum();
    void setBassOut(float min, float max){minBassOut = abs(min); maxBassOut = abs(max);}
    void setMidOut(float min, float max){minMidOut = abs(min); maxMidOut = abs(max);}
    void setTopOut(float min, float max){minTopOut = abs(min); maxTopOut = abs(max);}
    float getBass(){return bass;}
    float getMidLow(){return midL;}
    float getMidHigh(){return mid;}
    float getTop(){return top;}
    float normFromBass(float s){return ofNormalize(s,minBassOut,maxBassOut);}
    float normFromTOp(float s){return ofNormalize(s,minTopOut,maxTopOut);}
    void updateSpectrum();
    void drawSpectrum();
    void clearTempo();
    void updateTempo();
    void clearTraining();
};

#endif // GWLISTENING_H
