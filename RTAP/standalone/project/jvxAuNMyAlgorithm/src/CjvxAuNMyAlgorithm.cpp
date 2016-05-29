#include "jvx.h"
#include "CjvxAuNMyAlgorithm.h"

CjvxAuNMyAlgorithm::CjvxAuNMyAlgorithm(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass): 
	CjvxAudioNode(description, multipleObjects, module_name, descriptor, featureClass)
{
	_common_set.theComponentType = JVX_COMPONENT_AUDIO_NODE;
	_common_set.theObjectSpecialization = reinterpret_cast<jvxHandle*>(static_cast<IjvxNode*>(this));
	_common_set.thisisme = static_cast<IjvxObject*>(this);	
	_common_spec_proc_set.thisisme = static_cast<IjvxObject*>(this);
	
}

CjvxAuNMyAlgorithm::~CjvxAuNMyAlgorithm()
{
}

jvxErrorType 
CjvxAuNMyAlgorithm::activate()
{
	jvxErrorType res = CjvxAudioNode::activate();
	if(res == JVX_NO_ERROR)
	{
		genSimple_node::init__properties_active();
		genSimple_node::allocate__properties_active();
		genSimple_node::register__properties_active(static_cast<CjvxProperties*>(this));
	}
	return(res);
};

jvxErrorType 
CjvxAuNMyAlgorithm::deactivate()
{
	jvxErrorType res = CjvxAudioNode::deactivate();
	if(res == JVX_NO_ERROR)
	{
		genSimple_node::unregister__properties_active(static_cast<CjvxProperties*>(this));
		genSimple_node::deallocate__properties_active();
	}
	return(res);
};

jvxErrorType 
CjvxAuNMyAlgorithm::process_st(jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender)
{

	jvxErrorType res = JVX_NO_ERROR;

	// This default function does not tolerate a lot of unexpected settings
	assert(theData->sender_to_receiver.format == theData->receiver_to_sender.format);
	assert(theData->sender_to_receiver.buffersize == theData->receiver_to_sender.buffersize);
	
	res = process_buffer(
		theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[idx_sender_to_receiver],
		theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[idx_receiver_to_sender],
		theData->sender_to_receiver.buffersize,
		theData->sender_to_receiver.number_channels,
		theData->receiver_to_sender.number_channels,
		theData->sender_to_receiver.format);

	return(res);
};

jvxErrorType 
CjvxAuNMyAlgorithm::process_buffer(jvxHandle** buffers_input, jvxHandle** buffers_output, jvxSize bSize,
		jvxSize numChans_input,jvxSize numChans_output,jvxDataFormat format)
{
	jvxSize ii, j;

	jvxData* ptrInDbl = NULL, *ptrOutDbl = NULL;
	jvxData* ptrInFlt = NULL, *ptrOutFlt = NULL;
	jvxInt64* ptrInInt64 = NULL, *ptrOutInt64 = NULL;
	jvxInt32* ptrInInt32 = NULL, *ptrOutInt32 = NULL;
	jvxInt16* ptrInInt16 = NULL, *ptrOutInt16 = NULL;
	jvxInt8* ptrInInt8 = NULL, *ptrOutInt8 = NULL;
	jvxData tmp;
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize maxChans = JVX_MAX(numChans_input, numChans_output);

	//jvxData volume = genSimple_node::properties_active.volume.value;
	jvxData volume = 1.0;
	jvxData ANGLE = genSimple_node::properties_active.ANGLE.value;;
	jvxData CFAC = genSimple_node::properties_active.CFAC.value;
	jvxData gainLeft = 0.0;
	jvxData gainRight = 0.0;

	volume = sin(ANGLE/180*M_PI);
	gainLeft   = 1+volume*CFAC;
	gainRight  = 1-volume*CFAC;
	gainLeft  /= (1+CFAC);
	gainRight /= (1+CFAC);

	if(
		(numChans_input > 0) && 
		(numChans_output > 0))
	{
		for(ii = 0; ii < maxChans; ii++)
		{
			jvxSize idxIn = ii % numChans_input;
			jvxSize idxOut = ii % numChans_output;

			if (idxOut %2 == 0)
			{
				//even channel is left channel
				volume = gainLeft;
			}
			else
			{
				volume = gainRight;
			}

			switch(format)
			{
			case JVX_DATAFORMAT_DATA:
				ptrInDbl = (jvxData*)buffers_input[idxIn];
				ptrOutDbl = (jvxData*)buffers_output[idxOut];
				for(j = 0; j < bSize; j++)
				{
					tmp = ptrInDbl[j];
					tmp *= volume;
					ptrOutDbl[j] = tmp;
				}
				break;

			case JVX_DATAFORMAT_64BIT_LE:
				ptrInInt64 = (jvxInt64*)buffers_input[idxIn];
				ptrOutInt64 = (jvxInt64*)buffers_output[idxOut];
				for(j = 0; j < bSize; j++)
				{
					tmp = ptrInInt64[j];
					tmp *= volume;
					ptrOutInt64[j] = (jvxInt64)tmp;
				}
				break;

			case JVX_DATAFORMAT_32BIT_LE:
			case JVX_DATAFORMAT_24BIT_LE:
				ptrInInt32 = (jvxInt32*)buffers_input[idxIn];
				ptrOutInt32 = (jvxInt32*)buffers_output[idxOut];
				for(j = 0; j < bSize; j++)
				{
					tmp = ptrInInt32[j];
					tmp *= volume;
					ptrOutInt32[j] = (jvxInt32)tmp;
				}
				break;

			case JVX_DATAFORMAT_16BIT_LE:
				ptrInInt16 = (jvxInt16*)buffers_input[idxIn];
				ptrOutInt16 = (jvxInt16*)buffers_output[idxOut];
				for(j = 0; j < bSize; j++)
				{
					tmp = ptrInInt16[j];
					tmp *= volume;
					ptrOutInt16[j] = (jvxInt16)tmp;
				}
				break;
			case JVX_DATAFORMAT_8BIT:
				ptrInInt8 = (jvxInt8*)buffers_input[idxIn];
				ptrOutInt8 = (jvxInt8*)buffers_output[idxOut];
				for(j = 0; j < bSize; j++)
				{
					tmp = ptrInInt8[j];
					tmp *= volume;
					ptrOutInt8[j] = (jvxInt16)tmp;
				}
				break;
			default: 
				assert(0);
			}
		}
	}
	return(res);
}
