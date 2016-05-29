#include <iostream>
#include "templates/jvxComponents/jvxAudioNodes/CjvxRichAudioNode.h"

CjvxRichAudioNode::CjvxRichAudioNode(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass):
	CjvxAudioNode(description, multipleObjects, module_name, descriptor, featureClass)
{
     spectrumAnalyzer.active = false;
     beampatternSimulator.active = false;
     JVX_INITIALIZE_MUTEX(beampatternProcessMutex);
}
CjvxRichAudioNode::~CjvxRichAudioNode()
{
     JVX_TERMINATE_MUTEX(beampatternProcessMutex);
}

jvxErrorType
CjvxRichAudioNode::prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
{
     return CjvxAudioNode::prepare(theEndpoint, numEndpoints);
}

jvxErrorType
CjvxRichAudioNode::process_st(jvxLinkDataDescriptor* theData,
                              jvxSize idx_sender_to_receiver,
                              jvxSize idx_receiver_to_sender)
{
	jvxErrorType res = CjvxAudioNode::process_st(theData, idx_sender_to_receiver, idx_receiver_to_sender);
	if(res != JVX_NO_ERROR)
		return res;

	if(spectrumAnalyzer.props.params.spectrumEstimationSyncNotify.value
           || beampatternSimulator.props.params.beampatternSyncNotify.value)
		{
			updateInstrumentation(true);

			// this will call update_window in the GUI thread
			_report_property_has_changed(JVX_PROPERTY_CATEGORY_UNSPECIFIC, -1, true);

                        spectrumAnalyzer.props.params.spectrumEstimationSyncNotify.value = 0;
                        beampatternSimulator.props.params.beampatternSyncNotify.value = 0;
		}

	return JVX_NO_ERROR;
}

jvxErrorType
CjvxRichAudioNode::postprocess()
{
     jvxErrorType res = CjvxAudioNode::postprocess();
     if(res != JVX_NO_ERROR)
          return res;

     if(spectrumAnalyzer.active)
          if(jvx_spectrumEstimation_terminate(&spectrumAnalyzer.hdl) != JVX_DSP_NO_ERROR)
               return JVX_ERROR_INTERNAL;

     if(beampatternSimulator.active)
          if(jvx_beampattern_terminate(&beampatternSimulator.hdl) != JVX_DSP_NO_ERROR)
               return JVX_ERROR_INTERNAL;

     return JVX_NO_ERROR;
}



