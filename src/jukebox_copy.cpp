#include "jukebox.h"

Jukebox::Jukebox()
{
    Jukebox("./");
}

Jukebox::Jukebox(string fp){
    if(setFolderPath(fp))
    {
        cout<<"Jukebox created";
    }else//failed to load any files
    {
        cerr<<"Jukebox failed to load any files";
    }
    //do I need multiple players somehow??
    player.setMultiPlay(true);
    player.setLoop(false);
}

Jukebox::~Jukebox(){
    if(multi_mode){
        //I thought I could use delete[] ??
        for(ofSoundPlayer* sp:playerGroup){
            delete sp;
        }
    }
}

bool Jukebox::setFolderPath(string folder_path){
    tunes.open(folder_path);
    tunes.allowExt("wav");
    tunes.allowExt("mp3");
    tunes.listDir();
    tList = tunes.getFiles();
    jbSize = tList.size();
    //check that it found some files
    if(tList.empty())return false;
    return true;
}

bool Jukebox::toggleUnique(){
    unique_mode = !unique_mode;
    return unique_mode;
}

void Jukebox::makeMulti(int nOfPlayers){
    //check the player pool hasn't already been made
    if(multi_mode) return;
    multi_mode = true;
    for(int cnt=0;cnt<nOfPlayers;cnt++){
        playerGroup.push_back(new ofSoundPlayer);
        playerGroup[cnt]->setMultiPlay(true);
        playerGroup[cnt]->setLoop(false);
        cout<<"new sound player: playerGroup["<<cnt<<"]\t";
    }
    playerGroup.shrink_to_fit();
    cout<<"\nMultiplay capacity: "<<playerGroup.size()<<"\n";
}

void Jukebox::playFile(ofFile f, float vol=1.0){
    if(!player.isPlaying()){
        cout<<"player is available";
        player.load(f);
        player.setVolume(vol);
        player.play();
    }else if(multi_mode){
        for(ofSoundPlayer* sp : playerGroup){
            //check for a free player
            //cout<<"check player..\t";
            if(!sp->isPlaying()){
                sp->unload();
                cout<<"multi player is available";
                sp->load(f);
                sp->setVolume(vol);
                sp->play();
                break;
            }else{
                //cerr<<"multi player is busy\n";
            }
        }
        cerr<<"all players are busy\n";
    }else{
        cerr<<"\nIf you're running out of players try using makeMulti(int nOfPlayers)\n";
    }

}

ofFile Jukebox::selectByIndex(int i){
    //check arg is in bounds
    if(i>int(tList.size()-1))
    {//if > roll around to start
        i = i%tList.size();
    }else if(i<0)
    {//assume a negative is 0
        i = 0;
    }
    return tList.at(i);
}

ofFile Jukebox::selectRandom(){
    int me = 2;
    int* seedPtr = &me;
    //make it as random as possible
    int seed = ofGetElapsedTimeMicros()+(long)seedPtr;
    ofSeedRandom(seed);
    int selectTune = floor(ofRandom(1.0)*(jbSize-1));
    if(unique_mode){
        //whether to play each once
        tList.erase(tList.begin()+selectTune);
        tList.shrink_to_fit();
        cout<<"Jukebox has: "<<tList.size()<<" tracks loaded\n";
        if(tList.empty()){
            //if they've all been played in unique mode then refill collection
            tList = tunes.getFiles();
        }
    }

    return tList.at(selectTune);
}

void Jukebox::update(){
    ofSoundUpdate();
}
