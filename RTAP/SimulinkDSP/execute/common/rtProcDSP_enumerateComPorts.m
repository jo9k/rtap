% Function to list all COM ports found on the system to connect
% the PC to the DSP.
%
% Syntax: [success, comPorts] = rtProcDSP_enumerateComPorts();
%
% Return value success: Boolean variable to indicate success or failure
% Return value comPorts: If first parameter indicates success, contains
%                        a cell array to enumerate all available COM ports.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, comPorts] = rtProcDSP_enumerateComPorts();

% Call function through MEX gateway and command ID 0
[success, comPorts] = dspDebug(2);
