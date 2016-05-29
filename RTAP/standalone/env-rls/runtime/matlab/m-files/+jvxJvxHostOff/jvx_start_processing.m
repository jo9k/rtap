function [status_seq, hObject, handles] = jvx_start_processing(hObject, handles)

status_seq = -1;

% Callback for project specific actions
if(isfield(handles, 'local'))
    if(isa(handles.local.cb_start, 'function_handle'))
        try
            handles.local.private = handles.local.cb_start(handles.local.private);
        catch ME
            a = functions(handles.local.cb_start);
            warning(['Function <' a.function '> failed, reason: ' ME.message]);
        end
    end
end
    
% Start sequencer
[a b] = handles.hostcall('start_process_sequence', 1000, -1, true);
if(~a)
    jvxJvxHostOff.jvx_display_error(13, mfilename, b.DESCRIPTION_STRING);
end

% Trigger once, all steps should happen within the one step in case of
% audio processing.
[a status_seq] = handles.hostcall('trigger_external_process', 0);
if(~a)
    jvxJvxHostOff.jvx_display_error(14, mfilename, status_seq.DESCRIPTION_STRING);
end

