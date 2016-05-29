#ifndef __CJVXRICHAUDIONODE_H__
#define __CJVXRICHAUDIONODE_H__

#include "jvx.h"
#include "templates/jvxComponents/jvxAudioNodes/CjvxAudioNode.h"
#include "jvx_spectrumEstimation/jvx_spectrumEstimation.h"
#include "pcg_jvx_spectrumEstimation.h"
#include "jvx_beampattern/jvx_beampattern.h"
#include "pcg_jvx_beampattern.h"

class CjvxRichAudioNode : public CjvxAudioNode
{
 protected:

	JVX_CALLINGCONVENTION CjvxRichAudioNode(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxRichAudioNode();

	virtual jvxErrorType JVX_CALLINGCONVENTION activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint,
							   jvxSize numEndpoints);

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess();

        virtual jvxErrorType JVX_CALLINGCONVENTION process_st(jvxLinkDataDescriptor* theData,
                                                              jvxSize idx_sender_to_receiver,
                                                              jvxSize idx_receiver_to_sender);

	virtual jvxErrorType JVX_CALLINGCONVENTION put_configuration(IjvxConfigProcessor* processor,
								     jvxHandle* sectionToContainAllSubsectionsForMe,
								     const char* filename,
								     jvxInt32 lineno);

	virtual jvxErrorType JVX_CALLINGCONVENTION get_configuration(IjvxConfigProcessor* processor,
								     jvxHandle* sectionWhereToAddAllSubsections);

        virtual jvxErrorType JVX_CALLINGCONVENTION set_property(jvxHandle* fld,
                                                                jvxSize numElements,
                                                                jvxDataFormat format,
                                                                jvxSize propId,
                                                                jvxPropertyCategoryType category,
                                                                jvxSize offsetStart,
                                                                jvxBool contentOnly,
                                                                jvxPropertyCallPurpose callPurpose);

        virtual jvxErrorType JVX_CALLINGCONVENTION get_property(jvxHandle* fld,
                                                                jvxSize numElements,
                                                                jvxDataFormat format,
                                                                jvxSize propId,
                                                                jvxPropertyCategoryType category,
                                                                jvxSize offsetStart,
                                                                jvxBool contentOnly,
                                                                jvxBool* isValid,
                                                                jvxPropertyAccessType* accessType,
                                                                jvxPropertyCallPurpose callPurpose);
        /////////////////////////////////////////////
        // instrumentation and measurement methods //
        /////////////////////////////////////////////

        ////
        // spectrum analyzer
        ////

        jvxErrorType JVX_CALLINGCONVENTION addSpectrumAnalyzer(int bufferszie,
                                                                int nChannels,
                                                                int samplerate);

        jvxErrorType JVX_CALLINGCONVENTION processSpectrumAnalyzer(jvxSize channel,
                                                                   jvxData *input,
                                                                   jvxData **spectrum = NULL);

        ////
        // beampattern simulator
        ////

        // FIXME/BG: only planar simulation supported so far... elevation should be kept at 0 for now
	jvxErrorType JVX_CALLINGCONVENTION addBeampatternSimulator(jvxHandle *micArrayHdl,
                                                                    jvx_beampattern_process_cb micArrayCb,
                                                                    int samplerate,
                                                                    jvxData speedOfSound,
                                                                    jvxBool removePropagationToArray,
                                                                    jvxData radiusSimulatedCircle);

        /*
         * Coordinate system:
         *
         *
         *                         | +z (90°/undefined)
         *                         |
         *                         |     (0°/0°)
         *                         |    / +y
         *                         |   /
         *                         |  /
         *                         | /
         *                         | ---------------- +x (0°/270°)
         */
        jvxErrorType JVX_CALLINGCONVENTION beampatternSimulator_addMicrophone(jvxData x,
                                                                              jvxData y,
                                                                              jvxData z,
                                                                              jvxSize *id = NULL);

        jvxErrorType JVX_CALLINGCONVENTION updateInstrumentation(jvxBool syncUpdate);

 private:

        struct {
          jvxBool active;
          jvx_spectrumEstimation hdl;
          spectrumEstimationProps props;
        } spectrumAnalyzer;

        struct {
          jvxBool active;
          jvx_beampattern hdl;
          beampatternProps props;
        } beampatternSimulator;

        void associate_spectrumAnalyzerData();
        void associate_beampatternSimulatorData();
        void deassociate_spectrumAnalyzerData();
        void deassociate_beampatternSimulatorData();

        JVX_MUTEX_HANDLE beampatternProcessMutex;

        // this is private because it is automatically called on demand by get_property
        jvxErrorType processBeampatternSimulator(jvxData **beampattern = NULL);
};

#endif
