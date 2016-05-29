#ifndef __JVX_GESTURESENSE_PRV__H__
#define __JVX_GESTURESENSE_PRV__H__

#include "jvx_gestureSense/jvx_gestureSense.h"

// RAM
typedef struct {
     jvxData **buf; // received signal buffers
     jvxData *sig_tx; // measurement signal
     jvxDataCplx *sig_tx_FFT; // FFT of measurement signal
     jvxByte tx_on; // 1 = tx on, 0 = tx off
     jvxDataCplx **rx_FFT; // FFT of received signals
     jvxDataCplx **xcorr_FFT; // FFT of cross correlation signals
     jvxData **xcorr; // cross correlation signals
     jvxData **compensation; // compensation signals
     jvxData pos[3]; // estimated position
} jvx_gestureSense_ram;

// State data (i.e. delay line memory).
typedef struct {
     jvxData dummy;
} jvx_gestureSense_state;

// Parameters, which are derived from configuration but not set directly.
typedef struct  {
     // depending on synced prms
     jvxData dummy_1;

     // not depending on synced prms
     jvxData dummy_2;
} jvx_gestureSense_cfg_derived;


// Prvate data struct.
typedef struct
{
     jvx_gestureSense_ram ram;
     jvx_gestureSense_state state;
     jvx_gestureSense_cfg_derived derived;
     jvx_gestureSense_prmSync prmSync;
     jvx_gestureSense_prm prm;

     // submodules
     jvxHandle **FFT_hdl;
     jvxHandle **IFFT_hdl;
     jvxHandle *FFT_global_hdl;

} jvx_gestureSense_prv;


#endif
