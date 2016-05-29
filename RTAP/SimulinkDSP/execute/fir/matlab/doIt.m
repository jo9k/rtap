% Script to demonstrate the setting of filter coefficient from within
% Matlab. Note that the DSP is switched into the debug mode with audio
% disconnected for a short while. This behavior can be changed on the DSP
% side by using the Macro SWITCH_OFF_AUDIO_DEBUG_MODE.

addpath ../../common;

% Lod filters
load filters.mat; 

% Set the filter coefficients
%setFilterCoeffs(0, filt.impRespLP);
%setFilterCoeffs(0, filt.impRespHP);
%setFilterCoeffs(0, filt.impRespBP);
setFilterCoeffs(0, [1 zeros(1,30)]);