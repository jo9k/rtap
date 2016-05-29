function [outBuffer] = dspFFT(inBuffer)

% Get access to the global variable for access to global processing
% parameters/data
global referenceDSP;

% Change the matrix format for rtprocDSP first
inBuffer = inBuffer.';

blockID = 2;

[outBuffer, referenceDSP] = genericProcessing(inBuffer, blockID, referenceDSP); 

outBuffer = outBuffer(1,:) + i*outBuffer(2,:);

% Convert the format to Nx1
outBuffer = outBuffer.';



