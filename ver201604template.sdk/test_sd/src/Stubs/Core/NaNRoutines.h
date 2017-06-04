#ifndef __NANROUTINES_H__
#define __NANROUTINES_H__



class cNaNCheck
{
public:
    static bool   IsNAN(float x)        {return false;}         // Returns true or false to detect NaNs.
    static float  NoNAN(float x)        {return x;}             // Returns x, or +/- max floating pt if x is a NaN
    static double NoDblNAN(double x)    {return x;}             // Returns x or +/- max floating pt if x is a NaN.
};



#endif  // __NANROUTINES_H__

