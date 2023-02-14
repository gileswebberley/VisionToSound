#ifndef GWHISTOGRAM_H
#define GWHISTOGRAM_H
#include "ofMain.h"
/*
 * create a few readings for quick comparison
 * make it so it can create different collections
 * so it can be used by gwListening to convert image
 * data to a fake sound spectrum?
 */

class gwHistogram
{
    //so the range is [0..255]
    vector<int> histogramNow, histogramThen;


public:
    gwHistogram();
    vector<int>& getHistogramFrom(ofPixels& img);
};

#endif // GWHISTOGRAM_H
