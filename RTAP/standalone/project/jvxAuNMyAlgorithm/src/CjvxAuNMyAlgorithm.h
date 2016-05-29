#ifndef __CJVXAUNMYALGORITHM_H__
#define __CJVXAUNMYALGORITHM_H__

#include "jvx.h"
#include "templates/jvxComponents/jvxAudioNodes/CjvxAudioNode.h"
#include "pcg_exports_node.h"

class CjvxAuNMyAlgorithm: public CjvxAudioNode, public genSimple_node
{
	
public:

	JVX_CALLINGCONVENTION CjvxAuNMyAlgorithm(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxAuNMyAlgorithm();
	
	virtual jvxErrorType JVX_CALLINGCONVENTION activate();
	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();

	virtual jvxErrorType JVX_CALLINGCONVENTION process_st(jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender);

private:

	virtual jvxErrorType JVX_CALLINGCONVENTION process_buffer( jvxHandle** buffers_input,jvxHandle** buffers_output,
		jvxSize bSize, jvxSize numChans_input, jvxSize numChans_output, jvxDataFormat format);

};

#endif
