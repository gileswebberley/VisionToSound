#ifndef INPUTSELECTOR_H
#define INPUTSELECTOR_H
#include "ofMain.h"
enum class IS_TYPES{
    NO_INPUT, VID_FILE, VID_DEVICE
};

class InputSelector
{    
    ofVideoGrabber vidGrabber;
    ofVideoPlayer vidPlayer;
    ofPixels currentPixels;
    int isWidth, isHeight;
    IS_TYPES type_flag{IS_TYPES::NO_INPUT};
public:

    InputSelector();
    explicit InputSelector(IS_TYPES type);
    bool setupInput();
    bool setupInput(string filepath);
    void updateInput();
    const ofPixels& getPixelRead();
    void drawInput();
    int getWidth(){return isWidth;}
    int getHeight(){return isHeight;}

};

#endif // INPUTSELECTOR_H
