/******************************************************
 * Filename: algorithmIND_DSP.c						  *
 * Description: Base class to realize algorithm 	  *
 *              components. The most easy access to   *
 * 				new algorithms is to derive from this *
 * 				class.								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include <string.h>
#include "CrtpAlgorithm.h"
#include "memory.h"

// Delay function from accessSPORT.c
extern void Delay (int i) ;

/*==================================================
 * Constructor: Initialize all static data
 *///===============================================
CrtpAlgorithm::CrtpAlgorithm()
{
    memcpy(_nameAlgorithm, "ALGO DSP: TALKTHROUGH", STRING_LENGTH);
    memcpy(_descriptionAlgorithm, "FANCY FUNCTION", STRING_LENGTH);
    _stateAlgorithm = RTP_STATE_INIT;
    _errorType = RTP_ERROR_NOERROR;
    
	_runtimeMessageRef = NULL;
	
	#ifdef RTP_CHANNELFORMAT_FLOAT
		_dataFormat = RTP_DATAFORMAT_FLOAT;
		#else
		#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT
			_dataFormat = RTP_DATAFORMAT_24BIT_LE;
			#else
			#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
				_dataFormat = RTP_DATAFORMAT_16BIT_LE;
				#else
				_dataFormat = RTP_DATAFORMAT_NONE;
			#endif
		#endif
	#endif
}

/*==================================================
 * Return the state of the algorithm internally. this may be used later.
 *///===============================================
void
CrtpAlgorithm::getStateAlgorithm(RTP_STATE* stat)
{
    if (stat)
        *stat = _stateAlgorithm;
}


/**
 * Function to select the current algorithm, be aware of wrong state.
 * In selected state everything should be prepared for activation.
 *///=================================================================
bool
CrtpAlgorithm::selectAlgorithm(IrtpObject* hostRef, void* privData, int szFld)
{
	if(_stateAlgorithm == RTP_STATE_INIT)
	{
		_stateAlgorithm = RTP_STATE_SELECTED;
		_hostRef = hostRef;
		_runtimeMessageRef = NULL;

		if(_hostRef)
		{
			this->requestInterface(reinterpret_cast<void**>(&_runtimeMessageRef), RTP_INTERFACE_RUNTIMEMESSAGING);
		}

		
		char* txt = new char[STRING_LENGTH];
		txt = _nameAlgorithm;
		
		runtimeMessage_DSP* message = new runtimeMessage_DSP;
		message->header.idMessageType = 0;
        message->header.lengthMessage = STRING_LENGTH;
		message->fieldStart = *txt;
		
		this->sendRuntimeMessage(message, 0);
		
		return(true);
	}
	memcpy(_errorExpression, "ALGORITHM, BASE: Wrong state for current command (need to be in state RTP_STATE_INIT).", STRING_LENGTH);
	this->_errorType = RTP_ERROR_WRONGSTATEFORCALL;
	return(false);
}

/*==================================================
 * Activate algorithm. All dynamic memory that does not depend on the
 * processing parameters should be allocated here.
 *///===============================================
bool
CrtpAlgorithm::activateAlgorithm(IrtpObject* hostRef)
{
    _hostRef = hostRef;
    _stateAlgorithm = RTP_STATE_ACTIVE;
    return(true);
}

/** 
 *	Return the number of supported setups. In this default case, return the length of
 *  the setup list which is a protected member variable.
 *============================================================*/
bool
CrtpAlgorithm::getNumberSetupsAlgorithm(int* numSetups)
{
	if(_stateAlgorithm != RTP_STATE_ACTIVE)
	{
		memcpy(_errorExpression, "ALGORITHM, BASE: Wrong state for current command (need to be in state RTP_STATE_ACTIVE).", STRING_LENGTH);
		this->_errorType = RTP_ERROR_WRONGSTATEFORCALL;
		return(false);
	}

	// Any setup accepted
	if(numSetups)
		*numSetups = 1;
	return(true);
}


