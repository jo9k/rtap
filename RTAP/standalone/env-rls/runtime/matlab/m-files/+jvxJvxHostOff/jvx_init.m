function [hObject, handles] = jvx_init(hObject, handles)

% ===========================================
% Prepare the system tables
% ===========================================
handles.jvx_struct.tables.components = jvxJvxHost.jvx_create_tables('jvxComponentType');

handles.jvx_struct.technologies.comp_type = jvxJvxHost.jvx_lookup_type_id__name(...
    handles.jvx_struct.tables.components, 'JVX_COMPONENT_AUDIO_TECHNOLOGY');
handles.jvx_struct.devices.comp_type = jvxJvxHost.jvx_lookup_type_id__name(...
    handles.jvx_struct.tables.components, 'JVX_COMPONENT_AUDIO_DEVICE');
handles.jvx_struct.nodes.comp_type = jvxJvxHost.jvx_lookup_type_id__name(...
    handles.jvx_struct.tables.components, 'JVX_COMPONENT_AUDIO_NODE');

handles.jvx_struct.tables.states = jvxJvxHost.jvx_create_tables('jvxState');
handles.jvx_struct.tables.formats = jvxJvxHost.jvx_create_tables('jvxDataFormat');
handles.jvx_struct.tables.sequence_element_type = jvxJvxHost.jvx_create_tables('jvxSequenceElementType');
handles.jvx_struct.tables.sequence_queue_type = jvxJvxHost.jvx_create_tables('jvxSequencerQueueType');
handles.jvx_struct.tables.sequence_event = jvxJvxHost.jvx_create_tables('jvxSequencerEvent');
handles.jvx_struct.tables.sequence_status = jvxJvxHost.jvx_create_tables('jvxSequencerStatus');
handles.jvx_struct.tables.property_access_type = jvxJvxHost.jvx_create_tables('jvxPropertyAccessType');
handles.jvx_struct.tables.feature_class_bitsets = jvxJvxHost.jvx_create_tables('jvxFeatureClassType');

handles.jvx_struct.images.notready = imread('images/icon_notready.png', 'png');
handles.jvx_struct.images.ready = imread('images/icon_ready.png', 'png');
handles.jvx_struct.images.proc = imread('images/icon_proc.png', 'png');

handles.jvx_struct.subprojects = {};
[path name ext] = fileparts(mfilename('fullpath'));
handles.jvx_struct.subpath = [fileparts(path) '/subprojects-audionode'];
%subfolders = ls(handles.jvx_struct.subpath);
subfolders = dir(handles.jvx_struct.subpath);
for(ind=1:size(subfolders,1))
    if(subfolders(ind).isdir)
        oneEntry = strtrim(subfolders(ind).name);
        if(...
                (strcmp(oneEntry, '.') ~= 1) & (strcmp(oneEntry, '..') ~= 1))
            theFolder = [handles.jvx_struct.subpath '/' oneEntry];
            if(exist(theFolder, 'dir'))
                if(strcmp( oneEntry(1), '+'))
                    handles.jvx_struct.subprojects = [handles.jvx_struct.subprojects oneEntry(2:end)];
                end
            end
        end
    end
end

% Processing data
handles.jvx_struct.properties_run.active = false;

handles.jvx_struct.data.input.data = [];
handles.jvx_struct.data.input.rate = 48000;
handles.jvx_struct.data.input.vars = '';

handles.jvx_struct.data.output.data = [];
handles.jvx_struct.data.output.rate = 48000;

handles.jvx_struct.data.channels.plot.id_user = -1;

handles.jvx_struct.properties.channels.input.id_user = -1;
handles.jvx_struct.properties.channels.output.id_user = -1;

handles.jvx_struct.data.player = [];
handles.jvx_struct.data.stereo = true;

handles.jvx_struct.data.output.channels.selection = -1;
handles.jvx_struct.data.output.channels.data = [];

% ============================================================
% Obtain information about host
% ============================================================
[a b]= handles.hostcall('info');
if(~a)
    jvxJvxHostOff.jvx_display_error('Error code 0x3',b);
else
    handles.jvx_struct.info = b;
end

% ============================================================
% Select the generic wrapper audio technology
% ============================================================
handles.jvx_struct.technologies.allTechnologies = struct2cell(handles.jvx_struct.info.COMPONENTS.AUDIO_TECHNOLOGY);
if(size(handles.jvx_struct.technologies.allTechnologies,2) == 1)
    idSelectedTechnology = -1;
    idStateSelectedTechnology = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_NONE');
    
    descrTech = handles.jvx_struct.technologies.allTechnologies{1}.DESCRIPTOR_STRING;
    if(strcmp(descrTech, 'JVX_OFFLINE_AUDIO') == 1)
        idSelectedTechnology = jvxJvxHost.jvx_mat_2_c_idx(1);
        idStateSelectedTechnology = handles.jvx_struct.technologies.allTechnologies{1}.STATE_INT32;
    else
        error('Unexpected');
    end
end

