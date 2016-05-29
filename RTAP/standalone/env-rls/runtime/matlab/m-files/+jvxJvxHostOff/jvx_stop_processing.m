function [hObject, handles] = jvx_stop_processing(hObject, handles, skip_shutdown_seq)

    if(~skip_shutdown_seq)
         % Trigger shutdown of sequencer
         [a b] = handles.hostcall('trigger_stop_process');
         if(~a)
             jvxJvxHost.jvx_display_error(15, mfilename, b.DESCRIPTION_STRING); 
         end
     
        % Move processing forward in one step for audio
        [a b] = handles.hostcall('trigger_external_process', 0);
        if(~a)
             jvxJvxHost.jvx_display_error(16, mfilename, b.DESCRIPTION_STRING); 
         end
    end
    
    % Wait for completion
     [a b] = handles.hostcall('wait_completion_process', 0);
     if(~a)
         jvxJvxHost.jvx_display_error(17, mfilename, b.DESCRIPTION_STRING); 
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
     
     % Not needed
     %[a b] = handles.hostcall('status_process_sequence');
     %if(~a)
     %end