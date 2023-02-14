#include "kelvinconverter.h"

KelvinConverter::KelvinConverter()
{

}
float KelvinConverter::rgbBoundsCheck(float check_val){
    check_val = (check_val<0)?0:check_val;
    check_val = (check_val>255)?255:check_val;
    return check_val;
}

ofColor KelvinConverter::getRGBCorrectionFromKelvin(const float k_colour){
    //if out of algorithmic bounds..
    double tempK = k_colour;
    if(tempK < lower_bounds) tempK = lower_bounds;
    if(tempK > upper_bounds) tempK = upper_bounds;
    tempK /= 100;
    float kr,kg,kb;
    if(tempK <= algorithm_limit){
        //less than 66
        kr = 255;//red is full
        kg = tempK;
        double logG = log(kg);
        kg = (g_multiplier_low*logG)-g_minus_low;
        if(tempK<=b_low_limit){
            kb = 0;
        }else{
            kb = tempK - 10;
            double logB = log(kb);
            kb = (b_multiplier*logB)-b_minus;
        }
    }else{
        //greater than 66
        kb = 255;//blue is on full
        kg = tempK - 60;
        kg = g_multiplier_high * (pow(kg,g_exp_high));
        kr = tempK - 60;
        kr = r_multiplier*pow(kr,r_exp);
    }
    kr = rgbBoundsCheck(kr);
    kg = rgbBoundsCheck(kg);
    kb = rgbBoundsCheck(kb);
    return ofColor{kr,kg,kb};
}

ofColor KelvinConverter::getRBGFilterFromKelvin(const float k){
    ofColor tmpc{255};
    return tmpc - getRGBCorrectionFromKelvin(k);
}
