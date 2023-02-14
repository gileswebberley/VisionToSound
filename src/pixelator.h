#ifndef PIXELATOR_H
#define PIXELATOR_H

#include "ofMain.h"

class Pixelator : public ofBaseApp
{
    int nOfColumns, nOfRows, totalGSqs;
    int colPixelsCnt,rowPixelsCnt, drawAlpha{100},minDrawBoundsL{1},maxDrawBoundsL{255};
    bool is_drawing{true}, initialised{false};
    vector<int> red, green, blue, bright;
    int xsWidthPixels, xsHeightPixels;
    //does this belong??

public:
    struct pixelator_range_error
    {
        pixelator_range_error(int x):myx{x}{}
        int myx;
        const char * what() {
            return "Access out of grid bounds";
        }

    };

    Pixelator();
    Pixelator(int cols,int rows);
    //must call when size changes
    bool setGridPixels(int w, int h);
    //do not use without setting the grid
    void readGrid(const ofPixels* img);
    //throws pixelator_range_error
    vector<int> getGridSquare(int c, int r);
    //x is columns, y is rows; for looping
    ofPoint getMaxColsRows();
    int getTotalGridSquares(){return totalGSqs;}
    //found myself wanting to change draw behaviour so added this functionality
    void setDrawBoundsL(int low, int high);

    void draw() override;
};

#endif // PIXELATOR_H
