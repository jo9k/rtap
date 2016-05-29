function [hObject, handles] = jvx_verify_setting(hObject, handles)

txtField = '';
isFile = false;

if(handles.jvx_struct.properties.device.id_mat_in_text)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_text, handles.jvx_struct.properties.device.id_mat_in_text);
    if(a)
        txtField = b.SUBFIELD;
    end
end

if(handles.jvx_struct.properties.device.id_mat_in_is_file)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_is_file, handles.jvx_struct.properties.device.id_mat_in_is_file);
    if(a)
        isFile = b.SUBFIELD.SELECTION_UINT64;
    end
end

if(~isempty(txtField))
    if(isFile)
        if(exist(txtField, 'file'))
            
            [tt, r] = audioread(txtField);
            
            handles.jvx_struct.data.input.data = tt';
            handles.jvx_struct.data.input.rate = r;
        else
            disp(['Warning: Input file ' fname ' does not exist.']);
            
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_text, handles.jvx_struct.properties.device.id_mat_in_text, '');
            if(~a)
                error('Unexpected');
            end
            
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_is_file, handles.jvx_struct.properties.device.id_mat_in_is_file, 0);
            if(~a)
                error('Unexpected');
            end
            
        end
        
    else
        a = whos( txtField, 'global');
        if(~isempty(a))
            if( ...
                    (strcmp(a.class, 'double') == 1) | ...
                    (strcmp(a.class, 'int64') == 1) | ...
                    (strcmp(a.class, 'int32') == 1) | ...
                    (strcmp(a.class, 'int16') == 1) | ...
                    (strcmp(a.class, 'int8') == 1))
                rate = handles.jvx_struct.data.input.rate;
                if(handles.jvx_struct.properties.device.id_mat_in_rate)
                    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_rate, handles.jvx_struct.properties.device.id_mat_in_rate);
                    if(a)
                        rate = b.SUBFIELD;
                    end
                end
                
                eval(['global ' txtField ';']);
                eval(['handles.jvx_struct.data.input.data = ' txtField ';']);
                handles.jvx_struct.data.input.rate = double(rate);
            else
                disp(['Warning: Global variable ' txtField ' is not of corect type.']);
                [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_text, handles.jvx_struct.properties.device.id_mat_in_text, '');
                if(~a)
                    error('Unexpected');
                end
                
                [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_is_file, handles.jvx_struct.properties.device.id_mat_in_is_file, 0);
                if(~a)
                    error('Unexpected');
                end
            end
        else
            disp(['Warning: Global variable ' txtField ' does not exist.']);
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_text, handles.jvx_struct.properties.device.id_mat_in_text, '');
            if(~a)
                error('Unexpected');
            end
            
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_mat_in_is_file, handles.jvx_struct.properties.device.id_mat_in_is_file, 0);
            if(~a)
                error('Unexpected');
            end
        end
    end
    
    if(handles.jvx_struct.properties.device.id_srate > 0)
        [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_srate, handles.jvx_struct.properties.device.id_srate,int32(handles.jvx_struct.data.input.rate));
        if(a)
        end
    end
    
    if(handles.jvx_struct.properties.device.id_in_channels_set > 0)
        [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels_set, handles.jvx_struct.properties.device.id_in_channels_set,int32(size(handles.jvx_struct.data.input.data,1)));
        if(a)
        end
    end
    
    % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostOff.jvx_refresh_props(hObject, handles);
    
    % Show current status
    [hObject, handles] = jvxJvxHostOff.jvx_update_ui(hObject, handles);
    
end