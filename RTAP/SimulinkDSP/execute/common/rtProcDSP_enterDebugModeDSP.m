% Function to switch DSP into Debug mode. At this time, the names of all debug functions are 
% returned from DSP to PC to be stored, to be viewed and to be called remotely.
%
% Syntax: [success, structSetup] = rtProcDSP_enterDebugModeDSP();
%
% Return value success: Boolean variable to indicate success or failure
% Return value structSetup: If first parameter indicates success, contains
%                           a struct to hold all relevant nformation,
%                           otherwise the error.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, structSetup] = rtProcDSP_enterDebugModeDSP();

% Call function through MEX gateway and command ID 0
[success, structSetup] = dspDebug(128);
