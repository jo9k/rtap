#ifndef __JVX_CIRCBUFFER_H__
#define __JVX_CIRCBUFFER_H__

// Need to define the internally used data for inline functions
#include "jvx_dsp_base.h"
#include "jvx_dataformats.h"

JVX_DSP_LIB_BEGIN

/**
 * Circular buffer structure used in circular buffer realizations
 *///==============================================================
typedef struct
{
	jvxInt32 length;
	jvxInt16 channels;
	jvxSize idxRead;
	jvxInt32 fHeight;
	jvxInt32 nSections; // for SOS IIR filters
	jvxDataFormat format;
	size_t szElement;
	struct
	{
		jvxData** field;
	} ram;
} jvx_circbuffer;

// ===============================================================================
// Multi channel circbuffers
// ===============================================================================

jvxDspBaseErrorType jvx_circbuffer_allocate(jvx_circbuffer** hdlOnReturn,
					    jvxInt32 numberElements,
					    jvxInt32 nSections,
					    jvxInt16 channels);

jvxDspBaseErrorType jvx_circbuffer_deallocate(jvx_circbuffer* hdlReturn);

jvxDspBaseErrorType jvx_circbuffer_fill(jvx_circbuffer* hdl,
					jvxData toFillWith,
					jvxInt32 numberValuesFill);

jvxDspBaseErrorType jvx_circbuffer_remove(jvx_circbuffer* hdl,
					  jvxInt32 numberValuesRemove);

jvxDspBaseErrorType jvx_circbuffer_write_update(jvx_circbuffer* hdl,
						jvxData** fieldFill,
						jvxInt32 numberValuesFill);

jvxDspBaseErrorType jvx_circbuffer_read_update(jvx_circbuffer* hdl,
					       jvxData** fieldRead,
					       jvxInt32 numberValuesRead);

jvxDspBaseErrorType jvx_circbuffer_copy_update_buf2buf(jvx_circbuffer* hdl_copyTo,
						       jvx_circbuffer* hdl_copyFrom,
						       jvxInt32 numberValuesFill);

jvxDspBaseErrorType jvx_circbuffer_write_update_ignore(jvx_circbuffer* hdl,
						       jvxData** fieldFill,
						       jvxInt32 numberValuesFill);

jvxDspBaseErrorType jvx_circbuffer_read_update_ignore(jvx_circbuffer* hdl,
						      jvxData** fieldRead,
						      jvxInt32 numberValuesRead);

jvxDspBaseErrorType jvx_circbuffer_read_noupdate(jvx_circbuffer* hdl,
						 jvxData** fieldRead,
						 jvxInt32 numberValuesRead);

