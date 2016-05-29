function [varargs, local_handles] = jvx_filter_mat_cmdline(local_handles, varargin)
    cnt = 1;
    
    local_handles.cb_init = '';
    local_handles.cb_start = '';
    local_handles.cb_stop = '';
    local_handles.cb_term = '';
    
    varargs = {};
    varsIn = varargin{1};
    varsIn = varsIn{1};
    while(1)
        
        if(cnt > size(varsIn,2))
            break;
        end
        
        tokenUsed = false;
        token = varsIn{cnt};
        cnt = cnt + 1;
        if(ischar(token))
            
            if(strcmp(token, 'mat-init-cb'))
                
                tokenUsed = true;
                if(cnt > size(varsIn,2))
                    break;
                end
                local_handles.cb_init = varsIn{cnt}
                cnt = cnt + 1;
            end
            
            if(strcmp(token, 'mat-start-cb'))
                
                tokenUsed = true;
                if(cnt > size(varsIn,2))
                    break;
                end
                local_handles.cb_start = varsIn{cnt}
                cnt = cnt + 1;
            end
            
            if(strcmp(token, 'mat-stop-cb'))
                
                tokenUsed = true;
                if(cnt > size(varsIn,2))
                    break;
                end
                local_handles.cb_stop = varsIn{cnt}
                cnt = cnt + 1;
            end
            
            if(strcmp(token, 'mat-term-cb'))
                
                tokenUsed = true;
                if(cnt > size(varsIn,2))
                    break;
                end
                local_handles.cb_term = varsIn{cnt}
                cnt = cnt + 1;
            end
            
            if(strcmp(token, 'mat-msgq-period'))
                
                tokenUsed = true;
                if(cnt > size(varsIn,2))
                    break;
                end
                local_handles.msgq_period = varsIn{cnt}
                cnt = cnt + 1;
            end
            if(~tokenUsed)
                varargs = [varargs token];
            end
        end
    end
