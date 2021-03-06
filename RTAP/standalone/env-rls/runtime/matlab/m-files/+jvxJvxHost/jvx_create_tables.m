function [tbl] = jvx_create_tables(descriptor, compiledForDoublePrecision)

% There is one place where we need to distinguish between double and single
% version
if(nargin == 1)
    compiledForDoublePrecision = true;
end

if(strcmp(descriptor, 'jvxComponentType') == 1)
    tbl = ...
        { ...
            {'JVX_COMPONENT_UNKNOWN', 0, '--'},...
            ...
            {'JVX_COMPONENT_AUDIO_TECHNOLOGY', 1, '--'}, ...
            {'JVX_COMPONENT_AUDIO_DEVICE', 2, '--'}, ...
            {'JVX_COMPONENT_AUDIO_NODE', 3, '--'}, ...
            {'JVX_COMPONENT_EXTERNAL_CONTROLLER_TECHNOLOGY', 4, '--'}, ...
            {'JVX_COMPONENT_EXTERNAL_CONTROLLER_DEVICE', 5, '--'}, ...
            {'JVX_COMPONENT_HOST', 6, '--'}, ...
            ...
			{'JVX_COMPONENT_RT_AUDIO_FILE_READER,', 7, '--'}, ...
			{'JVX_COMPONENT_RT_AUDIO_FILE_WRITER,', 8, '--'}, ...
			{'JVX_COMPONENT_RESAMPLER, ', 9, '--'}, ...
			{'JVX_COMPONENT_DATACONVERTER,', 10, '--'}, ...
			...
            {'JVX_COMPONENT_DATALOGGER', 11, '--'}, ...
            {'JVX_COMPONENT_DATALOGREADER', 12, '--'}, ...
            {'JVX_COMPONENT_RS232', 13, '--'}, ...
            {'JVX_COMPONENT_CONFIG_PROCESSOR', 14, '--'}, ...
			{'JVX_COMPONENT_EXTERNAL_CALL', 15, '--'}, ...
            {'JVX_COMPONENT_THREADCONTROLLER', 16, '--'}, ...
			{'JVX_COMPONENT_CRYPT', 17, 'crypt'}, ...
			{'JVX_COMPONENT_WEBSERVER', 18, 'web server'}	
        };
    return;
end

if(strcmp(descriptor, 'jvxSequencerQueueType') == 1)
    tbl = ...
        { ...
           {'JVX_SEQUENCER_QUEUE_TYPE_NONE', 0, '--'}, ...
            {'JVX_SEQUENCER_QUEUE_TYPE_RUN', 1, '--'}, ...
            {'JVX_SEQUENCER_QUEUE_TYPE_LEAVE', 2, '--'}
        };
    return;
end

if(strcmp(descriptor, 'jvxSequenceElementType') == 1)
    tbl = ...
        { ...
           {'JVX_SEQUENCER_TYPE_COMMAND_NONE', 0, '--'}, ...
            {'JVX_SEQUENCER_TYPE_COMMAND_STD_ACTIVATE', 1, '--'}, ...
            {'JVX_SEQUENCER_TYPE_COMMAND_STD_PREPARE', 2, '--'},...
            {'JVX_SEQUENCER_TYPE_COMMAND_STD_START', 3, '--'}, ...
            {'JVX_SEQUENCER_TYPE_COMMAND_STD_STOP', 4, '--'},...
            {'JVX_SEQUENCER_TYPE_COMMAND_STD_POSTPROCESS', 5, '--'}, ...
            {'JVX_SEQUENCER_TYPE_COMMAND_STD_DEACTIVATE', 6, '--'},...
            {'JVX_SEQUENCER_TYPE_CONDITION_WAIT', 7, '--'}, ...
            {'JVX_SEQUENCER_TYPE_CONDITION_JUMP', 8, '--'},...
            {'JVX_SEQUENCER_TYPE_JUMP', 9, '--'}, ...
            {'JVX_SEQUENCER_TYPE_WAIT_FOREVER', 10, '--'}, ...
            {'JVX_SEQUENCER_TYPE_WAIT_CONDITION_RELATIVE_JUMP', 11, '--'}, ...
            {'JVX_SEQUENCER_TYPE_BREAK', 12, '--'}, ...
            {'JVX_SEQUENCER_TYPE_REQUEST_UPDATE_VIEWER', 13, '--'}
            };
    return;
end

