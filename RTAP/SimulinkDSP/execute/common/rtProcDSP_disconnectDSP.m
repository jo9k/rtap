% Function to disconnect from DSP. 
%
% Syntax: [success, structSetup] = rtProcDSP_disconnectDSP();
%
% Return value success: Boolean variable to indicate success or failure
% Return value structSetup: If first parameter indicates success, contains
%                           a struct to hold all relevant nformation,
%                           otherwise the error.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, structSetup] = rtProcDSP_disconnectDSP();

% Call function through MEX gateway and command ID 0
[success, structSetup] = dspDebug(6);