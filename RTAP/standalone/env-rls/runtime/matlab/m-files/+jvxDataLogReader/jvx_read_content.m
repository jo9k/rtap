% The intention of this function is to demonstrate the usage of the file
% writer and reader software. 
% You may take this function as an example on how to use the filereader
% MATLAB front-end to read out logfiles that have been recorded previously.
% The propper sequence of accessing the rtpFileReaderMatlab functional gateway
% is demonstrated in order to read the data for all stored datasets.
% This data is collected in an array which is afterwards returned to the 
% application calling this function.
% The file that is scanned for audio data is passed as single input argument.
% If empty, the user may choose a file. 
% 
% In most cases, the datasize of the stored buffers is constant. If that is
% not the case, the buffers must be collected manually.
% If the size is not constant, the buffer to collect all data in MATLAB 
% must be filled in a different way (in = [in samples];). 
% In the buffersize is not constant, processing will take significantly 
% longer.
%
% If the size is constant, the buffer can be allocated first and filled
% afterwards which is significantly faster. 
% This script has been written and copyright by Hauke Krueger, 
% Javox Solutions GmbH, 2011.
function [data] = rtpReadFileContent(fName, numberReadMax)

% Set default return type in case there is no stored data
data = {};

if(~exist(fName))
    [a b] = uigetfile('*.*', 'Open RTProc log file');
    fName = [b '/' a];
end

if(isempty(fName ))
    [a b] = uigetfile('*.*', 'Open RTProc log file');
    fName = [b '/' a];
end

if(isempty(fName))
    data = [];
end
   
if(nargin == 1)
    numberReadMax = -1;
end

% Read in constants for module control
rtpFilereaderTables;

% Start reading
% ===============================================================
% Flag to activate/deactivate fast access to the file from MATLAB
%================================================================
useFastReading = true;
%useFastReading = false;
%================================================================

%=======================================================
% Start script with a text message
disp('Matlab function to read logged input from files.');
disp('Developed by Hauke Kruger, Javox Solutions GmbH ');
disp('============================================');

%=======================================================
% The rtpFileReaderMatlab functional gateway has its own 
% documentation that can be printed by calling it without any argument
disp('Using rtpFileReaderMatlab-entry function: Usage:');
rtpFileReaderMatlab

%=======================================================
% Start the fileReader by initializing the fileReader module
disp('Step1: Initialize the filereaderMatlab module:');
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_INITIALIZE');
[result1 result2] = rtpFileReaderMatlab(id);
if(~result1)
    result2.DESCRIPTION
    rtpResetModule;
    return;
else
    result2
end

%=======================================================
% Open the file for data reading, the name is fixed for this example
disp(['Step2: Open file ' fName]);
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_OPENFILE')
[result1 result2] = rtpFileReaderMatlab(id, fName);
if(~result1)
    result2.DESCRIPTION
    rtpResetModule;
    return;
else
    result2
end

%=======================================================
% Scan the input file for sessions and datasets. The example contains only
% one session with 4 datasets.
% This function can be considered as: obtain the table of contents
disp('Step3: Scan for sessions');
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_SCANSESSIONS')
[result1 result2] = rtpFileReaderMatlab(id);
if(~result1)
    result2
    rtpResetModule;
    return;
else
    result2
end

%=======================================================
% Function to return the number of sessions in the current input file
disp('Step4: Return the number of sessions:');
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_RETURN_NUMBER_SESSIONS')
[result1 numberSessions] = rtpFileReaderMatlab(id);
if(~result1)
    numberSessions.DESCRIPTION
    rtpResetModule;
    return;
else
    disp(['Number sessions:' int2str(double(numberSessions))]);
end

