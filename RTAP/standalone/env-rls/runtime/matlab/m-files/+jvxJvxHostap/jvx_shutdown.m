function [] = jvx_shutdown(handles)

% ==============================================
% Terminate Rtproc host on close
% ==============================================

% If sequencer is running, shut it down
% Trigger shutdown of sequencer
[a b] = jvxJvxHostRt('status_process_sequence');
if(a)

    state_init_sequencer =  jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.sequence_status, 'JVX_SEQUENCER_STATUS_NONE');
    
    % Start only if seqeuncer is in neutral init state
    if(b.STATUS_INT32 ~= state_init_sequencer)
        
        [a b] = jvxJvxHostRt('trigger_stop_process');
        if(~a)
        end
        
        % Move processing forward in one step for audio
        [a b] = jvxJvxHostRt('trigger_external_process', 0);
        if(~a)
        end
        
        % Wait for completion
        [a b] = jvxJvxHostRt('wait_completion_process', 0);
        if(~a)
        end
    end
end

% ++++++++++++++++++++++++++++++++++++++++++++++

% Terminate host
[a b]= jvxJvxHostRt('terminate');

% ++++++++++++++++++++++++++++++++++++++++++++++   
% ++++++++++++++++++++++++++++++++++++++++++++++

% Clear host
clear jvxJvxHostRt;