/**
 * Function to return a specific setup. The text field must be copied
 * into a text buffer specified, -1 indicated don't care fields.
 *///============================================================
bool 
CrtpAlgorithm::getSetupAlgorithm(int id, int* samplerate, int* buffersize, 
								 int* numberChansIn, int* numberChansOut, char nmSetup[STRING_LENGTH])
{
	if(_stateAlgorithm != RTP_STATE_ACTIVE)
	{
		memcpy(_errorExpression, "ALGORITHM, BASE: Wrong state for current command (need to be in state RTP_STATE_ACTIVE).", STRING_LENGTH);
		this->_errorType = RTP_ERROR_WRONGSTATEFORCALL;
		return(false);
	}
	
	if(samplerate)
		*samplerate = SAMPLERATE;
	if(buffersize)
		*buffersize = BUFFERSIZE;
	if(numberChansIn)
		*numberChansIn = NUMBER_CHANNELS_INPUT;
	if(numberChansOut)
		*numberChansOut = NUMBER_CHANNELS_OUTPUT;
	return(true);
}


/**
 * Return the number of formats which are supported by the algoritm component.
 *///============================================================
bool 
CrtpAlgorithm::getNumberFormatProcessingAlgorithm(int* numForm)
{
	if(_stateAlgorithm != RTP_STATE_ACTIVE)
	{
		memcpy(_errorExpression, "ALGORITHM, BASE: Wrong state for current command (need to be in state RTP_STATE_ACTIVE).", STRING_LENGTH);
		this->_errorType = RTP_ERROR_WRONGSTATEFORCALL;
		return(false);
	}
	if(numForm)
		*numForm = 1;
	return(true);
}

/**
 * For a specified id, return the corresponding processing format.
 *///============================================================
bool
CrtpAlgorithm::getFormatProcessingAlgorithm(int id, RTP_DATAFORMAT* format)
{
	if(_stateAlgorithm != RTP_STATE_ACTIVE)
	{
		memcpy(_errorExpression, "ALGORITHM, BASE: Wrong state for current command (need to be in state RTP_STATE_ACTIVE).", STRING_LENGTH);
		this->_errorType = RTP_ERROR_WRONGSTATEFORCALL;
		return(false);
	}
	
	if(format)
	{
		*format = _dataFormat;
		return(true);
	}

	memcpy(_errorExpression, "ALGORITHM, BASE: Wrong state for current command (need to be in state RTP_STATE_PROCESSING).", STRING_LENGTH);
	this->_errorType = RTP_ERROR_WRONGSTATEFORCALL;
	return(false);
}

/*==================================================
 * Function to indicate host wether algo is readfy for processing or not.
 *///===============================================
bool
CrtpAlgorithm::isReadyForProcessingAlgorithm()
{
    return(true);
}

/*==================================================
 * Prepare processing. All dynamic memory that depends on the
 * processing parameters should be allocated here.
 * All processing parameters are known as preprocessor macros,
 * BUFFERSIZE, SAMPLERATE and the number of channels here.
 *///===============================================
bool
CrtpAlgorithm::prepareProcessingAlgorithm()
{
    _stateAlgorithm = RTP_STATE_PREPARED;
    return(true);
}

/*==================================================
 * Start processing, from here on, process may be called for
 * audio processing.
 *///===============================================
bool
CrtpAlgorithm::startProcessingAlgorithm()
{
    _stateAlgorithm = RTP_STATE_PROCESSING;
    return(true);
}

/*==================================================
 * Stop processing. From here on, no more functiocalls to process 
 * will occur.
 *///===============================================
bool
CrtpAlgorithm::stopProcessingAlgorithm(unsigned int timeOutCycles)
{
    Delay(timeOutCycles);
    _stateAlgorithm = RTP_STATE_PREPARED;
    return(true);
}