% ===========================================
% Select the offline audio technology - if not done so before
% ===========================================
if(idSelectedTechnology == 0)
    
    % Right technology picked before, let us check if we need to
    % select it at first to be at least in min state
    id_check_selected = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_SELECTED');
    id_check_active = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_ACTIVE');
    
    if(idStateSelectedTechnology < id_check_selected)
        [a b]= handles.hostcall('select_component', handles.jvx_struct.technologies.comp_type, idSelectedTechnology);
        if(~a)
            jvxJvxHostOff.jvx_display_error(4, 'jvxJvxHost.init', b.DESCRIPTION_STRING);
        end
    end
    
    if(idStateSelectedTechnology < id_check_active)
        [a b]= handles.hostcall('activate_component', handles.jvx_struct.technologies.comp_type);
        if(~a)
            jvxJvxHostOff.jvx_display_error(4, 'jvxJvxHost.init', b.DESCRIPTION_STRING);
        end
    end
    
    % ========================================================
    % Next step: Activate the single offline device
    % ========================================================
    % Read in the current system status struct
    [a b]= handles.hostcall('info');
    if(~a)
        jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
    else
        handles.jvx_struct.info = b;
    end
    
    handles.jvx_struct.technologies.allDevices = struct2cell(handles.jvx_struct.info.COMPONENTS.AUDIO_DEVICE);
    if(size(handles.jvx_struct.technologies.allDevices,2) == 1)
        idSelectedDevice = -1;
        idStateSelectedDevice = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_NONE');
        
        descrDev = handles.jvx_struct.technologies.allDevices{1}.DESCRIPTOR_STRING;
        if(strcmp(descrDev, 'JVX_OFFLINE_AUDIO_DEVICE') == 1)
            idSelectedDevice = jvxJvxHost.jvx_mat_2_c_idx(1);
            idStateSelectedDevice = handles.jvx_struct.technologies.allTechnologies{1}.STATE_INT32;
        else
            error('Unexpected');
        end
        
        
        if(idStateSelectedDevice < id_check_selected)
            [a b]= handles.hostcall('select_component', handles.jvx_struct.devices.comp_type, idSelectedDevice);
            if(~a)
                jvxJvxHostOff.jvx_display_error(4, 'jvxJvxHost.init', b.DESCRIPTION_STRING);
            end
        end
        
        if(idStateSelectedTechnology < id_check_active)
            [a b]= handles.hostcall('activate_component', handles.jvx_struct.devices.comp_type);
            if(~a)
                jvxJvxHostOff.jvx_display_error(4, 'jvxJvxHost.init', b.DESCRIPTION_STRING);
            end
        end
    else %if(size(handles.jvx_struct.technologies.allDevices,2) == 1)
        error('Unexpected');
    end %if(size(handles.jvx_struct.technologies.allDevices,2) == 1)
    
    [a b]= handles.hostcall('info');
    if(~a)
        jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
    else
        handles.jvx_struct.info = b;
    end
else
    jvxJvxHostOff.jvx_display_error('Error code 0x7');
end

% ======================================
% All related to the sequencer
% ======================================

[a b] = handles.hostcall('info_sequencer');
if(~a)
    jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
else
    if(isempty(b))
        
        % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
        
        % Add the default audio processing sequence
        [a b] = handles.hostcall('add_sequence', 'default sequence audio processing');
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
        
        id_sequence = 0;
        
        id_sequence_queue_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_queue_type, 'JVX_SEQUENCER_QUEUE_TYPE_RUN');
        id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_PREPARE');
        id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Prepare Audio Node', -1, -1, 'PrepANode', '', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Prepare Audio Device', -1, -1, 'PrepADevice','', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        % -  -  -  -  -
        
        id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_START');
        id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Start Audio Node', -1, -1, 'StartANode','', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Start Audio Device', -1, -1, 'StartADevice','', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        % -  -  -  -  -
        
        id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_REQUEST_UPDATE_VIEWER');
        id_sequence_comp_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.components, 'JVX_COMPONENT_UNKNOWN');
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Update Viewer', -1, -1, 'UpdateViewer','', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_BREAK');
        id_sequence_comp_type =jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.components, 'JVX_COMPONENT_UNKNOWN');
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Break sequencer steps', -1, -1, 'Break','', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
        
        id_sequence_queue_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_queue_type, 'JVX_SEQUENCER_QUEUE_TYPE_LEAVE');
        id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_STOP');
        id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Stop Audio Device', -1, -1, 'StopADevice', '', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Stop Audio Node', -1, -1, 'StopADevice', '', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        % -  -  -  -  -
        
        id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_POSTPROCESS');
        id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Postprocess Audio Device', -1, -1, 'PostADevice', '', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
        
        [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Postprocess Audio Node', -1, -1, 'PostANode', '', '', -1);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
        [a b] = handles.hostcall('set_active_sequence', id_sequence, true);
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
        
        [a b] = handles.hostcall('info_sequencer');
        if(~a)
            jvxJvxHostOff.jvx_display_error('Error code 0x6',b);
        end
    end
end

