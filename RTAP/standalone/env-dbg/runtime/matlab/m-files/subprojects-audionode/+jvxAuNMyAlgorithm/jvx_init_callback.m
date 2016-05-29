function [hdl] = jvx_init_callback(hObject_parent, handle_parent)
hdl.parent_object = hObject_parent;
hdl.parent_handle = handle_parent;

% Add local processing path
addpath H:/standalone/project/jvxAuNMyAlgorithm/matlab-local;

disp('<jvx_init_callback>: Setting path to use local matlab function in project stored at H:/standalone/project/jvxAuNMyAlgorithm/matlab-local');
