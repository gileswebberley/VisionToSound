#include "gwlistening.h"
/*
 * Need to make the fft log-bin based for actual notes
 */
GwListening::GwListening()
{
    clearTempo();
    //default visualisation
    setupSpectrumGraph(10,ofGetHeight(),ofGetWidth()-20,300);
    tempoTolerance = maxBassOut*0.45;
    clearTraining();
    //make a clean spectrum vector, saves checking in the 1st updateRms call
    spectrum.assign(bands,0.f);
    levelisedSpectrum.assign(bands,0.f);
}

void GwListening::setToDefault()//set values as used in SpectrumSampler
{
    bands = 128; bandBass = 0; bassSampleSize = 2;
    bandMidLow = bandBass+bassSampleSize; midLowSampleSize = 5;
    bandMid = bandMidLow+midLowSampleSize+1;
    midSampleSize =((bands-1)-bandMid)/2;
    bandTop = bandMid+midSampleSize; topSampleSize = (bands-1)-bandTop;
    minTempoTime = 2;
    clearTraining();
}

//should pick up the sound from any ofSoundPlayers that are running
void GwListening::updateSpectrum(){
    //pointer passed by our player to real spectrum
    //do not release from memory!!
    float* spectrumIn = ofSoundGetSpectrum(bands);
    //make the values decrease slowly until bumped back up
    for(int i = 0; i<bands; i++){
        spectrum.at(i) *= ofNoise(peakDropRate);
        spectrum.at(i) = max(spectrum.at(i), spectrumIn[i]);
    }
    //now update the measurements, ie 'listen'
    updateRMS();
    //no, tempo is a seperate functionality
    //updateTempo();
}

/*
 * This is functionality to make the spectrum measurements more
 * responsive in the top bands, a more visual representation.
 */
const std::vector<float>* GwListening::getLevelisedSpectrum()
{
    /*
     * only half of it is in human hearing range
     * so I'm going to try to produce an interpolated
     * version from the top half!??
     */
    //if(!levelisedSpectrum.empty()) levelisedSpectrum.clear();
    for(int i = 0; i<bands/2; i++){
    //try to level the top freq up
    float speci = spectrum.at(i);
    float specj = spectrum.at(i+1);
    float numerator = i;
    float denominator = bands/10;
    float multiplier = 1.1;//i/20;
    if(i>19){
        speci = ofClamp(speci*(numerator/denominator),0,2)*multiplier;
        specj = ofClamp(specj*(numerator/denominator),0,2)*multiplier;
    }else if(i<2){
        speci /= 2;
        specj /= 2;
            }
    float hzstep = 44100/bands;
        float compv = 0.f;
        compv = (speci+specj)/2;
        levelisedSpectrum.at(i*2) = speci;
        levelisedSpectrum.at((i*2)+1) = compv;
        //cout<<speci<<"("<<hzstep*(i+1)<<"Hz)\t";
        //cout<<levelisedSpectrum.at((i*2)+1)<<"("<<(hzstep*(i+1))+(hzstep/2)<<"Hz) interpolated\t";
    }
    //cout<<"\n\n";
    //return a const ptr so it can't be changed
    return &levelisedSpectrum;
}

//used by updateSpectrum - make private I think..
void GwListening::updateRMS(){
    //produce rms type figures for each band sample
    bm = 0;
    for(int i=0; i<bassSampleSize; i++){
        bm += pow(spectrum[bandBass+i],2.0);
    }
    bm /= bassSampleSize;
    bm = sqrt(bm);
    mlm = 0;
    for(int i=0; i<midLowSampleSize; i++){
        mlm += pow(spectrum[bandMidLow+i],2.0);
    }
    mlm /= midLowSampleSize;
    mlm = sqrt(mlm);
    mm = 0;
    for(int i=0; i<midSampleSize; i++){
        mm += pow(spectrum[bandMid+i],2.0);
    }
    mm /= midSampleSize;
    mm = sqrt(mm);
    tm = 0;
    for(int i=0; i<topSampleSize; i++){
        tm += pow(spectrum[bandTop+i],2.0);
    }
    tm /= topSampleSize;
    tm = sqrt(tm);
    //make sure that the ranges don't get zeroed out
    //whilst trying to 'learn' the song's ranges
    if(bm != 0.0){
        minBassIn = min(bm,minBassIn);
    }
    maxBassIn = max(bm,maxBassIn);
    if(mlm != 0.0){
        minMidLIn = min(mlm,minMidLIn);
    }
    maxMidLIn = max(mlm,maxMidLIn);
    if(mm != 0.0){
        minMidIn = min(mm,minMidIn);
    }
    maxMidIn = max(mm,maxMidIn);
    if(tm != 0){
        minTopIn = min(tm,minTopIn);
    }
    maxTopIn = max(tm, maxTopIn);

    //map the readings to the specified ranges
    bass = ofMap(bm,minBassIn,maxBassIn,minBassOut,maxBassOut,true);
    midL = ofMap(mlm,minMidLIn,maxMidLIn,minBassOut,maxBassOut,true);
    mid = ofMap(mm,minMidIn,maxMidIn,minMidOut,maxMidOut, true);
    top = ofMap(tm,minTopIn,maxTopIn,minTopOut,maxTopOut,true);
    //make this the output operator for the listening?
    cout<<"\nLISTENING: bass: "<<bass<<"\tmid low: "<<midL<<"\tmid high: "<<mid<<"\ttop: "<<top<<"\n";
}

