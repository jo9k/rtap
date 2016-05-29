function[hObject, handles] = initUi(hObject, handles)

columnname = {'Channel','Selection'};
columnformat = {'char','char'};
columnwidth = {150,100};
set(handles.uitable_inchannels, 'ColumnName', columnname);
set(handles.uitable_inchannels, 'ColumnFormat', columnformat);
set(handles.uitable_inchannels, 'ColumnWidth', columnwidth);

columnname = {'Channel','Selection'};
columnformat = {'char','char'};
columnwidth = {150,100};
set(handles.uitable_outchannels, 'ColumnName', columnname);
set(handles.uitable_outchannels, 'ColumnFormat', columnformat);
set(handles.uitable_outchannels, 'ColumnWidth', columnwidth);

columnname = {'File Name','Samplerate', 'Duration'};
columnformat = {'char','char','char'};
columnwidth = {75,75,75};
set(handles.uitable_infiles, 'ColumnName', columnname);
set(handles.uitable_infiles, 'ColumnFormat', columnformat);
set(handles.uitable_infiles, 'ColumnWidth', columnwidth);

columnname = {'File Name'};
columnformat = {'char'};
columnwidth = {225};
set(handles.uitable_outfiles, 'ColumnName', columnname);
set(handles.uitable_outfiles, 'ColumnFormat', columnformat);
set(handles.uitable_outfiles, 'ColumnWidth', columnwidth);

% Sub UI control
  hObject_subui = handles.subui.hdl;
  handles_subui = guidata(hObject_subui);
  [hObject_subui, handles_subui] = jvxJvxHostap.jvx_init_ui(hObject_subui, handles_subui);
  guidata(hObject_subui, handles_subui);