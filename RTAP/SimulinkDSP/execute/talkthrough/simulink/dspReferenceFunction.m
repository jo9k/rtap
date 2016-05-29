% This function is an exemplary Matlab function which is referenced in a
% block in the Simulink model. In this function, a reference function is 
% called which shall be the basis for the DSP implementation which is 
% called in the remote function. 
function [ outBuffer ] = dspReferenceFunction( inBuffer )

global referenceDSP;

% Get access to the rserved block
blockID = 1;
str = referenceDSP.execution.ref{blockID};

% Copy output
outBuffer = inBuffer;

% With the current DSP, the number of output arguments is greater that the
% nuber f input arguments, therefore fill up the output
for(ind = size(outBuffer,2)+1:str.numberOutputChannels)
    outBuffer = [outBuffer zeros(size(outBuffer,1),1)];
end

% Increment frame counter
str.frameCounter = str.frameCounter + 1;

disp(['-->Reference Processing: Frame #' num2str(str.frameCounter )]);

% Write back the block struct
referenceDSP.execution.ref{blockID} = str;
