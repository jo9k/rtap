% Function to return all available remote debug functions.
%
% Syntax: [success, structFunctions] = rtProcDSP_enumerateRemoteFunctionsDSP();
%
% Return value success: Boolean variable to indicate success or failure
% Return value structFunctions: If first parameter indicates success, contains
%                           a struct to hold all relevant nformation,
%                           otherwise the error.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, structFunctions] = rtProcDSP_enumerateRemoteFunctionsDSP();

% Call function through MEX gateway and command ID 0
[success, structFunctions] = dspDebug(129);
