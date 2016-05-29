function[hObject, handles] = jvx_init_ui(hObject, handles)

tbl_lahead = jvxJvxHost.jvx_create_tables('jvxPropertyLookaheadSizes');
data = {};
for(ind=1:size(tbl_lahead,2))
    data = [data ; num2str(tbl_lahead{ind})];
end

set(handles.popupmenu_input_lahead, 'String', data);
set(handles.popupmenu_input_lahead, 'Enable', 'off');

set(handles.popupmenu_output_lahead, 'String', data);
set(handles.popupmenu_output_lahead, 'Enable', 'off');


