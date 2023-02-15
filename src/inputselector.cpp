#include "inputselector.h"

InputSelector::InputSelector()
{
    InputSelector(NO_INPUT);
}
InputSelector::InputSelector(IS_TYPES type):type_flag{type}{

}

bool InputSelector::setupInput(string filepath)
{
    if(type_flag == IS_TYPES::VID_FILE)
    {
        if(vidPlayer.load(filepath)){
        isWidth = vidPlayer.getWidth();
        isHeight = vidPlayer.getHeight();
        vidPlayer.setLoopState(OF_LOOP_NORMAL);
        vidPlayer.setVolume(0.f);
        vidPlayer.play();
        return true;
        }

    }
    return false;
}

bool InputSelector::setupInput()
{
    if(type_flag == IS_TYPES::VID_DEVICE)
        {
            vidGrabber.setVerbose(true);
            vidGrabber.listDevices();
            //check which index the camera you wish to use has in the device list
            vidGrabber.setDeviceID(1);
            if(vidGrabber.initGrabber(640,360)){
            isWidth = vidGrabber.getWidth();
            isHeight = vidGrabber.getHeight();
            return true;
            }
        }
    return false;
}

void InputSelector::updateInput(){
    if(type_flag == IS_TYPES::VID_DEVICE){
        vidGrabber.update();
        return;
    }
     if(type_flag == IS_TYPES::VID_FILE){
         vidPlayer.update();
         return;
     }
}

const ofPixels& InputSelector::getPixelRead()
{
    if(type_flag == IS_TYPES::VID_DEVICE){
        if(vidGrabber.isFrameNew()){
            currentPixels = vidGrabber.getPixels();
        }
    }else if(type_flag == IS_TYPES::VID_FILE){
        if(vidPlayer.isFrameNew()){
            currentPixels = vidPlayer.getPixels();
        }
     }
    return currentPixels;
}

void InputSelector::drawInput()
{
    float sc;
    if(type_flag == IS_TYPES::VID_DEVICE){
        sc = ofGetWidth()/vidGrabber.getWidth();
        ofScale(sc);
        vidGrabber.draw(0,0);
    }else if(type_flag == IS_TYPES::VID_FILE){
        sc = ofGetWidth()/vidPlayer.getWidth();
        ofScale(sc);
        vidPlayer.draw(0,0);
    }
}
