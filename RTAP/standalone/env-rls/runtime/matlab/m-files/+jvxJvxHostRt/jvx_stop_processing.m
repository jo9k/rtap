function [hObject, handles] = jvx_stop_processing(hObject, handles, errorsAsWarnings)

    if(nargin <= 2)
        errorsAsWarnings = false;
    end
% If we are not in neutral state, do verything to shutdown the
     % sequencer to reach neutral state
     if(isfield(handles.jvx_struct, 'timer'))
         if(isvalid( handles.jvx_struct.timer.hdl))
             stop( handles.jvx_struct.timer.hdl);
             delete( handles.jvx_struct.timer.hdl);
         end
         end
     handles.jvx_struct.properties_run.active = false;
     
      [hObject, handles] = jvxJvxHostRt.jvx_refresh_props_run(hObject, handles, false);
     % Trigger shutdown of sequencer
     [a b] = handles.hostcall('trigger_stop_process');
     if(~a)
         jvxJvxHost.jvx_display_error(15, mfilename, b.DESCRIPTION_STRING, errorsAsWarnings); 
     end
     
     % Move processing forward in one step for audio
     [a b] = handles.hostcall('trigger_external_process', 0);
     if(~a)
         jvxJvxHost.jvx_display_error(16, mfilename, b.DESCRIPTION_STRING, errorsAsWarnings); 
     end
     
    % Wait for completion
     [a b] = handles.hostcall('wait_completion_process', 0);
     if(~a)
         jvxJvxHost.jvx_display_error(17, mfilename, b.DESCRIPTION_STRING, errorsAsWarnings); 
     end
     
    % Callback for project specific actions
     if(isfield(handles, 'local'))
         if(isa(handles.local.cb_stop, 'function_handle'))
             try
                 handles.local.private = handles.local.cb_stop(handles.local.private);
             catch ME
                 a = functions(handles.local.cb_start);
                 warning(['Function <' a.function '> failed, reason: ' ME.message]);
             end
         end
     end

     set(handles.checkbox_force, 'Visible', 'off');
     % Not needed
     %[a b] = handles.hostcall('status_process_sequence');
     %if(~a)
     %end