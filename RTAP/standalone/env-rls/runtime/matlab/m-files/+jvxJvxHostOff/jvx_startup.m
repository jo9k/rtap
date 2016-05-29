function [hObject, handles] = jvx_startup(hObject, handles)
    
    % Initialize the host into a stable state
    [hObject, handles] = jvxJvxHostOff.jvx_init(hObject, handles);

    % Initialize the UI window to set some "always identical" properties
    [hObject, handles] = jvxJvxHostOff.jvx_init_ui(hObject, handles);
    
    % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostOff.jvx_refresh_props(hObject, handles);
       
    % Set hook function calls - if desired to subproject version
     gotSubProject = false;
    subproject = 'jvxDefault';
    if(handles.jvx_struct.properties.node.offline_node.id_subproject_name >= 0)
        [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_subproject_name, handles.jvx_struct.properties.node.offline_node.id_subproject_name);
        if(a)
            for(ind=1:size(handles.jvx_struct.subprojects, 2))
                if(strcmp(handles.jvx_struct.subprojects{ind}, b.SUBFIELD))
                    gotSubProject = true;
                    break;
                end
            end
        end
    end
    if(gotSubProject)
        subproject = b.SUBFIELD;
    end
    
    nm = [subproject  '.jvx_init_callback'];
    handles.local.cb_init = str2func(nm);
    nm = [subproject  '.jvx_start_callback'];
    handles.local.cb_start = str2func(nm);
    nm = [subproject  '.jvx_stop_callback'];
    handles.local.cb_stop = str2func(nm);
    nm = [subproject  '.jvx_term_callback'];
    handles.local.cb_term = str2func(nm);
    
    handles.local.private = [];
    if(isa(handles.local.cb_init, 'function_handle'))
        try
            handles.local.private = handles.local.cb_init(hObject, handles);
        catch ME
            a = functions(handles.local.cb_init);
             warning(['Function <' a.function '> failed, reason: ' ME.message]);             
        end
        %a = functions(handles.local.cb_init);
        %if(~isempty(a.file))
    end
    
    % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostOff.jvx_refresh_props(hObject, handles);

    % Verify that setting is valid for this session (files and workspace
    % vars)
     [hObject, handles] = jvxJvxHostOff.jvx_verify_setting(hObject, handles);
    
    % Show current status
    [hObject, handles] = jvxJvxHostOff.jvx_update_ui(hObject, handles);
    
