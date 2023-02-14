#ifndef JUKEBOX_H
#define JUKEBOX_H
#include "ofMain.h"

class Jukebox : public ofBaseApp
{
    //maybe a collection of 12 players, like a pool?
    vector<ofSoundPlayer*> playerGroup;
    ofSoundPlayer player;
    vector<ofFile> tList;
    ofDirectory tunes;
    int jbSize{0};
    bool unique_mode{false}, multi_mode{false};

public:
    Jukebox();
    Jukebox(string folder_path);
    ~Jukebox();
    void makeMulti(int nOfPlayers);
    ofFile selectRandom();
    ofFile selectByIndex(int);
    //volume has default value of 1.0 if left out
    void playFile(ofFile, float volumeNormalised);
    bool setFolderPath(string folder_path);
    //true to remove track after playing until
    //tList is empty, then refill
    bool toggleUnique();
    void update() override;
};

#endif // JUKEBOX_H