%=======================================================
% Inner loop: For each of the sessions...
for(ind = 0:(double(numberSessions)-1))
    
    %=======================================================
    % for the current session, obtain the timing divisor
    disp(['For session ID=' int2str(double(ind)) ', obtain timing divisor']);
    id = findIDName(tableReaderCommands, 'JVX_FIO_MX_SELECT_TIMING_DIVISOR_SESSION');
    [result1 timingDivisor] = rtpFileReaderMatlab(id, ind);
    if(~result1)
        timingDivisor.DESCRIPTION
        rtpResetModule;
        return;
    else
        disp(['Timing divisor:' int2str(double(double(timingDivisor)))]);
    end
    
    %=======================================================
    % Obtain the number of datasets (channels for displaying
    disp(['For session ID=' int2str(double(ind)) ', obtain number datasets']);
    id = findIDName(tableReaderCommands, 'JVX_FIO_MX_RETURN_NUMBER_DATASETS_SESSION');
    [result1 numberDatasets] = rtpFileReaderMatlab(id, ind);
    if(~result1)
        numberDatasets.DESCRIPTION
        rtpResetModule;
        return;
    else
        disp(['Number datasets:' int2str(double(numberDatasets))]);
    end
    
    %=======================================================
    % For the complete session, obtain the number of datachunks for ALL
    % datasets
    disp(['For session ID=' int2str(double(ind)) ', obtain number datachunks']);
    id = findIDName(tableReaderCommands, 'JVX_FIO_MX_RETURN_NUMBER_DATACHUNKS_SESSION')
    [result1 numberChunks] = rtpFileReaderMatlab(id, ind);
    if(~result1)
        numberChunks.DESCRIPTION
        rtpResetModule;
        return;
    else
        disp(['Number datachunks:' int2str(double(numberChunks))]);
    end
    
    dataSession = [];
    %=======================================================
    % Now aqcuire the data for each dataset channel...
    for(ind2 = 0:double(numberDatasets)-1)
        disp(['For session ID=' int2str(double(ind)) ', for dataset ID=' int2str(double(ind2)) ', obtain dataset struct']);
        id = findIDName(tableReaderCommands, 'JVX_FIO_MX_RETURN_DATASET_SESSION');
        [result1 datasetStruct] = rtpFileReaderMatlab(id, ind, ind2);
        if(~result1)
            datasetStruct.DESCRIPTION
            rtpResetModule;
            return;
        else
            datasetStruct
        end

        dataSession{ind2+1}.object = datasetStruct;
        
        %=======================================================
        % Create generic name
        if(~useFastReading)
            dataSession{ind2+1}.propertyRead = 'slow';
        else
            dataSession{ind2+1}.propertyRead = 'fast';
        end
        
        %=======================================================
        % For each dataset, obtain the number of dataset specific
        % datachunks
        disp(['For session ID=' int2str(double(ind)) ', for dataset ID=' int2str(double(ind2)) ', obtain number datachunk']);
        id = findIDName(tableReaderCommands, 'JVX_FIO_MX_RETURN_NUMBER_DATACHUNKS_SESSION_DATASET');
        [result1 numChunksDataset] = rtpFileReaderMatlab(id, ind, ind2);
        if(~result1)
            numChunksDataset.DESCRIPTION
            rtpResetModule;
            return;
        else
            numChunksDataset
        end

        % Copy the complete data with one function call for speed up
        disp('Load data at once..');
        id = findIDName(tableReaderCommands, 'JVX_FIO_MX_RETURN_NEXT_DATACHUNK_SESSION_DATASET_COMPLETE_FIELD_DATA');

        numberReadValues = numChunksDataset;
        if(numberReadMax > 0)
            numberReadValues = min(numberReadValues, numberReadMax);
        end            
            
        [result1 dt] = rtpFileReaderMatlab(id, ind, ind2, numberReadValues, 0); %numChunksDataset, 
        if(~result1)
            dt.DESCRIPTION
            rtpResetModule;
            return;
        else
            dataSession{ind2+1}.data = dt;
        end
        
        disp('Rewind..');
        id = findIDName(tableReaderCommands, 'JVX_FIO_MX_REWIND_SESSION_DATASET');
        [result1 tt] = rtpFileReaderMatlab(id, ind, ind2);
        if(~result1)
            tt.DESCRIPTION
            rtpResetModule;
            return;
        end
        
        % Copy the complete timing information with one function call for speed up
        disp('Load timing info at once..');
        id = findIDName(tableReaderCommands, 'JVX_FIO_MX_RETURN_NEXT_DATACHUNK_SESSION_DATASET_COMPLETE_FIELD_DATA');
        
        numberReadValues = numChunksDataset;
        if(numberReadMax > 0)
            numberReadValues = min(numberReadValues, numberReadMax);
        end
        
        [result1 ti] = rtpFileReaderMatlab(id, ind, ind2, numberReadValues, 5);
        if(~result1)
            ti.DESCRIPTION
            rtpResetModule;
            return;
        else
            dataSession{ind2+1}.time = ti;
        end
        
        disp('===============================================');

    end
    data{ind+1}.description = ['Session #' num2str(ind)];
    data{ind+1}.data = dataSession;
end

%==============================================================
% Unscann session: opposite of scan sessions
disp('Step5: Unscan session:');
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_UNSCANSESSIONS');
[result1 result2] = rtpFileReaderMatlab(id);
if(~result1)
    result2.DESCRIPTION
    rtpResetModule;
    return;
else
    result2
end

%==============================================================
% Close the input file
disp('Step6: Close file test.out:');
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_CLOSE_FILE');
[result1 result2] = rtpFileReaderMatlab(17);
if(~result1)
    result2.DESCRIPTION
    rtpResetModule;
    return;
else
    result2
end

%==============================================================
% Terminate the module
disp('Step7: Terminate:');
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_TERMINATE');
[result1 result2] = rtpFileReaderMatlab(18);
if(~result1)
    result2.DESCRIPTION
    rtpResetModule;
    return;
else
    result2
end


