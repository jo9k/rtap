#ifndef __JVX_DSP_BASE_RANGECHECK__H__
#define __JVX_DSP_BASE_RANGECHECK__H__

#include "jvx_dsp_base_types.h"

void jvx_rangeCheck_jvxData(jvxData *x,
                            jvxData min,
                            jvxData max,
                            const char *moduleName,
                            const char *prmName);

void jvx_rangeCheck_int(int *x,
                        int min,
                        int max,
                        const char *moduleName,
                        const char *prmName);

void jvx_rangeCheck_int16(jvxInt16 *x,
                          jvxInt16 min,
                          jvxInt16 max,
                          const char *moduleName,
                          const char *prmName);

void jvx_rangeCheck_jvxSize(jvxSize *x,
                            jvxSize min,
                            jvxSize max,
                            const char *moduleName,
                            const char *prmName);
#endif
