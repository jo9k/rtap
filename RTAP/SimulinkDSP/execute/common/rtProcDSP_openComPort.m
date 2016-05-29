% Function to connect to the DSP through a specific COM port.
%
% Syntax: [success, structSetup] = rtProcDSP_openComPort(idComPort);
%
% Input Parameter: idComPort: ID of the selected COM port, either DOUBLE or
%                  INT32 type, in the range of available COM ports 
%                  (start with 0!)
% Return value success: Boolean variable to indicate success or failure
% Return value comPorts: If first parameter indicates success, contains
%                        a struct to hold all system parameters.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, structSetup] = rtProcDSP_openComPort(idComPort);

% Call function through MEX gateway, command ID 3 and the COM port ID
[success, structSetup] = dspDebug(3, idComPort);