jvxErrorType
CjvxRichAudioNode::activate()
{
     jvxErrorType res = CjvxAudioNode::activate();

     if(res != JVX_NO_ERROR)
          return res;

     // NOTE: configInit is always called no matter if the instrument is created or not

     // config init of instrumentation
     if(jvx_spectrumEstimation_configInit(&spectrumAnalyzer.hdl) != JVX_DSP_NO_ERROR)
          return JVX_ERROR_INTERNAL;
     if(jvx_beampattern_configInit(&beampatternSimulator.hdl) != JVX_DSP_NO_ERROR)
          return JVX_ERROR_INTERNAL;

     // spectrum analyzer parameter properties
     spectrumAnalyzer.props.init__params();
     spectrumAnalyzer.props.allocate__params();
     spectrumAnalyzer.props.associate__params(static_cast<CjvxProperties*>(this),
                                              &spectrumAnalyzer.hdl.prmSync.method, 1,
                                              &spectrumAnalyzer.hdl.prmSync.logarithmic, 1,
                                              &spectrumAnalyzer.hdl.prmSync.alpha, 1,
                                              &spectrumAnalyzer.hdl.prmSync.fftLengthLog2, 1,
                                              &spectrumAnalyzer.hdl.prmSync.windowType, 1,
                                              &spectrumAnalyzer.hdl.prmSync.winArgA, 1,
                                              &spectrumAnalyzer.hdl.prmSync.winArgB, 1,
                                              &spectrumAnalyzer.hdl.debug.spectrumSize, 1,
                                              &spectrumAnalyzer.hdl.cnst.samplingRate, 1,
                                              &spectrumAnalyzer.hdl.cnst.nChannelsIn, 1);
     spectrumAnalyzer.props.register__params(static_cast<CjvxProperties*>(this));
     spectrumAnalyzer.props.params.spectrumEstimationEnabled.value = 1;

     // spectrum analyzer data properties (associate later if used)
     spectrumAnalyzer.props.init__data();
     spectrumAnalyzer.props.allocate__data();
     spectrumAnalyzer.props.register__data(static_cast<CjvxProperties*>(this));

     // beampattern simulator parameter properties
     beampatternSimulator.props.init__params();
     beampatternSimulator.props.allocate__params();
     beampatternSimulator.props.associate__params(static_cast<CjvxProperties*>(this),
                                                  &beampatternSimulator.hdl.cnst.samplingRate, 1,
                                                  &beampatternSimulator.hdl.debug.spectrumSize, 1,
                                                  &beampatternSimulator.hdl.prmSync.numAzimuthAngles, 1,
                                                  &beampatternSimulator.hdl.prmSync.azimuthMin, 1,
                                                  &beampatternSimulator.hdl.prmSync.azimuthMax, 1,
                                                  &beampatternSimulator.hdl.prmSync.fftLengthLog2, 1,
                                                  &beampatternSimulator.hdl.prmSync.fracDelayFilterDelay, 1);
     beampatternSimulator.props.register__params(static_cast<CjvxProperties*>(this));
     beampatternSimulator.props.params.beampatternEnabled.value = 1;

     // beampattern simulator data properties (associate later if used)
     beampatternSimulator.props.init__data();
     beampatternSimulator.props.allocate__data();
     beampatternSimulator.props.register__data(static_cast<CjvxProperties*>(this));

     return res;
}

jvxErrorType
CjvxRichAudioNode::deactivate()
{
     jvxErrorType res = CjvxAudioNode::deactivate();

     if(res != JVX_NO_ERROR)
          return res;

     // NOTE: always deinitialize instrument properties no matter if the instrument is created or not

     // spectrum analyzer parameter properties
     spectrumAnalyzer.props.unregister__params(static_cast<CjvxProperties*>(this));
     spectrumAnalyzer.props.deassociate__params(static_cast<CjvxProperties*>(this));
     spectrumAnalyzer.props.deallocate__params();

     // spectrum analyzer data properties
     spectrumAnalyzer.props.unregister__data(static_cast<CjvxProperties*>(this));
     deassociate_spectrumAnalyzerData();
     spectrumAnalyzer.props.deallocate__data();

     // beampattern simulator parameter properties
     beampatternSimulator.props.unregister__params(static_cast<CjvxProperties*>(this));
     beampatternSimulator.props.deassociate__params(static_cast<CjvxProperties*>(this));
     beampatternSimulator.props.deallocate__params();

     // beampattern simulator parameter properties
     beampatternSimulator.props.unregister__data(static_cast<CjvxProperties*>(this));
     deassociate_beampatternSimulatorData();
     beampatternSimulator.props.deallocate__data();

     return JVX_NO_ERROR;
}

jvxErrorType
CjvxRichAudioNode::put_configuration(IjvxConfigProcessor* processor,
                                     jvxHandle* sectionToContainAllSubsectionsForMe,
                                     const char* filename,
                                     jvxInt32 lineno)
{
     std::vector<std::string> warnings;
     if(_common_set.theState == JVX_STATE_ACTIVE)
          {
               spectrumAnalyzer.props.put_configuration__params(processor, sectionToContainAllSubsectionsForMe, warnings);
               beampatternSimulator.props.put_configuration__params(processor, sectionToContainAllSubsectionsForMe, warnings);
          }
     return JVX_NO_ERROR;
}

jvxErrorType
CjvxRichAudioNode::get_configuration(IjvxConfigProcessor* processor,
                                     jvxHandle* sectionWhereToAddAllSubsections)
{
     if(_common_set.theState >= JVX_STATE_ACTIVE)
          {
               spectrumAnalyzer.props.get_configuration__params(processor, sectionWhereToAddAllSubsections);
               beampatternSimulator.props.get_configuration__params(processor, sectionWhereToAddAllSubsections);
          }
     return JVX_NO_ERROR;
}

