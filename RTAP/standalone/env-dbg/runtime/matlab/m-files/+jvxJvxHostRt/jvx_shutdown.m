function [] = jvx_shutdown(handles)

% ==============================================
% Terminate Rtproc host on close
% ==============================================

% If sequencer is running, shut it down
% Trigger shutdown of sequencer
[a b] = handles.hostcall('status_process_sequence');
if(a)

    state_init_sequencer =  jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_status, 'JVX_SEQUENCER_STATUS_NONE');
    
    % Start only if seqeuncer is in neutral init state
    if(b.STATUS_INT32 ~= state_init_sequencer)
        
        [a b] = handles.hostcall('trigger_stop_process');
        if(~a)
        end
        
        % Move processing forward in one step for audio
        [a b] = handles.hostcall('trigger_external_process', 0);
        if(~a)
        end
        
        % Wait for completion
        [a b] = handles.hostcall('wait_completion_process', 0);
        if(~a)
        end
    end
end

% ++++++++++++++++++++++++++++++++++++++++++++++
% Shutdown sub-ui
    if(isstruct(handles))
        if(isfield(handles, 'subui'))
            if(isstruct(handles.subui))
                if(isfield(handles.subui, 'hdl'))
                    if(~isempty(handles.subui.hdl))
                        if(ishandle(handles.subui.hdl))
                            
                            % Avoid infinite recursion
                            handles_subui = guidata(handles.subui.hdl);
                            handles_subui.parent = [];
                            guidata(handles.subui.hdl, handles_subui);
                            close(handles.subui.hdl);
                        end
                    end
                end
            end
        end
    end

    % Callback for project specific actions
    % Callback for project specific actions          
     if(isfield(handles, 'local'))
         if(isa(handles.local.cb_term, 'function_handle'))
             try
                 handles.local.private = handles.local.cb_term(handles.local.private);
             catch ME
                 a = functions(handles.local.cb_term);
                 warning(['Function <' a.function '> failed, reason: ' ME.message]);
             end
         end
     end
     
% ++++++++++++++++++++++++++++++++++++++++++++++

% Terminate host
[a b]= handles.hostcall('terminate');

% ++++++++++++++++++++++++++++++++++++++++++++++   
% ++++++++++++++++++++++++++++++++++++++++++++++

% Clear host
clear jvxJvxHostRt;