% doSimulink: Script to start simulink procesing involving
% hardware in the loop - the ADSP 21369.
% This example calls the DSP process function from extern.
% As a conclusion the same algorithm as the one used in real-time 
% processing will be used.

% Add common files to search path
addpath '../../common';


%===============================================
%  ID of COM port
%===============================================
portID = 0;


%===============================================
% Produce the input signal, we might also use a wav file here
% Simulation length
L_SIGNAL = 1000;

% Produce input signal, currently single channel
input_left = randn(1, L_SIGNAL);
input_right = randn(1, L_SIGNAL);
input = [input_left; input_right];
%===============================================


%===============================================
% Make the datatype for storage during processing global
% In all functions, data can be added as wished
global referenceDSP;
referenceDSP = [];

% Run the Simulink simulation
[processed] = simulinkDSP(input, nameModel, portID);
%===============================================