if(strcmp(descriptor, 'jvxSequencerEvent') == 1)
    tbl = ...
        { ...
        {'JVX_SEQUENCER_EVENT_NONE', 0, '--'}, ...
        {'JVX_SEQUENCER_EVENT_PROCESS_STARTUP_COMPLETE', 1, '--'}, ...
        {'JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_STEP', 2, '--'}, ...
        {'JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_STEP_ERROR', 4, '--'}, ...
        {'JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_SEQUENCE', 8, '--'}, ...
        {'JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_SEQUENCE_ERROR', 16, '--'}, ...
        {'JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_SEQUENCE_ABORT', 32, '--'}, ...
        {'JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_PROCESS', 64, '--'}, ...
        {'JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_PROCESS_ERROR', 128, '--'}, ...
        {'JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_PROCESS_ABORT', 256, '--'}, ...
        {'JVX_SEQUENCER_EVENT_PROCESS_TERMINATED', 512, '--'}
        };
    return;
end
                 
if(strcmp(descriptor, 'jvxSequencerStatus') == 1)
    tbl = ...
        { ...
        {'JVX_SEQUENCER_STATUS_NONE', 0, '--'}, ...
        {'JVX_SEQUENCER_STATUS_STARTUP', 1, '--'}, ...
        {'JVX_SEQUENCER_STATUS_IN_OPERATION', 2, '--'}, ...
        {'JVX_SEQUENCER_STATUS_WAITING', 3, '--'}, ...
        {'JVX_SEQUENCER_STATUS_WANTS_TO_SHUTDOWN', 4, '--'}, ...
        {'JVX_SEQUENCER_STATUS_SHUTDOWN', 5, '--'}, ...
        {'JVX_SEQUENCER_STATUS_ERROR', 6, '--'}, ...
        {'JVX_SEQUENCER_STATUS_SHUTDOWN_COMPLETE', 7, '--'}
        };
        return;
end 

if(strcmp(descriptor, 'jvxDataFormat') == 1)
    tbl = ...
        { ...
        {'JVX_DATAFORMAT_NONE', 0, '--'}, ...
        {'JVX_DATAFORMAT_DATA', 1, '--'}, ...
        {'JVX_DATAFORMAT_16BIT_LE', 2, '--'}, ...
        {'JVX_DATAFORMAT_24BIT_LE', 3, '--'}, ...
        {'JVX_DATAFORMAT_32BIT_LE', 4, '--'}, ...
        {'JVX_DATAFORMAT_64BIT_LE', 5, '--'}, ...
        {'JVX_DATAFORMAT_8BIT', 6, '--'}, ...
        {'JVX_DATAFORMAT_STRING', 7, '--'}, ...
        {'JVX_DATAFORMAT_STRING_LIST', 8, '--'}, ...
        {'JVX_DATAFORMAT_SELECTION_LIST', 9, '--'}, ...
        {'JVX_DATAFORMAT_VALUE_IN_RANGE', 10, '--'}
        };
    return;
end

if(strcmp(descriptor, 'jvxDataFormat_short') == 1)
    if(compiledForDoublePrecision)
        tbl = ...
            { ...
            {'none', 0,  '--'}, ...
            {'jvxData(double)',1 ,  '--'}, ...
            {'16 Bit le', 2,  '--'}, ...
            {'24 Bit le', 3,  '--'}, ...
            {'32 Bit le', 4,  '--'}, ...
            {'64 Bit le', 5,  '--'}, ...
            {'8 Bit', 6,  '--'}, ...
            {'string', 7,  '--'}, ...
            {'string list', 8,  '--'}, ...
            {'selection list', 9,  '--'}, ...
            {'value in range', 10,  '--'}
            };
    else
        tbl = ...
            { ...
            {'none', 0,  '--'}, ...
            {'jvxData(single)',1 ,  '--'}, ...
            {'16 Bit le', 2,  '--'}, ...
            {'24 Bit le', 3,  '--'}, ...
            {'32 Bit le', 4,  '--'}, ...
            {'64 Bit le', 5,  '--'}, ...
            {'8 Bit', 6,  '--'}, ...
            {'string', 7,  '--'}, ...
            {'string list', 8,  '--'}, ...
            {'selection list', 9,  '--'}, ...
            {'value in range', 10,  '--'}
            };
    end
    return;
end

if(strcmp(descriptor, 'jvxState') == 1)
    tbl = ...
        { ...
        {'JVX_STATE_NONE', 0, '--'}, ...
        {'JVX_STATE_INIT', 1, '--'}, ...
        {'JVX_STATE_SELECTED', 2, '--'}, ...
        {'JVX_STATE_ACTIVE', 4, '--'}, ...
        {'JVX_STATE_PREPARED', 8, '--'}, ...
        {'JVX_STATE_PROCESSING', 16, '--'}
        };
    return;
