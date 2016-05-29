/******************************************************
 * Filename: tools.h								  *
 * Description: Tool functions such as cos or sin.	  *
 * TO BE REPLACED!									  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef _TOOLS_H__
#define _TOOLS_H__

#define M_PI 3.1415926
#define NORMFACTOR_FLOAT_31BIT 2147483648.0
#define INV_NORMFACTOR_FLOAT_31BIT 4.656612873e-10

typedef struct
{
    float incAngle;
    float angle;
    float ampl;
} trigonometricControl;

inline int floatToInt24(float a)
{
    int out = ((int)(a*NORMFACTOR_FLOAT_31BIT));
    return(out>>8);
}

extern inline float Int24ToFloat(int a)
{
    float out = ((float)(a<<8))*INV_NORMFACTOR_FLOAT_31BIT;
    return(out);
}

extern inline void calcCosine(float* output, trigonometricControl* ctrl);
extern inline void calcSine(float* output, trigonometricControl* ctrl);
extern inline void prepareTrigonometric(trigonometricControl* ctrl, float freq, float freqFs,
                                            float ampl, float phase);
extern inline void postProcessTrigonometric(trigonometricControl* ctrl);

#endif
