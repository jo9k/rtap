% This function is an exemplary Matlab function which is referenced in a
% block in the Simulink model. In this function, a DSP remote function is 
% called. The implementation of this function is very generic, it need not
% be like this for prototyping.
% In the current implementation, the DSP remote function is called based on
% code generation to take into consideration possibly different buffersizes
% and number of input/output channels.
function [ outBuffer ] = dspRemoteFunction( inBuffer )

% Get access to the global variable for access to global processing
% parameters/data
global referenceDSP;

% Change the matrix format for rtprocDSP first
inBuffer = inBuffer.';

% Get access to the right block ID
blockID = 1;

[outBuffer, referenceDSP] = genericProcessing(inBuffer, blockID, referenceDSP);

% Change format to Simulink again
outBuffer = outBuffer.';