end

if(strcmp(descriptor, 'jvxPropertySetType') == 1)
    tbl = ...
        { ...
        {'JVX_THREADING_NONE', 0, '--'}, ...
        {'JVX_THREADING_INIT', 1, '--'}, ...
        {'JVX_THREADING_ASYNC', 2, '--'}, ...
        {'JVX_THREADING_SYNC', 4, '--'}
        };
    return;
end

if(strcmp(descriptor, 'jvxPropertyCategoryType') == 1)
    tbl = ...
        { ...
        {'JVX_PROPERTY_CATEGORY_UNKNOWN', 0, '--'}, ...
        {'JVX_PROPERTY_CATEGORY_PREDEFINED', 1, '--'}, ...
        {'JVX_PROPERTY_CATEGORY_UNSPECIFIC', 2, '--'}
        };
    return;
end

if(strcmp(descriptor, 'jvxPropertyAccessType') == 1)
    tbl = ...
        { ...
        {'JVX_PROPERTY_ACCESS_NONE', 0, '--'}, ...
        {'JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE', 1, '--'}, ...
        {'JVX_PROPERTY_ACCESS_READ_AND_WRITE_CONTENT', 2, '--'}, ...
        {'JVX_PROPERTY_ACCESS_READ_ONLY', 3, '--'}, ...
        {'JVX_PROPERTY_ACCESS_WRITE_ONLY', 4, '--'}
        };
    return;
end

if(strcmp(descriptor, 'jvxPropertyDecoderHintType') == 1)
    tbl = ...
        { ...
        {'JVX_PROPERTY_DECODER_NONE', 0, '--'}, ...
        {'JVX_PROPERTY_DECODER_FILENAME_OPEN', 1, '--'}, ...
        {'JVX_PROPERTY_DECODER_DIRECTORY_SELECT', 2, '--'}, ...
        {'JVX_PROPERTY_DECODER_FILENAME_SAVE', 3, '--'}, ...
        {'JVX_PROPERTY_DECODER_IP_ADDRESS', 4, '--'}, ...
        {'JVX_PROPERTY_DECODER_PLOT_ARRAY', 5, '--'}, ...
        {'JVX_PROPERTY_DECODER_BITFIELD', 6, '--'}, ...
        {'JVX_PROPERTY_DECODER_SINGLE_SELECTION', 7, '--'}, ...
        {'JVX_PROPERTY_DECODER_MULTI_SELECTION', 8, '--'}, ...
        {'JVX_PROPERTY_DECODER_FORMAT_IDX', 9, '--'}, ...
        {'JVX_PROPERTY_DECODER_PROGRESSBAR', 10, '--'}, ...
		{'JVX_PROPERTY_DECODER_INPUT_FILE_LIST', 11, '--'}, ...
		{'JVX_PROPERTY_DECODER_OUTPUT_FILE_LIST', 12, '--'}, ...
		{'JVX_PROPERTY_DECODER_MULTI_SELECTION_CHANGE_ORDER', 13, '--'}, ...
		{'JVX_PROPERTY_DECODER_MULTI_CHANNEL_CIRCULAR_BUFFER', 14, '--'}, ...
		{'JVX_PROPERTY_DECODER_COMMAND', 15, '--'}
        };
    return;
end

if(strcmp(descriptor, 'jvxFeatureClassType') == 1)
    tbl = ...
        { ...
        {'JVX_FEATURE_CLASS_NONE', 0, '--'}, ...
        {'JVX_FEATURE_CLASS_EXTERNAL_CALL', 1, '--'},...
        {'JVX_FEATURE_CLASS_HOOKUP_CALL', 2, '--'}        
        };
    return;
end

if(strcmp(descriptor, 'jvxReportEventType') == 1)
    tbl = ...
        { ...
           {'JVX_REPORT_REQUEST_TRY_TRIGGER_STOP_SEQUENCER', 0, 'stop sequencer'}, ...
           {'JVX_REPORT_REQUEST_TRY_TRIGGER_START_SEQUENCER', 1, 'start sequencer'}, ...
           {'JVX_REPORT_REQUEST_UPDATE_WINDOW', 2, 'stop sequencer'}, ...
           {'JVX_REPORT_REQUEST_USER_MESSAGE', 3, 'user event'}
           };
    return;
end

if(strcmp(descriptor, 'jvxPropertyLookaheadSizes') == 1)
        tbl = ...
            { ...
                1,2,5,7,10,15,20,30,50,100 ...
             };
         return;
end

error(['Failed to generate table for table specification <' descriptor '>']);