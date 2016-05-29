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

% Matlab subproject folders
handles.jvx_struct.subprojects = {};
[path name ext] = fileparts(mfilename('fullpath'));
handles.jvx_struct.subpath = [fileparts(path) '/subprojects-audionode'];
subfolders = ls(handles.jvx_struct.subpath);
for(ind=1:size(subfolders,1))
    oneEntry = strtrim(subfolders(ind,:));
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

% ============================================================
% Obtain information about host
% ============================================================
[a b]= handles.hostcall('info');
if(~a)
    jvxJvxHost.jvx_display_error('Error code 0x3',b);
else
    handles.jvx_struct.info = b;
end

% ============================================================
% Select the generic wrapper audio technology
% ============================================================
handles.jvx_struct.technologies.allTechnologies = struct2cell(handles.jvx_struct.info.COMPONENTS.AUDIO_TECHNOLOGY);
handles.jvx_struct.technologies.id_wrapper = 0;
idSelectedTechnology = 0;
idStateSelectedTechnology = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_NONE');
for(ind=1:size(handles.jvx_struct.technologies.allTechnologies,1))
    descrTech = handles.jvx_struct.technologies.allTechnologies{ind}.DESCRIPTOR_STRING;
    if(strcmp(descrTech, 'JVX_GENERIC_WRAPPER') == 1)
        handles.jvx_struct.technologies.id_wrapper = jvxJvxHost.jvx_mat_2_c_idx(ind);
    end
    if(handles.jvx_struct.technologies.allTechnologies{ind}.STATE_INT32 > 0)
        if(idSelectedTechnology == 0)
            idSelectedTechnology =jvxJvxHost.jvx_mat_2_c_idx(ind);
            idStateSelectedTechnology = handles.jvx_struct.technologies.allTechnologies{ind}.STATE_INT32;
        else
            error('Unexpected');
        end
    end
end

% ===========================================
% Select the generic wrapper audio technology - if not done so before
% ===========================================
if(handles.jvx_struct.technologies.id_wrapper >= 1)

    id_check_selected = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_SELECTED');
    if(idSelectedTechnology > 0)
        
        % In this case, there was a selected technology
        if(idSelectedTechnology ~= handles.jvx_struct.technologies.id_wrapper )
            
            % If not the generic wrapper, unselect it at first
            disp(['Warning: need to unselect previously selected technology ' handles.jvx_struct.technologies.allTechnologies{jvxJvxHost.jvx_c_2_mat_idx(idSelectedTechnology)}.DESCRIPTION_STRING ' at first.']);
             [a b]= handles.hostcall('unselect_component', handles.jvx_struct.technologies.comp_type);
             if(~a)
             end
             idStateSelectedTechnology =  jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_NONE');
        else
            
            % Right technology picked before, let us check if we need to
            % select it at first to be at least in min state
        end
    end
    
    if(idStateSelectedTechnology < id_check_selected)
        [a b]= handles.hostcall('select_component', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.technologies.id_wrapper);
        if(~a)
            jvxJvxHost.jvx_display_error(4, 'jvxJvxHost.init', b.DESCRIPTION_STRING);
        end
    end
        
    % Read in the current system status struct
    [a b]= handles.hostcall('info');
    if(~a)
        jvxJvxHost.jvx_display_error('Error code 0x6',b);
    else
        handles.jvx_struct.info = b;
    end
    
else
    jvxJvxHost.jvx_display_error('Error code 0x7');
end
    
 [a b] = handles.hostcall('info_sequencer');
 if(~a)
    jvxJvxHost.jvx_display_error('Error code 0x6',b);
 else
     if(isempty(b))
         
          % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 

         % Add the default audio processing sequence
         [a b] = handles.hostcall('add_sequence', 'default sequence audio processing');
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 

         id_sequence = 0;

         id_sequence_queue_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_queue_type, 'JVX_SEQUENCER_QUEUE_TYPE_RUN');
         id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_PREPARE');
         id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
         
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Prepare Audio Node', -1, -1, 'PrepANode', '', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Prepare Audio Device', -1, -1, 'PrepADevice','', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         % -  -  -  -  -  
         
         id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_START');
         id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
         
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Start Audio Node', -1, -1, 'StartANode','', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Start Audio Device', -1, -1, 'StartADevice','', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         % -  -  -  -  -  
         
         id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_REQUEST_UPDATE_VIEWER');
         id_sequence_comp_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.components, 'JVX_COMPONENT_UNKNOWN');

         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Update Viewer', -1, -1, 'UpdateViewer','', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_BREAK');
         id_sequence_comp_type =jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.components, 'JVX_COMPONENT_UNKNOWN');

         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Break sequencer steps', -1, -1, 'Break','', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
         
         id_sequence_queue_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_queue_type, 'JVX_SEQUENCER_QUEUE_TYPE_LEAVE');
         id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_STOP');
         id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
         
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Stop Audio Device', -1, -1, 'StopADevice', '', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
         
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Stop Audio Node', -1, -1, 'StopADevice', '', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         % -  -  -  -  -  
         
         id_sequence_element_type = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_element_type, 'JVX_SEQUENCER_TYPE_COMMAND_STD_POSTPROCESS');
         id_sequence_comp_type = handles.jvx_struct.devices.comp_type;
         
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Postprocess Audio Device', -1, -1, 'PostADevice', '', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         id_sequence_comp_type = handles.jvx_struct.nodes.comp_type;
         
         [a b] = handles.hostcall('insert_step_sequence', id_sequence, id_sequence_element_type, id_sequence_queue_type, id_sequence_comp_type, 'Postprocess Audio Node', -1, -1, 'PostANode', '', '', -1);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         % -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
          [a b] = handles.hostcall('set_active_sequence', id_sequence, true);
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
         
         [a b] = handles.hostcall('info_sequencer');
         if(~a)
             jvxJvxHost.jvx_display_error('Error code 0x6',b);
         end
     end
 end

 % Sub UI control
  hObject_subui = handles.subui.hdl;
  handles_subui = guidata(hObject_subui);
  [hObject_subui, handles_subui] = jvxJvxHostap.jvx_init(hObject_subui, handles_subui, ...
      handles.jvx_struct.technologies.comp_type, ...
      handles.jvx_struct.devices.comp_type);
  
  guidata(hObject_subui, handles_subui);