jvxErrorType
CjvxRichAudioNode::get_property(jvxHandle* fld,
                                jvxSize numElements,
                                jvxDataFormat format,
                                jvxSize propId,
                                jvxPropertyCategoryType category,
                                jvxSize offsetStart,
                                jvxBool contentOnly,
                                jvxBool* isValid,
                                jvxPropertyAccessType* accessType,
                                jvxPropertyCallPurpose callPurpose)
{
	jvxErrorType res = JVX_NO_ERROR;

	// special treatment for params which are not directly associated with the dsp lib or used only in this class
        if(beampatternSimulator.active)
             if(propId == beampatternSimulator.props.data.beampatternData.id)
                  {
                       res = processBeampatternSimulator();
                       if(res != JVX_NO_ERROR)
                            return res;
                  }

	res = CjvxAudioNode::get_property(fld, numElements, format, propId, category, offsetStart, contentOnly,
						isValid, accessType, callPurpose);
	return res;
}

jvxErrorType
CjvxRichAudioNode::set_property(jvxHandle* fld,
                                jvxSize numElements,
                                jvxDataFormat format,
                                jvxSize propId,
                                jvxPropertyCategoryType category,
                                jvxSize offsetStart,
                                jvxBool contentOnly,
                                jvxPropertyCallPurpose callPurpose)
{
     jvxErrorType res = CjvxAudioNode::set_property(fld, numElements, format, propId, category, offsetStart, contentOnly, callPurpose);

     if(res != JVX_NO_ERROR)
          return res;

     if(_common_set.theState >= JVX_STATE_PREPARED)
          res = updateInstrumentation(false);

     return res;
}


jvxErrorType
CjvxRichAudioNode::addSpectrumAnalyzer(int buffersize,
                                       int nChannels,
                                       int samplerate)
{
     std::cout<<"[Instrumentation] Adding spectrum analyzer (n = "<<buffersize<<", nCh = "<<nChannels<<", fs = "<<samplerate<<")"<<std::endl;

     spectrumAnalyzer.active = true;

     if(jvx_spectrumEstimation_init(&spectrumAnalyzer.hdl,
                                    buffersize,
                                    nChannels,
                                    samplerate)
        != JVX_DSP_NO_ERROR)
          return JVX_ERROR_INTERNAL;
     associate_spectrumAnalyzerData();
     return(JVX_NO_ERROR);
}

jvxErrorType
CjvxRichAudioNode::processSpectrumAnalyzer(jvxSize channel,
                                           jvxData *input,
                                           jvxData **spectrum)
{
     if(spectrumAnalyzer.props.params.spectrumEstimationEnabled.value)
          if(jvx_spectrumEstimation_process(&spectrumAnalyzer.hdl, input, spectrum, channel) != JVX_DSP_NO_ERROR)
               return JVX_ERROR_INTERNAL;
     return JVX_NO_ERROR;
}

jvxErrorType
CjvxRichAudioNode::addBeampatternSimulator(jvxHandle *micArrayHdl,
                                            jvx_beampattern_process_cb micArrayCb,
                                            int samplerate,
                                            jvxData speedOfSound,
                                            jvxBool removePropagationToArray,
                                            jvxData radiusSimulatedCircle)
{
     jvxData *beampattern;

     std::cout<<"[Instrumentation] Adding beampattern simulator (fs = "<<samplerate<<
          ", c = "<<speedOfSound<<", removePropagationToArray = "<<removePropagationToArray<<
          ", radiusSimulatedCircle = "<<radiusSimulatedCircle<<")"<<std::endl;

     beampatternSimulator.active = true;

     // set some parameters before init call
     // FIXME/BG: should these be associated / made available in gui?
     beampatternSimulator.hdl.prmSync.speedOfSound = speedOfSound;
     beampatternSimulator.hdl.prmSync.removePropagationToArray = removePropagationToArray;
     beampatternSimulator.hdl.prmSync.radiusCircleAround = radiusSimulatedCircle;

     if(jvx_beampattern_init(&beampatternSimulator.hdl,
                             samplerate,
                             micArrayHdl,
                             micArrayCb)
        != JVX_DSP_NO_ERROR)
          return JVX_ERROR_INTERNAL;
     associate_beampatternSimulatorData();
     return(JVX_NO_ERROR);
}

