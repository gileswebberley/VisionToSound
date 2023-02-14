#ifndef KELVINCONVERTER_H
#define KELVINCONVERTER_H
#include "ofMain.h"
/*
 *just a quick implementation of the really useful
 * work done by Tanner Helland to convert from
 * degK to an rbg colour value
*/

class KelvinConverter
{
    //they only need to exist once so make them static class members
    static constexpr float lower_bounds{1250.f};
    static constexpr float upper_bounds{15000.f};
    static constexpr float algorithm_limit{66.f};
    static constexpr double r_multiplier{329.698727446};
    static constexpr double r_exp{-0.1332047592};
    static constexpr double g_multiplier_low{99.4708025861};
    static constexpr double g_multiplier_high{288.1221695283};
    static constexpr double g_minus_low{161.1195681661};
    static constexpr double g_exp_high{-0.0755148492};
    static constexpr double b_multiplier{138.5177312231};
    static constexpr double b_minus{305.0447927307};
    static constexpr float b_low_limit{19.f};
    //double tempK{1500.f};

public:
    KelvinConverter();
    ofColor static getRGBCorrectionFromKelvin(const float k_colour);
    ofColor static getRBGFilterFromKelvin(const float k_colour);
    //just check the rgb values are in [0..255]
    float static rgbBoundsCheck(float check_val);
};

#endif // KELVINCONVERTER_H
