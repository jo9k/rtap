function [] = setFilterCoeffs(portID,coeffs);

disp('===============================================================');
disp('Test run to demonstrate the debugging capabilities of rtProcDSP');
disp('===============================================================');

% Set the runtime parameters
COM_PORT_ID = portID;

% Test whether DLL can be loaded and is in uninitialized state 
[a b] = dspDebug;
if(~a)
    error(['DSP Error: ' b]);
end

%% Let the system scan for available COM ports
[a, b] = rtProcDSP_initializeComPorts();
if(~a)
    error(['DSP Error: ' b]);
end

%% Open the specified COM port
disp(['Open Com Port with ID ' num2str(COM_PORT_ID)]);
[a b] = rtProcDSP_openComPort(COM_PORT_ID);
if(~a)
    error(['DSP Error: ' b]);
end

%% Connect to the connected DSP via selected COM port
disp('Connect to DSP.');
[a b] = rtProcDSP_connectToDSP();
if(~a)
    error(['DSP Error: ' b]);
end

%% Enter DSP Debugging mode. 
% disp('Switch DSP into Debug mode.');
[a b] = rtProcDSP_enterDebugModeDSP();
if(~a)
    error(['DSP Error: ' b]);
end

[a out ] = rtProcDSP_executeRemoteFunctionDSP(1, [], coeffs);
if(~a)
    % out is varargout, copy the error description from first cell
    % element
    errTxt = out;
    error(['DSP Error: ' errTxt]);
end


%% Processing is over, leave DSP debug mode again
[a b] = rtProcDSP_leaveDebugModeDSP();
if(~a)
    error(['DSP Error: ' b]);
end

%% Disconnect the DSP again
[a b] = rtProcDSP_disconnectDSP();
if(~a)
    error(['DSP Error: ' b]);
end

%% Close the COM port
[a b] = rtProcDSP_closeComPort();
if(~a)
    error(['DSP Error: ' b]);
end

%% Set the module to initial
[a b] = rtProcDSP_terminateComPorts();
if(~a)
    error(['DSP Error: ' b]);
end

%% Safely release the debugging sub system.
clear dspDebug;
