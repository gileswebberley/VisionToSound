#include "gwhistogram.h"

gwHistogram::gwHistogram()
{
    histogramNow.assign(256,0);
    histogramThen.assign(256,0);
}

vector<int>& gwHistogram::getHistogramFrom(ofPixels& img){
    histogramThen = histogramNow;
    for(int i=0; i<img.size(); i++){
        cout<<"HISTOGRAM: p in img: "<<img[i]<<"\t";
        //histogramNow.at(p.getL)
    }
    cout<<"\n";
    return histogramNow;
}
