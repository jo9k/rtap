function [data] = jvx_read_file(fName, numberReadMax)

% Set default return type in case there is no stored data
data = {};

if(nargin == 0)
    [a b] = uigetfile('*.*', 'Open Javox log file');
    fName = [b a];
else
    
    if(isempty(fName ))
        [a b] = uigetfile('*.*', 'Open Javox log file');
        fName = [b  a];
    else
    
        if(~exist(fName))
            [a b] = uigetfile('*.*', 'Open Javox log file');
            fName = [b a];
        end
    end
end

if(isempty(fName))
    data = [];
    return;
end
   
if(nargin == 1)
    numberReadMax = -1;
end

% Start reading
disp(['Start reading out content in file ' fName '.']);

%=======================================================
% Specify input filename
%=======================================================
[result1 result2] = jvxLogRead('activate', fName);
if(~result1)
    disp(result2.DESCRIPTION_STRING);
    clear jvxLogRead;
    return;
end

%=======================================================
% Scan the input file content
%=======================================================
[result1 result2] = jvxLogRead('prepare');
if(~result1)
    disp(result2.DESCRIPTION_STRING);
    jvxLogRead('deactivate');
    clear jvxLogRead;
    return;
end

%=======================================================
% Determine the number of sessions
%=======================================================
[result1 numberSessions] = jvxLogRead('number_sessions');
if(~result1)
    disp(numberSessions.DESCRIPTION_STRING);
    jvxLogRead('postprocess');
    jvxLogRead('deactivate');
    clear jvxLogRead;
    return;
end

%=======================================================
% Inner loop: For each of the sessions...
%=======================================================
for(ind = 0:(double(numberSessions)-1))
       
    %=======================================================
    % Obtain the number of datasets
    %=======================================================
    [result1 numberDatasets] = jvxLogRead('number_datasets_session', ind);
    if(~result1)
        disp(numberSessions.DESCRIPTION_STRING);
        jvxLogRead('postprocess');
        jvxLogRead('deactivate');
        clear jvxLogRead;
        return;
    end
    
    dataSession = [];

    %=======================================================
    % Now aqcuire the data for each dataset channel...
    %=======================================================
    for(ind2 = 0:double(numberDatasets)-1)
        
        %=======================================================
        % Dataset with given id..
        %=======================================================
        [result1, datasetStruct] = jvxLogRead('dataset_session', ind, ind2);
        if(~result1)
            disp(datasetStruct.DESCRIPTION_STRING);
            jvxLogRead('postprocess');
            jvxLogRead('deactivate');
            clear jvxLogRead;
            return;
        end

        dataSession{ind2+1}.object = datasetStruct;
        
        %=======================================================
        % Copy the complete data with one function call for speed up
        %=======================================================
        [result1, dt] = jvxLogRead('next_datachunk_dataset_session_complete', ind, ind2, -1, 'copy_data');
        if(~result1)
            disp(dt.DESCRIPTION_STRING);
            jvxLogRead('postprocess');
            jvxLogRead('deactivate');
            clear jvxLogRead;
            return;
        end
        
        dataSession{ind2+1}.data = dt;
        
        %=======================================================
        % Copy the timestamps
        %=======================================================
          [result1, result2] = jvxLogRead('rewind_dataset_session', ind, ind2);
        if(~result1)
            disp(result2.DESCRIPTION_STRING);
            jvxLogRead('postprocess');
            jvxLogRead('deactivate');
            clear jvxLogRead;
            return;
        end
        
        %=======================================================
        % Copy the complete entries of timestamps with one function call for speed up
        %=======================================================
        [result1, tt] = jvxLogRead('next_datachunk_dataset_session_complete', ind, ind2, -1, 'copy_timing');
        if(~result1)
            disp(tt.DESCRIPTION_STRING);
            jvxLogRead('postprocess');
            jvxLogRead('deactivate');
            clear jvxLogRead;
            return;
        end
        dataSession{ind2+1}.time_us = tt;
  
    end
    data{ind+1}.DESCRIPTION_STRING = ['Session #' num2str(ind)];
    data{ind+1}.data = dataSession;
end

%==============================================================
 jvxLogRead('postprocess');
 jvxLogRead('deactivate');
clear jvxLogRead;
