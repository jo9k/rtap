#ifndef __JVXIOSCPPHOST__H__
#define __JVXIOSCPPHOST__H__

#import "jvx.h"
#import "allHostsStatic_common.h"
#import "codeFragments/commandline/CjvxCommandline.h"
#include "templates/common/CjvxRealtimeViewer.h"
#include "realtimeViewer_helpers.h"
#include "helpers/tools/HjvxReadConfigEntries.h"
#include "helpers/tools/HjvxWriteConfigEntries.h"

#include "jvxIosHost_typedefs.h"

#define JVX_IOS_HOST_1_SECTION_MAIN "jvxIosHost1_main"
#define JVX_IOS_HOST_1_SECTION_COMPONENTS "jvxIosHost1_components"
#define JVX_IOS_HOST_1_EXTERNAL_TRIGGER  "jvxIosHost1_exttrigger"

// Print all configuration text in compact form
#define JVX_QT_HOST_1_PRINT_COMPACT_FORM false

// Callback function to pass data to obj c objects
typedef jvxErrorType (*callback)(jvxIosBwdReportEvent commandId, void* priv_data, void *context);

class jvxIosCppHost: public IjvxReport, public IjvxSequencer_report, public CjvxCommandline
{
    struct
    {
        callback cb_entry;
        void* cb_private;
    }bwd_ref;
    
    jvxBool hasBeenInitializedBefore;
    
    //void* theObjCRef;

    struct
    {
        IjvxObject* hObject;
        IjvxHost* hHost;
        IjvxToolsHost* hTools;
        
        std::vector<oneAddedStaticComponent> addedStaticObjects;

    } theSystemRefs;
    
    struct
    {
        jvxBool running;
        IjvxSequencer* theSeq;
        jvxData progress;
    } stateRun;
    
    CjvxRealtimeViewer viewer_props;
    CjvxRealtimeViewer viewer_plots;
    
public:

    jvxIosCppHost();
    
    ~jvxIosCppHost();
    
    IjvxHost* get_host_ref(){return(theSystemRefs.hHost);};
        
    // ===================================================================

    jvxErrorType initialize(callback bwd_cb, void* priv_cb, const char* fName_config);
    jvxErrorType terminate();

    // ===================================================================
    
    virtual jvxErrorType JVX_CALLINGCONVENTION report_simple_text_message(const char* txt, jvxReportPriority prio);

    virtual jvxErrorType JVX_CALLINGCONVENTION report_internals_have_changed(jvxComponentType thisismytype, IjvxObject* thisisme,
                                                                             jvxPropertyCategoryType cat, jvxInt16 propId, bool onlyContent, jvxComponentType tpTo,
                                                                             jvxPropertyCallPurpose callpurpose = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);

    virtual jvxErrorType JVX_CALLINGCONVENTION report_take_over_property(jvxComponentType thisismytype, IjvxObject* thisisme,
                                                                         jvxHandle* fld, jvxSize numElements, jvxDataFormat format, jvxSize offsetStart, jvxBool onlyContent,
                                                                         jvxPropertyCategoryType cat, jvxInt16 propId, jvxComponentType tpTo, jvxPropertyCallPurpose callpurpose = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);

    virtual jvxErrorType JVX_CALLINGCONVENTION report_command_request(jvxCommandRequestType request);
    
    virtual jvxErrorType JVX_CALLINGCONVENTION report_os_specific(jvxSize commandId, void* context);
    
    // ===================================================================
    
    // Report the status of the sequencer if desired
    virtual jvxErrorType JVX_CALLINGCONVENTION report_event(jvxUInt64 event_mask, const char* description, jvxInt16 sequenceId, jvxInt16 stepId, jvxSequencerQueueType tp, jvxSequencerElementType stp = JVX_SEQUENCER_TYPE_COMMAND_NONE);
    
    virtual jvxErrorType JVX_CALLINGCONVENTION sequencer_callback(jvxInt16 sequenceId, jvxInt16 stepId, jvxSequencerQueueType tp, jvxInt32 functionId); 
    // ===================================================================

    jvxErrorType bootup_specific();
    jvxErrorType shutdown_specific();
    
    jvxErrorType bootup_finalize_specific();
    jvxErrorType prepare_shutdown_specific();
    
    jvxErrorType start_run();

    jvxErrorType stop_run();

    jvxErrorType state_run(jvxBool* isRunning);
    // ===================================================================

    void postMessage_outThread(const char* txt, jvxReportPriority prio);

    void postMessage_inThread(std::string txt, jvxReportPriority prio);
    
    // ===================================================================
    
    jvxErrorType configureFromFile(const char* fName_config);
    
    jvxErrorType rtv_get_configuration(IjvxConfigProcessor* theReader, jvxHandle* ir, IjvxHost* theHost, CjvxRealtimeViewer& theViewer, jvxRealtimeViewerType configurationType);
    
    jvxErrorType rtv_put_configuration(IjvxConfigProcessor* theReader, jvxHandle* ir, IjvxHost* theHost, const char* fName, int lineno, std::vector<std::string>& warnings, CjvxRealtimeViewer& theViewer, jvxRealtimeViewerType configurationType);
    
    jvxErrorType configurationToFile(const char* fName_config);
    
    // ===================================================================
    
    void addDefaultSequence(bool onlyIfNoSequence);
    
    void activateDefaultAlgorithm();
    
    void deactivateDefaultAlgorithm();

    void removeAllSequencer();
    
    jvxErrorType request_component_filewriter(jvxHandle** instOnReturn);
    jvxErrorType open_filewriter_write_batchmode(jvxHandle* hdl, const char* fName, jvxSize channels, jvxInt32 sRate, jvxDataFormat format);
    jvxErrorType set_tag_filewriter_write_batchmode(jvxHandle* hdl, jvxAudioFileTagType , const char*);
    jvxErrorType add_buffer_filewriter_write_batch_mode_interleaved(jvxHandle* hdl, jvxHandle* buffer, jvxSize bsize);
    jvxErrorType start_filewriter_write_batchmode(jvxHandle* hdl);
    jvxErrorType stop_filewriter_write_batchmode(jvxHandle* hdl);
    jvxErrorType close_filewriter_write_batchmode(jvxHandle* hdl);
    jvxErrorType return_component_filewriter(jvxHandle* instToReturn);

    jvxErrorType removeFile(const char* fName);
    
    jvxErrorType produce_list_file_extension(std::string dirname, std::string ext,
                                             std::vector<std::string>& lstfileNames,
                                             std::vector<std::string>& lstTagsNames,
                                             std::vector<jvxSize>& lstChannels,
                                             std::vector<jvxInt32>& lstRates,
                                             std::vector<jvxSize>& lstLengths);

    jvxErrorType request_component_filereader(jvxHandle** instOnReturn);
    jvxErrorType return_component_filereader(jvxHandle* instToReturn);
    
    jvxErrorType set_external_config_entry(const char* entryname, jvxHandle* val, jvxConfigSectionTypes tp, jvxInt32 id);
    jvxErrorType get_external_config_entry(const char* entryname, jvxHandle** val, jvxConfigSectionTypes* tp, jvxInt32 id);
    jvxErrorType host_deallocate(jvxHandle* val, jvxConfigSectionTypes tp);
};

#endif