#ifndef GWSEQUENCER_H
#define GWSEQUENCER_H
#include "ofMain.h"
//using the jukebox as our player currently
#include "jukebox.h"

//will need these I imagine
//clashing with id.gold :(
namespace  GwMusic{

enum class NOTES{
    C,Db,D,Eb,E,F,Gb,G,Ab,A,Bb,B
};

//overload the inc/dec operators so you can
//cycle through the notes
inline NOTES& operator++ (NOTES& n){
    n = (n==NOTES::B)? NOTES::C : NOTES(int(n)+1);
    return n;
}

inline NOTES& operator -- (NOTES& n){
    n = (n==NOTES::C)? NOTES::B : NOTES(int(n)-1);
    return n;
}
}

//and now the class declaration
using GwMusic::NOTES;
class GwSequencer : public ofBaseApp
{
    int bpm;
    double beatLengthMillis, startTime;
    bool seq_playing{false}, click_track{false};
    /*needs a collection of sound files for the
     * notes to be played, use a Jukebox for now
*/
    Jukebox* jBox;

public:
    GwSequencer();
    //sng arg constructor so make explicit (it's not type conversion)
    explicit GwSequencer(int bpm);
    //use a Jukebox as the sound creator
    GwSequencer(int bpm, string pathToSoundsFolder, bool asSampler = false);
    //destructor to clear up our pointer explicitly
    ~GwSequencer(){delete jBox;}
    bool isClick();
    void click();
    void setBpm(int);
    bool seqPlaying(){return seq_playing;}
    void seqStart();
    void seqStop();
    void playNote(NOTES);
    //volume [0..1]
    void playNote(NOTES,float volume);
    //pan[-1..1]
    void playNote(NOTES,float volume,float pan);
    bool toggleClickTrack();
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
};

#endif // GWSEQUENCER_H
