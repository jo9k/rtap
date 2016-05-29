function [ outBuffer ] = dspFunction( inBuffer )

global referenceDSP;


outBuffer = inBuffer;

referenceDSP.ref.frameCounter = referenceDSP.ref.frameCounter + 1;

 disp(['Reference Processing: Frame #' num2str(referenceDSP.ref.frameCounter)]);
