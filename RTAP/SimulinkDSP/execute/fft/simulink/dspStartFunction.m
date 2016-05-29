% Script to be called right before the simulink simulation starts.
% The input variable is the specific workspace during processing which 
% can be accessed during all functional blocks to store/save data.
% Each block in the simulink model may have specific user data and specific
% configuration parameters. The blocks are grouped into DSP remote blocks
% and reference blocks. For the sake of tranpsparency, the blockIDs should 
% match.
% function [referenceDSP] = dspStartFunction(referenceDSP )

tt = 10;
%weightingFunction = [1 ones(1,tt) zeros(1,referenceDSP.dspCfg.buffersize-1-2*tt) ones(1,tt)];
weightingFunction = ones(1,referenceDSP.dspCfg.buffersize);

[success, err] = rtProcDSP_executeRemoteFunctionDSP(3, [], weightingFunction);
if(~success)
    error(err);
end

% Edit configuration for DSP remote functioncall here
% Configuration for block ID#1
str.blockID = 1;
str.functionID = 0;
str.buffersize = referenceDSP.dspCfg.buffersize;
str.numberInputChannels = referenceDSP.dspCfg.numberChannelsIn;
str.numberOutputChannels = referenceDSP.dspCfg.numberChannelsOut;
str.weights = weightingFunction;
str.frameCounter = 0;
referenceDSP.execution.dsp{str.blockID} = str;

% Edit configuration for DSP remote functioncall here
% Configuration for block ID#2
str.blockID = 2;
str.functionID = 1;
str.buffersize = referenceDSP.dspCfg.buffersize;
str.numberInputChannels = 1;
str.numberOutputChannels = 2;
str.frameCounter = 0;
referenceDSP.execution.dsp{str.blockID} = str;

% Edit configuration for DSP remote functioncall here
% Configuration for block ID#3
str.blockID = 3;
str.functionID = 2;
str.buffersize = referenceDSP.dspCfg.buffersize;
str.numberInputChannels = 2;
str.numberOutputChannels = 1;
str.frameCounter = 0;
referenceDSP.execution.dsp{str.blockID} = str;

% Edit configuration for reference functioncalls here
% Configuration for reference block ID#1
str.blockID = 1;
str.functionID = -1;
str.buffersize = referenceDSP.dspCfg.buffersize;
str.numberInputChannels = referenceDSP.dspCfg.numberChannelsIn;
str.numberOutputChannels = referenceDSP.dspCfg.numberChannelsOut;
str.weights = weightingFunction;
str.frameCounter = 0;
referenceDSP.execution.ref{str.blockID} = str;

% Edit configuration for reference functioncalls here
% Configuration for reference block ID#2
str.blockID = 2;
str.functionID = -1;
str.buffersize = referenceDSP.dspCfg.buffersize;
str.numberInputChannels = 1;
str.numberOutputChannels = 1;
str.weights = weightingFunction;
str.frameCounter = 0;
referenceDSP.execution.ref{str.blockID} = str;

return;