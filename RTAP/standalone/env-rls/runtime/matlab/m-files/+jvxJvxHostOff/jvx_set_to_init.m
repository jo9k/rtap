function [hObject, handles] = jvx_set_to_init(hObject, handles)

% Set into neutral state
[hObject, handles] = jvxJvxHostOff.jvx_set_to_neutral(hObject, handles);

% Means: no selection yet
handles.jvx_struct.input.selection = 0;

handles.jvx_struct.input.fName = '';
handles.jvx_struct.input.vName = '';

[a b]= handles.hostcall('info');
if(a)

    % The host has been initialized before, re-initialize
    [a b]= handles.hostcall('terminate');
    if(~a)
        jvxJvxHostRt.jvx_display_error('Error code 0x1', b);
    end
end