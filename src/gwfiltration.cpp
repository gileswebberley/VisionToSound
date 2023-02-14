#include "gwfiltration.h"

GwFiltration::GwFiltration()
{

}

void GwFiltration::setFilter(ofColor rgb_filter){
    //set it with no transmission (should make everything this colour?
    setFilter(rgb_filter, 2);
}

void GwFiltration::setFilter(ofColor rgb_filter, float transmission){
    filterColour = rgb_filter;
    filterHue = filterColour.getHue();
    //transmission clamped as a percentage [0..100]
    transmission = ofClamp(transmission,0.0,100.f);
    //reverse it as the multiplier in applying
    //so 100% has no filtering effect...
    transmission = 100-transmission;
    normalisedTransmission = transmission/100.f;
}

void GwFiltration::applyFilter(ofPixels *iip)
{
    for(unsigned long i=0;i<iip->getWidth();i++){
        for(unsigned long j=0; j<iip->getHeight();j++){
            ofColor pixelVal{iip->getColor(i,j)};
            float hueVal = pixelVal.getHue()-(filterColour.getHue()*normalisedTransmission);
            pixelVal.setHue(hueVal);
            iip->setColor(pixelVal);
        }
    }
}
