#include "gwsequencer.h"
using GwMusic::NOTES;

GwSequencer::GwSequencer():bpm{120}
{
    GwSequencer{bpm};
}

GwSequencer::GwSequencer(int b)
{
    setBpm(b);
    //create sound in the click() method
    click_track = true;
}
GwSequencer::GwSequencer(int b, string folderPath, bool is)
{
    setBpm(b);
    //made jBox a pointer to avoid it
    //initialising before we want to use it
    //DELETED IN DESTRUCTOR!!!!!!!!!!!!!!
    jBox = new Jukebox{folderPath};
    //either sample style player
    if(is) jBox->setUpSamples();
    //OR a player pool style multi player
    if(!is) jBox->makeMulti(24);
}

//if a negative value it is a number of seconds between beats
void GwSequencer::setBpm(int b){
    //mustn't be zero as it's used as a divisor make it 60 by default? seems reasonable?
    b = (b==0)?60:b;
    //make 300bpm the maximum
    b = (b>300)?300:b;
    bpm = b;
    cout<<"BPM: "<<bpm<<"\t";
    startTime=ofGetElapsedTimeMillis();
    // use minus numbers for whole seconds
    beatLengthMillis = (bpm>0)?(60.f/bpm)*1000:abs(bpm)*1000;
    //cout<<"beat in milliseconds: "<<beatLengthMillis<<"\n";
}

void GwSequencer::playNote(NOTES note){
    //note is the index of the sounds array
    playNote(note,1.0,0.0);
}

void GwSequencer::playNote(NOTES note, float volume){
    playNote(note, volume,0.0);
}

void GwSequencer::playNote(NOTES note, float volume, float pan){
    //volume [0..1]
    //pan[-1..1] = [L..R]
    //cout<<"PLAY THE "<<" NOTE at vol: "<<volume<<"\n";
    //cerr<<"NOTE as an int value: "<<int(note)<<"\n";
    if(jBox->isSamplerMode()){
        jBox->playByIndex(int(note),volume,pan);
        return;
    }
    ofFile theNote = jBox->selectByIndex(int(note));
    jBox->playFile(theNote,volume,pan);
}

bool GwSequencer::isClick(){
    if(ofGetElapsedTimeMillis()-startTime >= beatLengthMillis && seq_playing){
        startTime = ofGetElapsedTimeMillis();
        return true;
    }
    return false;
}

bool GwSequencer::toggleClickTrack(){
    click_track = !click_track;
    return click_track;
}

void GwSequencer::click(){
    //do a click action

}

void GwSequencer::seqStart(){
    seq_playing = true;
    startTime = ofGetElapsedTimeMillis();
}



void GwSequencer::seqStop(){
    seq_playing = false;
}

void GwSequencer::setup(){

}

void GwSequencer::update(){
    if(click_track)//play clicks
    {
        if(isClick()) click();
    }
}

void GwSequencer::draw(){

}

void GwSequencer::keyPressed(int key){
    if(key == ' '){
        //space bar to start and stop
        if(seq_playing){
            seqStop();
        }else{
            seqStart();
        }
        seq_playing = !seq_playing;
    }
}
