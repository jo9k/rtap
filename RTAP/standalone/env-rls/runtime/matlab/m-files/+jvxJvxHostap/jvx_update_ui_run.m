function  [hObject, handles] = jvx_update_ui_run(hObject, handles, input_file_id, comp_type_tech, comp_type_dev)

set(handles.text_input_progress_0, 'BackgroundColor', [0 0.1 0]);
set(handles.text_input_progress_1, 'BackgroundColor', [0 .1 0]);
set(handles.text_input_progress_2, 'BackgroundColor', [0 .1 0]);
set(handles.text_input_progress_3, 'BackgroundColor', [0 .1 0]);
set(handles.text_input_progress_4, 'BackgroundColor', [0 .1 0]);
set(handles.text_input_progress_5, 'BackgroundColor', [0 .1 0]);

set(handles.text_load_0, 'BackgroundColor', [0 0.1 0]);
set(handles.text_load_1, 'BackgroundColor', [0 .1 0]);
set(handles.text_load_2, 'BackgroundColor', [0 .1 0]);
set(handles.text_load_3, 'BackgroundColor', [0 .1 0]);
set(handles.text_load_4, 'BackgroundColor', [0 .1 0]);
set(handles.text_load_5, 'BackgroundColor', [0 .1 0]);

if(input_file_id >= 1)
    if(handles.jvx_struct.properties.technology.id_file_progress >= 0)
        [a b]= handles.hostcall('get_property_uniqueid', comp_type_tech, handles.jvx_struct.properties.technology.cat_file_progress, handles.jvx_struct.properties.technology.id_file_progress);
        if(a)
            progress = b.SUBFIELD(input_file_id) * 100;
            progressFrac = progress/100 * 6;
            if(progressFrac >= 0.0)
                set(handles.text_input_progress_0, 'BackgroundColor', [0 1 0]);
            end
            if(progressFrac >= 1.0)
                set(handles.text_input_progress_1, 'BackgroundColor', [0 1 0]);
            end
            if(progressFrac >= 2.0)
                set(handles.text_input_progress_2, 'BackgroundColor', [0 1 0]);
            end
            if(progressFrac >= 3.0)
                set(handles.text_input_progress_3, 'BackgroundColor', [0 1 0]);
            end
            if(progressFrac >= 4.0)
                set(handles.text_input_progress_4, 'BackgroundColor', [0 1 0]);
            end
            if(progressFrac >= 5.0)
                set(handles.text_input_progress_5, 'BackgroundColor', [0 1 0]);
            end
            
            set(handles.text_progress, 'String', [num2str(progress,  '%.1f') ' %']);
        else
            error('XXX');
        end
    end
end

if(handles.jvx_struct.properties.device.id_dev_load >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', comp_type_dev, handles.jvx_struct.properties.device.cat_dev_load, handles.jvx_struct.properties.device.id_dev_load);
    if(a)
        load = b.SUBFIELD;
        loadFrac = load/100 * 6;
        if(loadFrac >= 0.0)
            set(handles.text_load_0, 'BackgroundColor', [0 1 0]);
        end
        if(loadFrac >= 1.0)
            set(handles.text_load_1, 'BackgroundColor', [0 1 0]);
        end
        if(loadFrac >= 2.0)
            set(handles.text_load_2, 'BackgroundColor', [0 1 0]);
        end
        if(loadFrac >= 3.0)
            set(handles.text_load_3, 'BackgroundColor', [0 1 0]);
        end
        if(loadFrac >= 4.0)
            set(handles.text_load_4, 'BackgroundColor', [0 1 0]);
        end
        if(loadFrac >= 5.0)
            set(handles.text_load_5, 'BackgroundColor', [0 1 0]);
        end
        
        set(handles.text_load, 'String', [num2str(load,  '%.1f') ' %']);

else
        error('XXX');
    end
end

