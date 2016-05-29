function [outBuffer] = dspIFFT(inBuffer)
% out = ifft(in);

% Get access to the global variable for access to global processing
% parameters/data
global referenceDSP;

% Change the matrix format for rtprocDSP first, DO NIT use inBuffer'
% since it is conjugate complex!!
inBuffer = inBuffer.';

in(1,:) = real(inBuffer);
in(2,:) = imag(inBuffer);

blockID = 3;

[outBuffer, referenceDSP] = genericProcessing(in, blockID, referenceDSP); 

% Convert to Nx1
outBuffer = outBuffer.';


