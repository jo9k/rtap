% Function to close the selected COM port.
%
% Syntax: [success, structSetup] = rtProcDSP_closeComPort();
%
% Return value success: Boolean variable to indicate success or failure
% Return value comPorts: If first parameter indicates success, contains
%                        a struct to hold all system parameters.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, structProcParams] = rtProcDSP_closeComPort();

% Call function through MEX gateway, command ID 3 and the COM port ID
[success, structProcParams] = dspDebug(4);
