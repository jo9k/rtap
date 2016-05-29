% Script to be called right before the simulink simulation starts.
% The input variable is the specific workspace during processing which 
% can be accessed during all functional blocks to store/save data.
% Each block in the simulink model may have specific user data and specific
% configuration parameters. The blocks are grouped into DSP remote blocks
% and reference blocks. For the sake of tranpsparency, the blockIDs should 
% match.
% function [referenceDSP] = dspStartFunction(referenceDSP )

% Load the filter coefficients
load filters.mat;

coeffs = [1 zeros(1,30)];
%coeffs = filt.impRespLP;
%coeffs = filt.impRespBP;
%coeffs = filt.impRespHP;

% Send the filter coefficients to the DSP
[success, err] = rtProcDSP_executeRemoteFunctionDSP(1, [], coeffs);
if(~success)
    error(['Error setting filter coefficients: ' err]); 
end

% Edit configuration for DSP remote functioncall here
% Configuration for block ID#1
str.blockID = 1;
str.functionID = 0;
str.buffersize = referenceDSP.dspCfg.buffersize;
str.numberInputChannels = referenceDSP.dspCfg.numberChannelsIn;
str.numberOutputChannels = referenceDSP.dspCfg.numberChannelsOut;
str.frameCounter = 0;
referenceDSP.execution.dsp{1} = str;

% Edit configuration for reference functioncalls here
% Configuration for reference block ID#1
str.blockID = 1;
str.functionID = -1;
str.buffersize = referenceDSP.dspCfg.buffersize;
str.numberInputChannels = referenceDSP.dspCfg.numberChannelsIn;
str.numberOutputChannels = referenceDSP.dspCfg.numberChannelsOut;
str.frameCounter = 0;
str.userData.coeffs = coeffs;
for(ind=1:str.numberInputChannels)
    str.userData.states{ind} = [];
end
referenceDSP.execution.ref{1} = str;

return;