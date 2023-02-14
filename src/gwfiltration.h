#ifndef GWFILTRATION_H
#define GWFILTRATION_H
#include "ofMain.h"

class GwFiltration
{
   ofColor filterColour{0};//clear glass default
   float filterHue{0}, normalisedTransmission{0.99};
public:
    GwFiltration();
    //could make an nd filter or something that is applied when you get a grid square vector
    void setFilter(ofColor rgb_filter);
    void setFilter(ofColor rgb_filter, float transmission);
    void applyFilter(ofPixels* input_image_pixels);
};

#endif // GWFILTRATION_H
