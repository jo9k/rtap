function [hdl] = jvx_init_callback(hObject_parent, handle_parent)
hdl.parent_object = hObject_parent;
hdl.parent_handle = handle_parent;

% Add local processing path
addpath O:/jvx/audio/sources/jvxComponents/jvxAudioNodes/jvxAuNMVDRElko/matlab-local;

disp('<jvx_init_callback>: Producing beamformer input file, stored in global variable in');
try
    jvxAuNMVDRElko.jvx_produce_input_signal();
catch ME
         warning(['Function <jvxAuNMVDRElko.jvx_produce_input_signal> failed, reason: ' ME.message]);             
end
