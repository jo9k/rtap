/******************************************************
 * Filename: tools.c								  *
 * Description: Some mathematic functions which are   *
 *				useful to produce sinusoidal outputs. *								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 #include <math.h>
#include "tools.h"

/*==================================================
 * Calculate cosine
 *///===============================================
inline void calcCosine(float* output, trigonometricControl* ctrl)
{
    ctrl->angle += ctrl->incAngle;
    *output = ctrl->ampl*cosf(ctrl->angle);
}

/*==================================================
 * Calculate sine
 *///===============================================
inline void calcSine(float* output, trigonometricControl* ctrl)
{
    ctrl->angle += ctrl->incAngle;
    *output = ctrl->ampl*sinf(ctrl->angle);
}

/*==================================================
 * Prepare the sinusoidal
 *///===============================================
inline void prepareTrigonometric(trigonometricControl* ctrl, float freq, float freqFs,
                                 float ampl, float phase)
{
    ctrl->incAngle = 2*M_PI*freq/freqFs;
    ctrl->angle = phase;
    ctrl->ampl = ampl;
}

inline void postProcessTrigonometric(trigonometricControl* ctrl)
{
    while (ctrl->angle > 2*M_PI)
        ctrl->angle -= 2*M_PI;
}
