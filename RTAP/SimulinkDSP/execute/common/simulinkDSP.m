% ========================================================================
% Function to prepare the ADSP by calling specific functions over RS232
% link, then simulate the model with hardware in the loop block.
% Parameter #1: input signal in [NxM] double array format, N is the number
%               of input channels which is 2 in general since the EZKIT is 
%               stereo
% Parameter #2: Name of the model to be simulated.
% Parameter #3: Port ID for serial communication, use the dspGUI2 tool to
%               find out whch port is connected (play around, the entries 
%               are numbered by 0,1,2...
% ========================================================================
function [output] = simulinkDSP(input, nmSimulinkModel, portID)

% Check the state and set DSP into initial state again
[a b] = dspDebug;
if(a)
    if(b.systemState ~= 0)
        rtProcDSP_terminateComPorts();
    end
end;

% Start the DSP to be set into debug mode
startDSPDebug(portID);

% Return the processing paratmeters of the DSP, samplerate, buffersize,
% input/output channels, dataformat
[success, str] = rtProcDSP_obtainProcessingParameters();
if(~success)
    error(str);
end

% Prepare a global struct to be available during runtime
global referenceDSP;

% Prepare input signal for simulink fromWorkspace block
referenceDSP.input.time = [0:size(input,2)-1]';
referenceDSP.input.signals.values = input';
referenceDSP.input.numberChannels = size(input,1);

% Copy the settings from DSP platform
referenceDSP.dspCfg = str;

% API  API  API  API  API  API  API  API  API  API 
% Call the start function
% API  API  API  API  API  API  API  API  API  API 
dspStartFunction();

disp(['>>> DSP DEBUG <<<: Starting simulink simulation, model ' nmSimulinkModel ' ...']);

% Execute the simulink model
mdl = nmSimulinkModel;
open_system(mdl);

% Set the simulation parameters according to input signal
% Note: Samplerate is 1 since length is equal to simulation time then
set_param(mdl, 'StartTime', num2str(0));
set_param(mdl, 'StopTime', num2str(size(input,2)));

% Execute the simulink model
simOut = sim(mdl);

% Close the simulink model
save_system(mdl);

% API  API  API  API  API  API  API  API  API  API 
% Call the stop function
% API  API  API  API  API  API  API  API  API  API 
dspStopFunction();

% Stop DSP debugger
stopDSPDebug();