void GwListening::clearTraining(){
    //learnt from days of tweeking with 24 songs
    maxBassIn = 0.8;
    minBassIn  = 0.01;
    maxMidLIn = 0.5;
    minMidLIn = 0.001;
    maxMidIn = 0.1;
    minMidIn = 0.01;
    maxTopIn = 0.005;
    minTopIn = 0.000001;
}

void GwListening::setupSpectrumGraph(int x, int y, int w, int h)
{
    cout<<"LISTENING: spectrum graph xywh: "<<x<<" "<<y<<" "<<w<<" "<<h<<"\n";
    spectrum.reserve(bands);
    specBarX = x;//10
    specBarW = w;//ofGetWidth() - 20;
    specBarH = h;//300;
    specBarY = y;//ofGetHeight();
    //why don't the frequency bars spread evenly across the screen??
    colCorrect = 0;
    if(bands == 128){
        colCorrect = 0.85;
    }else if(bands == 256){
        colCorrect = 0.92;
    }//but where do these numbers come from???
    specBarCol = round(specBarW/(bands))+colCorrect;
}

void GwListening::clearTempo()
{
    tempoSampling = false;
    tempoCaught = false;
    tempoUp = false;
    //maybe the first 4 bass hits (incase of dbl hits)
    tempoCount = 0;
    //set the length of tempo in seconds
    tempo = 30.0;
    tempoClock = ofGetElapsedTimef();
}

void GwListening::updateTempo(){
    if(bass >= tempoTolerance && tempoSampling == false && tempoCaught == false){
        tempoClock = ofGetElapsedTimef();
        tempoSampling = true;
        tempoUp =true;
        tempoTolerance = bass;
        cout<<"in the first tempo sample, bass up\n";
    }
    if(!tempoCaught && tempoSampling){
        if(bass < tempoTolerance && tempoUp == true){
            tempoUp = false;
            tempoCount++;
            cout<<"bass down\n";
        }
        if(tempoCount < 4){

            if(bass >= tempoTolerance && tempoUp == false){
                tempoUp = true;
                cout<<"bass up\n";
            }
        }else{
            float now = ofGetElapsedTimef();
            tempo = now - tempoClock;
            if(tempo > minTempoTime){
                tempoCaught = true;
                tempoSampling = false;
            }else{
                //dbl sample time
                tempoCount = 0;
            }
            cout<<"length of tempo measured is: "<<tempo<<"\n";
        }
    }
}


void GwListening::drawSpectrum(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //draw the spectrum chart
    ofSetColor(specBg);
    ofFill();
    //ofDrawRectangle(specBarX, specBarY, specBarW,-specBarH/2);
    ofSetColor(posBand);
    const std::vector<float>* sp = getLevelisedSpectrum();
    topColour.a = 127;
    int i = 0;
    //cout<<"LISTENING: draw loop:";
     for(float f : *sp){//for(int i = 0; i < bands; i++){
         //cout<<i<<" ";
         if(i==bandBass ||i==bandMidLow || i==bandMid || i==bandTop){
             ofSetColor(topColour);
         }else{
             ofSetColor(topColour);
         }
         ofSetCircleResolution(30);
         ofDrawCircle(specBarX+i*specBarCol,specBarY,f*specBarH/2);
         ofDrawRectangle(specBarX+i*specBarCol,specBarY,specBarCol-2,-f*specBarH);
         //ofNoFill();
         ++i;
     }
     //cout<<"\nLISTENING draw loop end\n";
}
