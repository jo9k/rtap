% Function to switch DSP from Debug into normal mode. No remote function calling is
% possible in normal mode.
%
% Syntax: [success, structSetup] = rtProcDSP_leaveDebugModeDSP();
%
% Return value success: Boolean variable to indicate success or failure
% Return value structSetup: If first parameter indicates success, contains
%                           a struct to hold all relevant nformation,
%                           otherwise the error.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, structSetup] = rtProcDSP_leaveDebugModeDSP();

% Call function through MEX gateway and command ID 0
[success, structSetup] = dspDebug(130);