jvxErrorType JVX_CALLINGCONVENTION
CjvxRichAudioNode::beampatternSimulator_addMicrophone(jvxData x,
                                                      jvxData y,
                                                      jvxData z,
                                                      jvxSize *id)
{
     if(jvx_beampattern_addMicrophone(&beampatternSimulator.hdl, x, y, z, id) != JVX_DSP_NO_ERROR)
          return JVX_ERROR_INTERNAL;
     return JVX_NO_ERROR;
}

jvxErrorType
CjvxRichAudioNode::processBeampatternSimulator(jvxData **beampattern)
{
     jvxDspBaseErrorType res;

     if(beampatternSimulator.props.params.beampatternEnabled.value)
          {
               // locking because of asynchronous computation of beampattern
               JVX_LOCK_MUTEX(beampatternProcessMutex);
               res = jvx_beampattern_process(&beampatternSimulator.hdl, beampattern);
               JVX_UNLOCK_MUTEX(beampatternProcessMutex);
               if(res != JVX_DSP_NO_ERROR)
                    return JVX_ERROR_INTERNAL;
          }
     return JVX_NO_ERROR;
}

jvxErrorType
CjvxRichAudioNode::updateInstrumentation(jvxBool syncUpdate)
{
     jvxDspBaseErrorType res;

     if(spectrumAnalyzer.active)
          {
               res = jvx_spectrumEstimation_update(&spectrumAnalyzer.hdl, syncUpdate);
               if(res != JVX_DSP_NO_ERROR)
                    return JVX_ERROR_INTERNAL;
               if(syncUpdate)
                    {
                         deassociate_spectrumAnalyzerData();
                         associate_spectrumAnalyzerData();
                    }
          }
     if(beampatternSimulator.active)
          {
               // locking because of asynchronous computation of beampattern
               JVX_LOCK_MUTEX(beampatternProcessMutex);
               res = jvx_beampattern_update(&beampatternSimulator.hdl, syncUpdate);
               if(syncUpdate)
                    {
                         deassociate_beampatternSimulatorData();
                         associate_beampatternSimulatorData();
                    }
               JVX_UNLOCK_MUTEX(beampatternProcessMutex);
               if(res != JVX_DSP_NO_ERROR)
                    return JVX_ERROR_INTERNAL;
          }
     return JVX_NO_ERROR;
}



////
// private methods
////

void
CjvxRichAudioNode::associate_spectrumAnalyzerData()
{
     jvxData *spectrum;

     spectrum = jvx_spectrumEstimation_getOutputPointer(&spectrumAnalyzer.hdl);
     if (spectrum)
          {
               spectrumAnalyzer.props.associate__data(static_cast<CjvxProperties*>(this),
                                                      spectrum,
                                                      spectrumAnalyzer.hdl.debug.spectrumSize
                                                      * spectrumAnalyzer.hdl.cnst.nChannelsIn);
          }
}

void
CjvxRichAudioNode::deassociate_spectrumAnalyzerData()
{
     spectrumAnalyzer.props.deassociate__data(static_cast<CjvxProperties*>(this));
}


void
CjvxRichAudioNode::associate_beampatternSimulatorData()
{
     jvxData *beampattern;

     beampattern = jvx_beampattern_getOutputPointer(&beampatternSimulator.hdl);
     if(beampattern)
          {
               beampatternSimulator.props.associate__data(static_cast<CjvxProperties*>(this),
                                                          beampattern,
                                                          beampatternSimulator.hdl.debug.spectrumSize
                                                          * beampatternSimulator.hdl.prmSync.numAzimuthAngles);
          }
}

void
CjvxRichAudioNode::deassociate_beampatternSimulatorData()
{
     beampatternSimulator.props.deassociate__data(static_cast<CjvxProperties*>(this));
}
