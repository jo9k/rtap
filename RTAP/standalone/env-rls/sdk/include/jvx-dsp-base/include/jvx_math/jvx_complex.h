#ifndef __JVX_COMPLEX_H__
#define __JVX_COMPLEX_H__

#include "jvx_dsp_base_types.h"


//! complex argument
jvxData jvx_complex_arg(jvxData real_in, jvxData imag_in);


//! complex cartesian to polar conversion
void jvx_complex_cart_2_polar(jvxData real_in, jvxData imag_in, jvxData* abs_out, jvxData* angle_out);


//! complex polar to cartesian conversion
void jvx_complex_polar_2_cart(jvxData abs_in, jvxData angle_in, jvxData* real_out, jvxData* imag_out);

//! complex multiplication
void jvx_complex_multiply(jvxDataCplx in1, jvxDataCplx in2, jvxDataCplx* out);

#endif
