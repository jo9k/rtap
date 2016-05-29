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

function [] = stopDSPDebug()
%% Processing is over, leave DSP debug mode again
[a b] = rtProcDSP_leaveDebugModeDSP();
if(~a)
    disp(['DSP Warning: ' b]);
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