/*==================================================
 * Data processing callback. All processing parameters are
 * available as MACROs:
 * - BUFFERSIZE: size of audio buffers, non-overlapping
 * - SAMPLERATE: samplerate of ADC and DAC
 * - NUMBER_CHANNELS_OUTPUT: Number of output audio channels
 * - NUMBER_CHANNELS_INPUT: Number of input audio channels
 *///===============================================
/*
bool
CrtpAlgorithm::process(DATATYPE outputBuffers[NUMBER_CHANNELS_OUTPUT][BUFFERSIZE], DATATYPE inputBuffers[NUMBER_CHANNELS_INPUT][BUFFERSIZE])
{
	// Here is the right place to put your algorithm,
	// the task is to transfer input buffers into output buffers
	
	// In this template, the talkthrough is realized,
	// all datatformats are supported (16,24 bit and float).
#ifdef RTP_CHANNELFORMAT_FLOAT
	float * inputPtr;
    float * outputPtr;

    // Copy input samples to output buffers.
    // On all outputs, the same signal will be audible
   	inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[0];
	memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[1];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);

#ifdef CHANNELSET2
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[2];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[3];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#endif

#ifdef CHANNELSET3
#ifdef CHANNELSET2
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[4];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[5];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#else
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[2];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[3];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#endif
#endif
#endif // end of float

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT 
    unsigned int * inputPtr;
    unsigned int * outputPtr;

    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[0];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[1];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    
#ifdef CHANNELSET2
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[2];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[3];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#endif

#ifdef CHANNELSET3
#ifdef CHANNELSET2
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[4];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[5];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#else
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[2];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[3];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#endif
#endif
#endif // end of INT24

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
    unsigned int * inputPtr;
    unsigned int * outputPtr;

    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[0];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[1];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    
#ifdef CHANNELSET2
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[2];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[3];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#endif

#ifdef CHANNELSET3
#ifdef CHANNELSET2
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[4];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[5];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#else
    inputPtr = inputBuffers[0];
    outputPtr = outputBuffers[2];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
    inputPtr = inputBuffers[1];
    outputPtr = outputBuffers[3];
    memcpy(outputPtr, inputPtr, BUFFERSIZE);
#endif
#endif
#endif // end of INT16
    return(true);
}
*/

/*==================================================
 * Post processing, this function is the inverse of the 
 * startProcessing function. All dynamic memory allocations
 * should fit to the inverse function.
 *///===============================================
bool
CrtpAlgorithm::postProcessingAlgorithm()
{
    _stateAlgorithm = RTP_STATE_ACTIVE;
    return(true);
}

/*==================================================
 * Post processing, this function is the inverse of the 
 * activateAlgorithm function. All dynamic memory allocations
 * should fit to the inverse function.
 *///===============================================
bool
CrtpAlgorithm::deactivateAlgorithm()
{
    _stateAlgorithm = RTP_STATE_INIT;
    return(true);
}

/**
 * Unselect the algorithm. The unselection will always set the algorithm into the initial
 * state no matter in which state the component currently is.
 *///===========================================================================
bool
CrtpAlgorithm::unselectAlgorithm(void* fldSpecific, int szFld)
{
	if(_stateAlgorithm == RTP_STATE_PROCESSING)
		this->stopProcessingAlgorithm(500);

	if(_stateAlgorithm == RTP_STATE_PREPARED)
		this->postProcessingAlgorithm();

	if(_stateAlgorithm == RTP_STATE_ACTIVE)
		this->deactivateAlgorithm();

	if(_hostRef)
	{
		if(_runtimeMessageRef)
		{
			this->returnInterface(_runtimeMessageRef, RTP_INTERFACE_RUNTIMEMESSAGING);
		}
	}

	_runtimeMessageRef = NULL;
	_hostRef = NULL;

	_stateAlgorithm = RTP_STATE_INIT;
	return(true);
}


/**
 * Function to return the latency of the current algorithm.
 * Talkthrough has zero latency.
 *///=======================================================
