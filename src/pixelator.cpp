#include "pixelator.h"
//default constructor sets 1 octave as it's designed
//for creating music from video originally
Pixelator::Pixelator()
{
    Pixelator(12,1);
}

//c: no.of columns, r: no.of rows
Pixelator::Pixelator(int c, int r)    :nOfColumns{c},nOfRows{r},
    totalGSqs{nOfColumns*nOfRows}
{
    red.reserve(totalGSqs);
    green.reserve(totalGSqs);
    blue.reserve(totalGSqs);
    bright.reserve(totalGSqs);
}

/*
 * if w is not divisible by nOfColumns or
 * h is not divisible by nOfRows reurn false
 * else set the number of pixels that make up
 * each grid squares w and h
 * the whole loop over the pixel[] could go
 * badly wrong otherwise (need to build a
 * workaround (perhaps have excess wP and hP?)
 * ++implemented excess pixels being ignored
 */
bool Pixelator::setGridPixels(int w, int h){
    colPixelsCnt = w/nOfColumns;
    rowPixelsCnt = h/nOfRows;
    //any excess pixels to add to a row or column
    xsWidthPixels = w%colPixelsCnt;
    xsHeightPixels = h%rowPixelsCnt;
    cout<<"colPixelsCnt: "<<colPixelsCnt<<"\trowPixelsCnt: "<<rowPixelsCnt<<"\n";
    if(xsWidthPixels!=0||xsHeightPixels!=0){
        //to indicate it's not a perfect fit but it's not fatal
        //as it's looked after in readGrid, so just false
        return false;
    }
    return true;
}

//The point of this class is this functionality,
//averages colour info for each grid square of
//the pixel data passed in
void Pixelator::readGrid(const ofPixels* img){
    //check argument first...
    if(img->size()==0)return;
    if(img == NULL) return;
    if((int)img->getWidth()!=nOfColumns*colPixelsCnt+xsWidthPixels||(int)img->getHeight()!=nOfRows*rowPixelsCnt+xsHeightPixels){
        //check that the image is the size we're expecting
        //if not have a little moan then fix it...
        cerr<<"tried to access out of bounds, MUST CALL setGridPixels() first";
        setGridPixels(img->getWidth(),img->getHeight());
    }
    //to stop attempting to access a grid square if not read
    if(!initialised)initialised = true;
    cout<<"readGrid():\n";
    red.clear();
    green.clear();
    blue.clear();
    bright.clear();
    for(int r=0;r<nOfRows;r++){
        //goes through each grid row
        for(int c=0; c<nOfColumns;c++){
            //then each column of the row
            //create the tmp colour variables
            double tmpR{0},tmpG{0},tmpB{0},tmpL{0};
            for(int i=0;i<rowPixelsCnt;i++){
                //each row of pixels in gridSq
                for(int j=0;j<colPixelsCnt;j++){
                    //each column of row of pixels in gridSq
                    int realx = j+(colPixelsCnt*c);
                    int realy = i+(rowPixelsCnt*r);
                    //grab the pixel colour info
                    ofColor pixelVal{img->getColor(realx,realy)};
                    //create rms values for each gridSq
                    tmpR+=pow(pixelVal.r,2.0);
                    tmpG+=pow(pixelVal.g,2.0);
                    tmpB+=pow(pixelVal.b,2.0);
                    //human perception lightness
                    //0.299*R + 0.587*G + 0.114*B
                    unsigned char percept = (pixelVal.r*0.299)+(pixelVal.g*0.587)+(pixelVal.b*0.114);
                    tmpL+=pow(percept,2.0);
//                    tmpL+=pixelVal.getLightness()*pixelVal.getLightness();
                }
            }
            //total number of squared values collected
            int divisor = colPixelsCnt*rowPixelsCnt;
            //then divide by n and take square root
            tmpR = sqrt(tmpR/divisor);
            tmpG = sqrt(tmpG/divisor);
            tmpB = sqrt(tmpB/divisor);
            tmpL = sqrt(tmpL/divisor);
            //each grid square's readings
            //cout<<"\t\t\t red: "<<tmpR<<" green: "<<tmpG<<" blue: "<<tmpB<<" brightness: "<<tmpL<<"\n";
            //add the reading to the collections
            //from top_left[0] to bottom_right[totalGSqs-1]
            red.push_back(tmpR);
            green.push_back(tmpG);
            blue.push_back(tmpB);
            bright.push_back(tmpL);
        }
    }
}

//returns vector{r,g,b,l} for r[1,nOfRows] and c[1,nOfCols]
//NOTE: NOT ZERO-BASED INDEX, THROWS pixelator_range_error
vector<int> Pixelator::getGridSquare(int c, int r){
    //if there's no grid to read yet return zero-values
    if(!initialised)return vector<int>{0,0,0,0};
    //check the arguments are in bounds...
    if(r<1||c<1||r>nOfRows||c>nOfColumns){
        cerr<<"getGridSquare out of bounds";
        throw pixelator_range_error(c+r);
    }
    vector<int> gs;
    //work out the position in the collection based on col and row position
    int index = ((r-1)*nOfColumns)+(c-1);
    gs.push_back(red.at(index));
    gs.push_back(green.at(index));
    gs.push_back(blue.at(index));
    gs.push_back(bright.at(index));
    return gs;
}

//x = columns, y = rows  for looping safely
ofPoint Pixelator::getMaxColsRows(){
    return ofPoint(nOfColumns,nOfRows);
}



void Pixelator::setDrawBoundsL(int low, int high){
    //ensure the args are ordered correctly?
    low = min(low,high);
    high = max(low,high);
    //are daisy chain conditionals ok?
    low = (low<1)?1:(low>255)?255:low;
    high = (high<1)?1:(high>255)?255:high;
    minDrawBoundsL = low;
    maxDrawBoundsL = high;
}

void Pixelator::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    if(is_drawing){
        //setting the colour tints everything so set to white after drawing
        //ofNoFill();
        for(int r=0;r<nOfRows;r++){
            //goes through each grid row
            for(int c=0; c<nOfColumns;c++){
                vector<int> tgs = getGridSquare(c+1,r+1);
                if(tgs.at(3)<minDrawBoundsL)ofSetColor(0,0,0,drawAlpha);
                else if(tgs.at(3)>maxDrawBoundsL)ofSetColor(255,255,255,drawAlpha);
                else ofSetColor(tgs.at(0),tgs.at(1),tgs.at(2),drawAlpha);
                ofPushMatrix();
                ofTranslate(c*colPixelsCnt,r*rowPixelsCnt);
                //cout<<"x: "<<c*colPixelsCnt<<"\t y: "<<r*rowPixelsCnt<<"\n";
                ofDrawRectangle(0,0,colPixelsCnt,rowPixelsCnt);
                //ofRectangle(0,0,colPixelsCnt,rowPixelsCnt);
                ofPopMatrix();
            }
        }
        //clear up the rendering colour after ourselves...
        ofSetColor(255);
        //is_drawing = false;
    }
}
