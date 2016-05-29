function [out] = dspFreqReferenceFunction(in)

global referenceDSP;

blockID = 2;
str = referenceDSP.execution.ref{blockID};

% Add the weights
out = in.' .* str.weights;
out = out.';