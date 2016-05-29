% Function to initialize the system and all COM ports to connect
% the PC to the DSP.
%
% Syntax: [success, structSetup] = rtProcDSP_initializeComPorts();
%
% Return value success: Boolean variable to indicate success or failure
% Return value structSetup: If first parameter indicates success, contains
%                           a struct to hold all relevant nformation,
%                           otherwise the error.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, structSetup] = rtProcDSP_initializeComPorts();

% Call function through MEX gateway and command ID 0
[success, structSetup] = dspDebug(0);
