function [hObject, handles] = jvx_start_processing(hObject, handles)

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

set(handles.checkbox_force, 'Visible', 'on');

% Start sequencer
[a b] = handles.hostcall('start_process_sequence', 1000, -1, true);
if(~a)
    jvxJvxHostRt.jvx_display_error(13, mfilename, b.DESCRIPTION_STRING);
end

% Trigger once, all steps should happen within the one step in case of
% audio processing.
[a b] = handles.hostcall('trigger_external_process', 0);
if(~a)
    jvxJvxHostRt.jvx_display_error(14, mfilename, b.DESCRIPTION_STRING);
end

[hObject, handles] = jvxJvxHostRt.jvx_refresh_props_run(hObject, handles, true);
handles.jvx_struct.properties_run.active = true;

[hObject, handles] = jvxJvxHostRt.jvx_update_ui_run_start(hObject, handles);
[hObject, handles] = jvxJvxHostRt.jvx_set_gains(hObject, handles);

% Pass handles to GUI object for timer callback
guidata(hObject, handles);

% Combine window handle with timer handle
handles.jvx_struct.timer.hdl = timer('TimerFcn',@jvxJvxHostRt.jvx_timer_callback, 'Period', 0.2, 'ExecutionMode', 'fixedSpacing' );
subHandles.hObject_mainWindow = hObject;
set(handles.jvx_struct.timer.hdl, 'UserData', subHandles);

% Start timer
start(handles.jvx_struct.timer.hdl);

% Not needed ...
%[a b] = handles.hostcall('status_process_sequence');
%if(~a)
     %end