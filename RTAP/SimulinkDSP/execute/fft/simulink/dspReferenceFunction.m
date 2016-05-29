% This function is an exemplary Matlab function which is referenced in a
% block in the Simulink model. In this function, a reference function is 
% called which shall be the basis for the DSP implementation which is 
% called in the remote function. 
function [ outBuffer ] = dspReferenceFunction( inBuffer )

global referenceDSP;

blockID = 1;
str = referenceDSP.execution.ref{blockID};

% Get access to the rserved block
blockID = 1;
str = referenceDSP.execution.ref{blockID};

% Change input format
inBuffer = inBuffer.';

% Do the fft->weights->ifft processing
minChans = min(str.numberInputChannels, str.numberOutputChannels);
for(ind = 1:minChans)
   temp = fft(inBuffer(ind,:));
   temp = temp .* str.weights;
   outBuffer(ind,:) = real(ifft(temp));
end

% With the current DSP, the number of output arguments is greater that the
% nuber f input arguments, therefore fill up the output
for(ind = size(outBuffer,1)+1:str.numberOutputChannels)
    outBuffer = [outBuffer; zeros(1,size(outBuffer,2))];
end

% Increment frame counter
str.frameCounter = str.frameCounter + 1;

disp(['-->Reference Processing: Frame #' num2str(str.frameCounter )]);

% Write back the block struct
referenceDSP.execution.ref{blockID} = str;

% Change the outbuffer format again
outBuffer = outBuffer';