jvxDspBaseErrorType jvx_circbuffer_fir_1can_1io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData** fieldInOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_fir_2can_1io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData** fieldInOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_1can_1io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData* fCoeffs_bw,
						jvxData** fieldInOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_2can_1io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData* fCoeffs_bw,
						jvxData** fieldInOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_1can_1io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData* fCoeffs_bw,
						jvxData** fieldInOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_sos1can_1io(jvx_circbuffer* hdlIn,
						   jvxData* fCoeffs_fw,
						   jvxData* fCoeffs_bw,
						   jvxData* gains,
						   jvxInt32 shiftFactor,
						   jvxData** fieldInOut,
						   jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_sos2can_1io(jvx_circbuffer* hdlIn,
						   jvxData* fCoeffs_fw,
						   jvxData* fCoeffs_bw,
						   jvxData* gains,
						   jvxInt32 shiftFactor,
						   jvxData** fieldInOut,
						   jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_fir_1can_2io(jvx_circbuffer* hdl,
						jvxData* fCoeffs_fw,
						jvxData** fieldIn,
						jvxData** fieldOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_fir_2can_2io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData** fieldIn,
						jvxData** fieldOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_1can_2io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData* fCoeffs_bw,
						jvxData** fieldIn,
						jvxData** fieldOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_2can_2io(jvx_circbuffer* hdlIn,
						jvxData* fCoeffs_fw,
						jvxData* fCoeffs_bw,
						jvxData** fieldIn,
						jvxData** fieldOut,
						jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_sos1can_2io(jvx_circbuffer* hdlIn,
						   jvxData* fCoeffs_fw,
						   jvxData* fCoeffs_bw,
						   jvxData* gains,
						   jvxInt32 shiftFactor,
						   jvxData** fieldIn,
						   jvxData** fieldOut,
						   jvxInt32 bSize);

jvxDspBaseErrorType jvx_circbuffer_iir_sos2can_2io(jvx_circbuffer* hdlIn,
						   jvxData* fCoeffs_fw,
						   jvxData* fCoeffs_bw,
						   jvxData* gains,
						   jvxInt32 shiftFactor,
						   jvxData** fieldIn,
						   jvxData** fieldOut,
						   jvxInt32 bSize);

// ===============================================================================
// Single channel circbuffers
// ===============================================================================

jvxDspBaseErrorType jvx_circbuffer_allocate_1chan(jvx_circbuffer** hdlOnReturn,
						  jvxInt32 numberElements);

jvxDspBaseErrorType jvx_circbuffer_write_update_1chan(jvx_circbuffer* hdl,
						      jvxData* fieldFill,
						      jvxInt32 numberValuesFill);

jvxDspBaseErrorType jvx_circbuffer_read_update_1chan(jvx_circbuffer* hdl,
						     jvxData* fieldRead,
						     jvxInt32 numberValuesRead);

jvxDspBaseErrorType jvx_circbuffer_read_noupdate_1chan(jvx_circbuffer* hdl,
						       jvxData* fieldRead,
						       jvxInt32 numberValuesRead);

jvxDspBaseErrorType jvx_circbuffer_write_convolve_update_1chan(jvx_circbuffer* hdl,
							       jvxData input,
							       jvxData* coeffs,
							       jvxSize lengthCoeffBuffer,
							       jvxData* output);

jvxDspBaseErrorType jvx_circbuffer_write_convolve_no_update_1chan(jvx_circbuffer* hdl,
								  jvxData input,
								  jvxData* coeffs,
								  jvxSize lengthCoeffBuffer,
								  jvxData* output);

jvxDspBaseErrorType jvx_circbuffer_write_convolve_only_update_1chan(jvx_circbuffer* hdl);

jvxDspBaseErrorType jvx_circbuffer_write_norm_update_1chan(jvx_circbuffer* hdl,
							   jvxData input,
							   jvxData* output);

jvxDspBaseErrorType jvx_circbuffer_write_norm_no_update_1chan(jvx_circbuffer* hdl,
							      jvxData input,
							      jvxData* output);

jvxDspBaseErrorType jvx_circbuffer_write_norm_only_update_1chan(jvx_circbuffer* hdl);

jvxDspBaseErrorType jvx_circbuffer_fld_mult_fac_add_circbuffer_fac(jvxData* fldInOut,
								   jvxSize sz,
								   jvxData fac1,
								   jvxData fac2,
								   jvx_circbuffer* hdl);

jvxDspBaseErrorType jvx_circbuffer_fld_mult_fac_add_circbuffer_fac_ret_energy(jvxData* fldInOut,
									      jvxSize sz,
									      jvxData fac1,
									      jvxData fac2,
									      jvx_circbuffer* hdl,
									      jvxData* energyFilter);

/** Compute cross correlation function based on recursive smoothing */
jvxDspBaseErrorType
jvx_circbuffer_ccorr_1chan(jvx_circbuffer* hdl1,
			   jvx_circbuffer* hdl2,
			   jvxData* fieldUpdate,
			   jvxData* fieldRead1,
			   jvxData* fieldRead2,
			   int numValues,
			   jvxData smooth);

JVX_DSP_LIB_END

#endif
