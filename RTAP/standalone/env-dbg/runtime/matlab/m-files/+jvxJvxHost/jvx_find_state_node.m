function [idState] = find_state_node(info, handles)
    
id_check_none = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_NONE');
idState = id_check_none;
tt = struct2cell(info.COMPONENTS.AUDIO_NODES);;
for(ind = 1:size(tt,2))
    if(tt{ind}.STATE_INT32 ~= id_check_none)
        idState = tt{ind}.STATE_INT32;
        break;
    end
end