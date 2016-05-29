% Function to be called right before the simulink simulation starts.
% The input variable is the specific workspace during processing which 
% can be accessed during all functional blocks to store/save data.
% Each block in the simulink model may have specific user data and specific
% configuration parameters. The blocks are grouped into DSP remote blocks
% and reference blocks. For the sake of tranpsparency, the blockIDs should 
% match.
% function [referenceDSP] = dspStartFunction;

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
referenceDSP.execution.ref{1} = str;
