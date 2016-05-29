/******************************************************
 * Filename: algorithmIND_DSP.h						  *
 * Description: Base class to realize algorithm 	  *
 *              components. The most easy access to   *
 * 				new algorithms is to derive from this *
 * 				class.								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef _CRTPALGORITHM_H__
#define _CRTPALGORITHM_H__

#include "IrtpAlgorithm.h"

/*==================================================
 * Base class definitions for algorithm components
 *///===============================================
class CrtpAlgorithm: public IrtpAlgorithm
{
public:

    // Constructor to initialize all variables/fields
    CrtpAlgorithm();

   
    // ======== INHERITED FROM IRTPALGORITHM================
	
    // Return the current processing state
    virtual void getStateAlgorithm(RTP_STATE* stat);
    
    // Select the algorithm
    virtual bool selectAlgorithm(IrtpObject* hostRef, void* privData = 0, int szField = 0);

    // Activate the algorithm
    virtual bool activateAlgorithm(IrtpObject* hostRef);
        
	//! Return the number of setups that the plugin can handle, -1 means: try!
	virtual bool getNumberSetupsAlgorithm(int* numSetups);
	
	//! Return the setup for index, -1 means: Any value excepted
	virtual bool getSetupAlgorithm(int id, int* samplerate, 
		int* buffersize, int* numberChannelsInput, int* numberChannelsOutput, char nmSetup[STRING_LENGTH]);
		
	//! Return the number of processing formats that are supported
	virtual bool getNumberFormatProcessingAlgorithm(int* form);

	//! Return the format for an index
	virtual bool getFormatProcessingAlgorithm(int id, RTP_DATAFORMAT* format);

    // Check whether algorithm is ready for processing
    virtual bool isReadyForProcessingAlgorithm();

    // Prepare for processing
    virtual bool prepareProcessingAlgorithm();

    // Start processing
    virtual bool startProcessingAlgorithm();

    // Stop processing, additional timeout to complete interrupts
    virtual bool stopProcessingAlgorithm(unsigned int timeOutCycles);

    // Main processing routine
	virtual bool process(DATATYPE** outputBuffers, DATATYPE** inputBuffers, unsigned numberChannelsOut, unsigned numberChannelsIn, unsigned buffersize) = 0;
	
    // Post processing
    virtual bool postProcessingAlgorithm();

    // Deactivate algorithm
    virtual bool deactivateAlgorithm();
    
    // Opposite of select
	virtual bool unselectAlgorithm(void* fldSpecific = 0, int szFld = 0);
	
	// Function to return the latencies for a specific id, see plugin for details
	virtual bool queryLatencyAlgorithm(int* lat);
	
	 
    
	// ======== INHERITED FROM IRTPEXTENSIONS===============
    
    // Gateway function to get references to extended interfaces
	virtual bool requestInterface(void** returnedInterface, UWORD32 interfaceIdentifier);
	
	// If interface is not required anymore, return the used handle
	virtual bool returnInterface(void* returnedInterface, UWORD32 interfaceIdentifier);
	
	
    
	// ======== INHERITED FROM IRTPGENERICRUNTIME========================
	
    // Interface for GENERIC CONFIGURATION: Provide generic configure template
    virtual bool getGenericConfigureTemplate(rtpGenericConfigure_DSP** ptrTemplateOnReturn);

    // Interface for GENERIC CONFIGURATION: Process the generic configure template pointer
    virtual bool exchangeDataConfigure(rtpGenericConfigure_DSP* ptrConfigure);
    
    
    
    // ======== INHERITED FROM IRTPOBJECT===================
    
    // Return the name of the algorithm component
    virtual void getName(char name[STRING_LENGTH]);
    
    // Return the description of the algorithm component
    virtual void getDescription(char desc[STRING_LENGTH]);
        
    // Interface for ERROR MESSAGES: Return last error message
    virtual void getLastError(unsigned short* errCode, char errDescription[STRING_LENGTH]);

    // Return the specialization of the algorithm component
    virtual void getObjectSpecialization(void** link, RTP_OBJECT* type);
    
    // Interface for RUNTIME MESSAGES: Receive a runtime message
    virtual bool sendRuntimeMessage(runtimeMessage_DSP* specificData, unsigned short* errID);
    
protected:
	
    // Protected member variables
	char _nameAlgorithm[STRING_LENGTH];
	char _descriptionAlgorithm[STRING_LENGTH];
    RTP_STATE _stateAlgorithm;
    IrtpObject* _hostRef;
    IrtpObject* _runtimeMessageRef;
    RTP_ERROR _errorType;
    char _errorExpression[STRING_LENGTH];
    
    RTP_DATAFORMAT _dataFormat;
    
};


#endif // __CRTPALGORITHM_H__