bool 
CrtpAlgorithm::queryLatencyAlgorithm(int* lat)
{
	if(_stateAlgorithm != RTP_STATE_PREPARED)
	{
		memcpy(_errorExpression, "ALGORITHM, BASE: Wrong state for current command (need to be in state RTP_STATE_PREPARED).", STRING_LENGTH);
	this->_errorType = RTP_ERROR_WRONGSTATEFORCALL;
	return(false);
	}
	if(lat)
		*lat = 0;
	return(true);
}


bool 
CrtpAlgorithm::requestInterface(void** returnedInterface, UWORD32 interfaceIdentifier)
{
	if(returnedInterface)
	{
		RTP_INTERFACE idIf = (RTP_INTERFACE)interfaceIdentifier;
		switch(idIf)
		{
		
		case RTP_INTERFACE_RUNTIMEMESSAGING:
			*returnedInterface = static_cast<IrtpObject*>(this);
			return(true);
		default:
			return(false);
		}
	}
	return(false);
}

bool 
CrtpAlgorithm::returnInterface(void* returnedInterface, UWORD32 interfaceIdentifier)
{
	RTP_INTERFACE idIf = (RTP_INTERFACE)interfaceIdentifier;
	switch(idIf)
	{
	
		case RTP_INTERFACE_RUNTIMEMESSAGING:
			if(returnedInterface == static_cast<IrtpObject*>(this))
			{
				return(true);
			}
		default:
			return(false);
	}
}


/*==================================================
 * Entry function to return the generic configuration.
 * Generic configuration is intended for future functionality.
 *///===============================================
bool
CrtpAlgorithm::getGenericConfigureTemplate(rtpGenericConfigure_DSP** ptrTemplateOnReturn)
{
    if (ptrTemplateOnReturn)
        *ptrTemplateOnReturn = 0;
    return(true);
}

/*==================================================
 * Entry function to return data related to the generic configuration.
 * Generic configuration is intended for future functionality.
 *///===============================================
bool
CrtpAlgorithm::exchangeDataConfigure(rtpGenericConfigure_DSP* ptrConfigure)
{
    if (ptrConfigure == 0)
    {
        return(true);
    }
    else
    {
        _errorType = RTP_ERROR_IDMISMATCH;
        memcpy(_errorExpression, "Generic pointer not zero", STRING_LENGTH);
    }
    return(false);
}




/*==================================================
 * Return the name of the algorithm. This may be used later
 *///===============================================
void
CrtpAlgorithm::getName(char name[STRING_LENGTH])
{
    memcpy(name, _nameAlgorithm, STRING_LENGTH);
}

/*==================================================
 * Return the description of the algorithm. This may be used later
 *///===============================================
void
CrtpAlgorithm::getDescription(char desc[STRING_LENGTH])
{
    memcpy(desc, _descriptionAlgorithm, STRING_LENGTH);
}


/*==================================================
 * Error message handling. If there was an error, 
 * this function returns the error code and a short description.
 *///===============================================
void
CrtpAlgorithm::getLastError(unsigned short* errCode, char errDescription[STRING_LENGTH])
{
    if (errCode)
        *errCode = _errorType;
    memcpy(errDescription, _errorExpression, STRING_LENGTH);
}


/*==================================================
 * Return the specialization of the algorithm. This may be used later
 *///===============================================
void CrtpAlgorithm::getObjectSpecialization(void** link, RTP_OBJECT* type)
{
	if(link&&type)
	{
		*type = RTP_OBJECT_ALGORITHM;
		*link = static_cast<IrtpAlgorithm*>(this);
	}
}

/*==================================================
 * The algorithm is the destination of a runtime message which arrives
 * here.
 *///===============================================
bool
CrtpAlgorithm::sendRuntimeMessage(runtimeMessage_DSP* specificData, unsigned short* errID)
{
    if (errID)
    {
        *errID = RTP_ERROR_IDMISMATCH;
    }
    return(false);
}

