% Example script to demonstrate the use of rtProcDSP Matlab debugger.
% In this script, a COM port is opened to connect MATLAB to a DSP.
% This DSP lists all debug functions to be displayed in MATLAB.
% The user specifies input arguments and output variables and calls 
% the remote function on the DSP by specifying the right function ID.
%
% This script is intended to be modifed and builds the development starting
% point.
% 
% Developed by HK, RTPROCDSP, 04/2008

function [] = startDSPDebug(portID)

if(nargin == 0)
    portID = 0;
end


%% Let the system scan for available COM ports
[a, b] = rtProcDSP_initializeComPorts();
if(~a)
    error(['DSP Error: ' b]);
end

%% List all available COM ports
[a b] = rtProcDSP_enumerateComPorts();
if(~a)
    error(['DSP Error: ' b]);
end

%% Open the specified COM port
[a b] = rtProcDSP_openComPort(portID);
if(~a)
    error(['DSP Error: ' b]);
end

%% Connect to the connected DSP via selected COM port
[a b] = rtProcDSP_connectToDSP();
if(~a)
    error(['DSP Error: ' b]);
end

%% Enter DSP Debugging mode. At this time, all remote functions are
%% specified by the DSP
[a b] = rtProcDSP_enterDebugModeDSP();
if(~a)
    error(['DSP Error: ' b]);
end


