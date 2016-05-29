function varargout = jvxHost_rt(varargin)
% JVXHOST_RT MATLAB code for jvxHost_rt.fig
%      JVXHOST_RT, by itself, creates a new JVXHOST_RT or raises the existing
%      singleton*.
%
%      H = JVXHOST_RT returns the handle to a new JVXHOST_RT or the handle to
%      the existing singleton*.
%
%      JVXHOST_RT('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in JVXHOST_RT.M with the given input arguments.
%
%      JVXHOST_RT('Property','Value',...) creates a new JVXHOST_RT or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before jvxHost_rt_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to jvxHost_rt_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help jvxHost_rt

% Last Modified by GUIDE v2.5 24-Nov-2015 20:38:28

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @jvxHost_rt_OpeningFcn, ...
                   'gui_OutputFcn',  @jvxHost_rt_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before jvxHost_rt is made visible.
function jvxHost_rt_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to jvxHost_rt (see VARARGIN)

% Allow remote access
global jvxHost_rt_remote;
global jvx_host_call_global;
jvxHost_rt_remote.hObject = hObject;
jvxHost_rt_remote.jvx_start_stop = @jvxHost_rt_remote_startstop;
jvxHost_rt_remote.jvx_update_ui = @jvxHost_rt_remote_updateui;

% Default variables rto be overwritten by command libne args
handles.local.msgq_period = 1;

% Set callback for all host system calls
handles.hostcall = @jvxHostRt;
jvx_host_call_global = @jvxHostRt;

% ===================================
% In case of restart, set to state in which host can be initialized
% afterwards
% ===================================
[hObject, handles] = jvxJvxHostRt.jvx_set_to_init(hObject, handles);

% ===================================
% Initialize Rtproc host
% ===================================
handles.jvx_struct.valid = false;

% Pull out Matlab specific configuration props
[varargin_filtered, handles.local] = jvxJvxHost.jvx_filter_mat_cmdline(handles.local, varargin);

% Start C host tool
[a b]= handles.hostcall('initialize', varargin_filtered);
if(~a)
    jvxJvxHost.jvx_display_error(1, mfilename, b.DESCRIPTION_STRING); 
else
    handles.jvx_struct.valid = true;
end

% Start global timer thread
[a b]= jvx_host_call_global('set_callback_msgq', 'jvxJvxHost.jvx_report_msg');

handles.tasks.msgq.timer_hdl = timer('TimerFcn',@jvxJvxHost.jvx_msg_timer_callback, ...
    'Period', handles.local.msgq_period, ...
    'ExecutionMode', 'fixedSpacing' );
start(handles.tasks.msgq.timer_hdl);

% ===================================
% Initialize the host into a stable state
% ===================================
if(handles.jvx_struct.valid == true)
    
    [hObject, handles] = jvxJvxHostRt.jvx_startup(hObject, handles);

end

% Choose default command line output for jvxHost_rt
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes jvxHost_rt wait for user response (see UIRESUME)
% uiwait(handles.figure1);

% ==============================================
% ==============================================
% ==============================================
% ==============================================


% --- Outputs from this function are returned to the command line.
function varargout = jvxHost_rt_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in popupmenu_devices.
function popupmenu_devices_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu_devices (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu_devices contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu_devices

% ==============================================
% User selection; first entry is None, this should not be of interest
% ==============================================
idSelect = get(handles.popupmenu_devices, 'Value');
idSelect = idSelect - 1; % First entry:None

% ==============================================
% Deactivate currently active component
% ==============================================
if(handles.jvx_struct.properties.device.id >= 1)
    [a b]= handles.hostcall('deactivate_component', handles.jvx_struct.devices.comp_type);
    if(~a)
        jvxJvxHost.jvx_display_error(2, mfilename, b.DESCRIPTION_STRING); 
    else
        refreshUi = true;
    end

    % Unselect
    [a b]= handles.hostcall('unselect_component', handles.jvx_struct.devices.comp_type);
    if(~a)
        jvxJvxHost.jvx_display_error(3, mfilename, b.DESCRIPTION_STRING); 
    else
        refreshUi = true;
    end
end

if(idSelect >= 1)

    % ==============================================
    % Select and activate new selection component
    % ==============================================
    [a b]= handles.hostcall('select_component', handles.jvx_struct.devices.comp_type, idSelect-1);% c notation idx
    if(~a)
        jvxJvxHost.jvx_display_error(4, mfilename, b.DESCRIPTION_STRING); 
    end

    % Activate
    [a b]= handles.hostcall('activate_component', handles.jvx_struct.devices.comp_type);
    if(~a)
        jvxJvxHost.jvx_display_error(5, mfilename, b.DESCRIPTION_STRING); 
    end
end

% ==============================================
% Obtain references to all relevant properties
% ==============================================
[hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);

% ==============================================
% Show current status
% ==============================================
[hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);

guidata(hObject, handles);

% ==============================================
% ==============================================
% ==============================================
% ==============================================


% --- Executes during object creation, after setting all properties.
function popupmenu_devices_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu_devices (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit_project_samplerate_Callback(hObject, eventdata, handles)
% hObject    handle to edit_project_samplerate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_project_samplerate as text
%        str2double(get(hObject,'String')) returns contents of edit_project_samplerate as a double
valueTxt = get(handles.edit_project_samplerate, 'String');
valueNum = str2double(valueTxt);
if(handles.jvx_struct.properties.device.id_srate>= 0)
    if(~isempty(valueNum))
        
        % ==============================================
        % Set new value
        % ==============================================
        jvxJvxHost.jvx_set_property_value(handles, handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_srate, handles.jvx_struct.properties.device.id_srate, handles.jvx_struct.properties.device.tp_srate, valueNum);
        
        % ==============================================
        % Show current status
        % ==============================================
        [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
    end
end
    
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function edit_project_samplerate_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_project_samplerate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit_project_buffersize_Callback(hObject, eventdata, handles)
% hObject    handle to edit_project_buffersize (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_project_buffersize as text
%        str2double(get(hObject,'String')) returns contents of edit_project_buffersize as a double
valueTxt = get(handles.edit_project_buffersize, 'String');
valueNum = str2double(valueTxt);
if(handles.jvx_struct.properties.device.id_bsize>= 0)
    if(~isempty(valueNum))
        % ==============================================
        % Set new value
        % ==============================================
        jvxJvxHost.jvx_set_property_value(handles, handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_bsize, handles.jvx_struct.properties.device.id_bsize, handles.jvx_struct.properties.device.tp_bsize, valueNum);
        
        % ==============================================
        % Show current status
        % ==============================================
        [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
    end
end
    
guidata(hObject, handles);

% ==============================================
% ==============================================
% ==============================================
% ==============================================
        
% --- Executes during object creation, after setting all properties.
function edit_project_buffersize_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_project_buffersize (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu_project_formats.
function popupmenu_project_formats_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu_project_formats (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu_project_formats contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu_project_formats
% ==============================================
% Setup new processing format
% ==============================================
idSelect = get(handles.popupmenu_project_formats, 'Value');
if(handles.jvx_struct.properties.device.id_formats>= 0)
    
    % ==============================================
    % Set new selection processing format
    % ==============================================
    jvxJvxHost.jvx_set_property_single_selection_mat_id(handles, handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_formats, handles.jvx_struct.properties.device.id_formats, idSelect);
  
    % ==============================================
    % Show current status
    % ==============================================
    [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
end

guidata(hObject, handles);

% ==============================================
% ==============================================
% ==============================================
% ==============================================

% --- Executes during object creation, after setting all properties.
function popupmenu_project_formats_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu_project_formats (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu_technology.
function popupmenu_technology_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu_technology (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% ==============================================
% Select new sub-technology 
% ==============================================
idSelect = get(handles.popupmenu_technology, 'Value');
idSelect = idSelect -1;

%if(handles.jvx_struct.properties.technology.id_technologies == -1)
if(handles.jvx_struct.properties.technology.id_technologies == -2)

    % ==============================================
    % Here, deactivate technology
    % ==============================================
    [a b]= handles.hostcall('deactivate_component', handles.jvx_struct.technologies.comp_type);
    if(~a)
       jvxJvxHost.jvx_display_error(6, mfilename, b.DESCRIPTION_STRING); 
    end
elseif(handles.jvx_struct.properties.technology.id_technologies>= 1)
    
    % ==============================================
    % Here, set new technology sub-id
    % ==============================================
    jvxJvxHost.jvx_set_property_single_selection_mat_id(handles, handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_technologies, handles.jvx_struct.properties.technology.id_technologies, idSelect);

    % ==============================================
    % Activate generic wrapper component
    % ==============================================
    [a b]= handles.hostcall('activate_component', handles.jvx_struct.technologies.comp_type);
    if(~a)
        jvxJvxHost.jvx_display_error(7, mfilename, b.DESCRIPTION_STRING); 
    end
end

% ==============================================
% Obtain references to all relevant properties
% ==============================================
[hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);

% ==============================================
% Show current status
% ==============================================
[hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);

guidata(hObject, handles);

% ==============================================
% ==============================================
% ==============================================
% ==============================================
% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu_technology contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu_technology


% --- Executes during object creation, after setting all properties.
function popupmenu_technology_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu_technology (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton_add_output_file.
function pushbutton_add_output_file_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_add_output_file (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)if(handles.jvx_struct.properties.technology.id_input_files >= 0)
if(handles.jvx_struct.properties.technology.id_output_files >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_output_files, handles.jvx_struct.properties.technology.id_output_files);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        [f p] = uiputfile('*.wav', 'Select output file');
        if(~isempty(f))
            
            p = jvxJvxHost.jvx_relative_path(p);
            fname = [p f];
            
            % Add new filename
            txtField = [txtField;  fname];
            selection = 2^size(txtField, 1)-1;
            exclusive = 0;
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_output_files, handles.jvx_struct.properties.technology.id_output_files, selection, exclusive, txtField);
            if(a)
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(31, mfilename, b.DESCRIPTION_STRING);
            end
        end
    else
        jvxJvxHost.jvx_display_error(30, mfilename, b.DESCRIPTION_STRING);
    end
    guidata(hObject, handles);
end

% --- Executes on button press in pushbutton_remove_output_file.
function pushbutton_remove_output_file_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_remove_output_file (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.technology.id_output_files >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_output_files, handles.jvx_struct.properties.technology.id_output_files);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        
        if((handles.jvx_struct.properties.output_files.output.id_user >= 1) && (handles.jvx_struct.properties.output_files.output.id_user <= size(txtField, 1)))
            txtField = [txtField{1:handles.jvx_struct.properties.output_files.output.id_user-1} txtField{handles.jvx_struct.properties.output_files.output.id_user+1:end}];
            
            selection = 2^size(txtField, 1)-1;
            exclusive = 0;
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_output_files, handles.jvx_struct.properties.technology.id_output_files, selection, exclusive, txtField);
            if(a)
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(33, mfilename, b.DESCRIPTION_STRING);
            end
        else
            % No selection
        end
    else
        jvxJvxHost.jvx_display_error(32, mfilename, b.DESCRIPTION_STRING);
    end
    guidata(hObject, handles);
end

% --- Executes on button press in pushbutton_add_input_file.
function pushbutton_add_input_file_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_add_input_file (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.technology.id_input_files >= 0)
    
    
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_input_files, handles.jvx_struct.properties.technology.id_input_files);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        [f p] = uigetfile('*.wav', 'Select input file');
        if(~isempty(f))
            
            p = jvxJvxHost.jvx_relative_path(p);
            fname = [p '/' f];
            
            % Add new filename
            txtField = [txtField;  fname];            
            selection = 2^size(txtField, 1)-1;
            exclusive = 0;
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_input_files, handles.jvx_struct.properties.technology.id_input_files, selection, exclusive, txtField);
            if(a)
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(35, mfilename, b.DESCRIPTION_STRING);
            end
        end
    else
        jvxJvxHost.jvx_display_error(34, mfilename, b.DESCRIPTION_STRING);
    end
end
guidata(hObject, handles);





% --- Executes on button press in pushbutton_remove_input_file.
function pushbutton_remove_input_file_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_remove_input_file (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)if(handles.jvx_struct.properties.technology.id_input_files >= 0)
    
if(handles.jvx_struct.properties.technology.id_input_files >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_input_files, handles.jvx_struct.properties.technology.id_input_files);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        
        if((handles.jvx_struct.properties.input_files.input.id_user >= 1) && (handles.jvx_struct.properties.input_files.input.id_user <= size(txtField, 1)))
            txtField = [txtField{1:handles.jvx_struct.properties.input_files.input.id_user-1} txtField{handles.jvx_struct.properties.input_files.input.id_user+1:end}];

            selection = 2^size(txtField, 1)-1;
            exclusive = 0;
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.technologies.comp_type, handles.jvx_struct.properties.technology.cat_input_files, handles.jvx_struct.properties.technology.id_input_files, selection, exclusive, txtField);
            if(a)
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(37, mfilename, b.DESCRIPTION_STRING);
            end
        else
            % No selection
        end
    else
        jvxJvxHost.jvx_display_error(36, mfilename, b.DESCRIPTION_STRING);
    end
end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_all_unselect_out_channels.
function pushbutton_all_unselect_out_channels_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_all_unselect_out_channels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_out_channels >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        selection = size(txtField, 2);
        selection = 0;
        [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels, selection);
        if(a)
            % Show current status
            [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
        else
            jvxJvxHost.jvx_display_error(29, mfilename, b.DESCRIPTION_STRING);
        end
    else
        jvxJvxHost.jvx_display_error(28, mfilename, b.DESCRIPTION_STRING);
    end
end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_all_select_out_channels.
function pushbutton_all_select_out_channels_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_all_select_out_channels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_out_channels >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        selection = size(txtField, 2);
        selection = 2^selection-1;
        [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels, selection);
        if(a)
            % Show current status
            [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
        else
            jvxJvxHost.jvx_display_error(27, mfilename, b.DESCRIPTION_STRING);
        end
    else
        jvxJvxHost.jvx_display_error(26, mfilename, b.DESCRIPTION_STRING);
    end
end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_all_unselect_in_channels.
function pushbutton_all_unselect_in_channels_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_all_unselect_in_channels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_in_channels >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        selection = size(txtField, 2);
        selection = 0;
        [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels, selection);
        if(a)
            % Show current status
            [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
        else
            jvxJvxHost.jvx_display_error(25, mfilename, b.DESCRIPTION_STRING);
        end
    else
        jvxJvxHost.jvx_display_error(24, mfilename, b.DESCRIPTION_STRING);
    end
end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_all_select_in_channels.
function pushbutton_all_select_in_channels_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_all_select_in_channels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(jvxJvxHost.jvx_ready_for_start(handles))
    if(handles.jvx_struct.properties.device.id_in_channels >= 0)
        [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels);
        if(a)
            txtField =b.SUBFIELD.OPTIONS;
            selection = size(txtField, 2);
            selection = 2^selection -1;
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels, selection);
            if(a)
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(23, mfilename, b.DESCRIPTION_STRING);
            end
        else
            jvxJvxHost.jvx_display_error(22, mfilename, b.DESCRIPTION_STRING);
        end
    end
    guidata(hObject, handles);
end
% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Shutdown message queue
stop(handles.tasks.msgq.timer_hdl);
delete(handles.tasks.msgq.timer_hdl);

jvxJvxHostRt.jvx_shutdown(handles);

% Hint: delete(hObject) closes the figure
delete(hObject);

% ==============================================
% ==============================================
% ==============================================
% ==============================================

% --- Executes on selection change in popupmenu_audionode.
function popupmenu_audionode_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu_audionode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu_audionode contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu_audionode

% ==============================================
% User selection
% ==============================================
idSelect = get(handles.popupmenu_audionode, 'Value');
idSelect = idSelect - 1; % First entry: none

if(handles.jvx_struct.properties.node.id >= 1)
    % ==============================================
    % Deactivate component
    % ==============================================
    [a b]= handles.hostcall('deactivate_component', handles.jvx_struct.nodes.comp_type);
    if(~a)
        jvxJvxHost.jvx_display_error(8, mfilename, b.DESCRIPTION_STRING); 
    end

    % ==============================================
    % Unselect node
    % ==============================================
    [a b]= handles.hostcall('unselect_component', handles.jvx_struct.nodes.comp_type);
    if(~a)
        jvxJvxHost.jvx_display_error(9, mfilename, b.DESCRIPTION_STRING); 
    end
end

if(idSelect >= 1)
    
    % ==============================================
    % Activate selected component
    % ==============================================
    [a b]= handles.hostcall('select_component', handles.jvx_struct.nodes.comp_type, idSelect-1);% c notation idx
    if(~a)
        jvxJvxHost.jvx_display_error(10, mfilename, b.DESCRIPTION_STRING); 
    end

    [a b]= handles.hostcall('activate_component', handles.jvx_struct.nodes.comp_type);
    if(~a)
         jvxJvxHost.jvx_display_error(11, mfilename, b.DESCRIPTION_STRING); 
    end
end

% Obtain references to all relevant properties
[hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);

% Show current status
[hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);

guidata(hObject, handles);

% ==============================================
% ==============================================
% ==============================================
% ==============================================


% --- Executes during object creation, after setting all properties.
function popupmenu_audionode_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu_audionode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton_startstop.
function pushbutton_startstop_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_startstop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
forceit = get(handles.checkbox_force,'Value');
[handles] = jvxHost_rt_startstop_core(hObject, handles, forceit);
 guidata(hObject, handles);


% --- Executes when selected cell(s) is changed in uitable_inchannels.
function uitable_inchannels_CellSelectionCallback(hObject, eventdata, handles)
% hObject    handle to uitable_inchannels (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) currently selecteds
% handles    structure with handles and user data (see GUIDATA)
if(~isempty(eventdata.Indices))
    rowIdx = eventdata.Indices(1);
    colIdx = eventdata.Indices(2);
    
    if(handles.jvx_struct.properties.device.id_in_channels >= 0)
        if(colIdx == 1)
            handles.jvx_struct.properties.channels.input.id_user = rowIdx;
        end
        
        if(colIdx == 2)
            handles.jvx_struct.properties.channels.input.id_user = rowIdx;
            
            if(jvxJvxHost.jvx_ready_for_start(handles))
                [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels);
                if(a)
                    idSelect =b.SUBFIELD.SELECTION_UINT64;
                    enable = 'on';
                    mask = bitshift(1, rowIdx-1);
                    idSelect = bitxor(idSelect, mask);
                    [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels, idSelect);
                    if(a)
                        % Show current status
                        [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
                    else
                        jvxJvxHost.jvx_display_error(19, mfilename, b.DESCRIPTION_STRING);
                    end
                else
                    jvxJvxHost.jvx_display_error(18, mfilename, b.DESCRIPTION_STRING);
                end
            end
        end
        
        [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
    end
    guidata(hObject, handles);
end

% --------------------------------------------------------------------
function uitable_inchannels_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to uitable_inchannels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



% --- Executes when selected cell(s) is changed in uitable_outchannels.
function uitable_outchannels_CellSelectionCallback(hObject, eventdata, handles)
% hObject    handle to uitable_outchannels (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) currently selecteds
% handles    structure with handles and user data (see GUIDATA)
if(~isempty(eventdata.Indices))
    rowIdx = eventdata.Indices(1);
    colIdx = eventdata.Indices(2);
    
    if(handles.jvx_struct.properties.device.id_out_channels >= 0)
        if(colIdx == 1)
            handles.jvx_struct.properties.channels.output.id_user = rowIdx;
        end
        
        if(colIdx == 2)
            handles.jvx_struct.properties.channels.output.id_user = rowIdx;
            if(jvxJvxHost.jvx_ready_for_start(handles))
                [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels);
                if(a)
                    idSelect =b.SUBFIELD.SELECTION_UINT64;
                    enable = 'on';
                    mask = bitshift(1, rowIdx-1);
                    idSelect = bitxor(idSelect, mask);
                    [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels, idSelect);
                    if(a)
                        % Show current status
                        [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
                    else
                        jvxJvxHost.jvx_display_error(21, mfilename, b.DESCRIPTION_STRING);
                    end
                else
                    jvxJvxHost.jvx_display_error(20, mfilename, b.DESCRIPTION_STRING);
                end
            end
        end
        [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
    end
    guidata(hObject, handles);
end


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over pushbutton_startstop.
function pushbutton_startstop_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to pushbutton_startstop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over pushbutton_all_select_in_channels.
function pushbutton_all_select_in_channels_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to pushbutton_all_select_in_channels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes when selected cell(s) is changed in uitable_infiles.
function uitable_infiles_CellSelectionCallback(hObject, eventdata, handles)
% hObject    handle to uitable_infiles (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) currently selecteds
% handles    structure with handles and user data (see GUIDATA)
if(~isempty(eventdata.Indices))
    rowIdx = eventdata.Indices(1);
    colIdx = eventdata.Indices(2);
    
    if(handles.jvx_struct.properties.technology.id_input_files >= 0)
        handles.jvx_struct.properties.input_files.input.id_user = rowIdx;
    end
end


% --- Executes on button press in pushbutton_outchannels_up.
function pushbutton_outchannels_up_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_outchannels_up (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_out_channels >= 0)
    if(handles.jvx_struct.properties.channels.output.id_user >= 2)
        [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels);
        if(a)
            txtField =b.SUBFIELD.OPTIONS;
            selection = b.SUBFIELD.SELECTION_UINT64;
            
            tmp = txtField{handles.jvx_struct.properties.channels.output.id_user-1};
            txtField{handles.jvx_struct.properties.channels.output.id_user-1} = txtField{handles.jvx_struct.properties.channels.output.id_user};
            txtField{handles.jvx_struct.properties.channels.output.id_user} = tmp;
            
            % Set zeros to bit positions
            bit_upper = bitget(selection, handles.jvx_struct.properties.channels.output.id_user-1);
            bit_lower = bitget(selection, handles.jvx_struct.properties.channels.output.id_user);
            
            selection = bitset(selection, handles.jvx_struct.properties.channels.output.id_user, bit_upper);
            selection = bitset(selection, handles.jvx_struct.properties.channels.output.id_user-1, bit_lower);
            
            exclusive = 0;
            
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels, selection, exclusive, txtField );
            if(a)
                handles.jvx_struct.properties.channels.output.id_user = handles.jvx_struct.properties.channels.output.id_user - 1;
                
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(43, mfilename, b.DESCRIPTION_STRING);
            end
        else
            jvxJvxHost.jvx_display_error(42, mfilename, b.DESCRIPTION_STRING);
        end
    end
end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_outchannels_down.
function pushbutton_outchannels_down_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_outchannels_down (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_out_channels >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        selection = b.SUBFIELD.SELECTION_UINT64;
        
        if(handles.jvx_struct.properties.channels.output.id_user < size(txtField,1))
            tmp = txtField{handles.jvx_struct.properties.channels.output.id_user+1};
            txtField{handles.jvx_struct.properties.channels.output.id_user+1} = txtField{handles.jvx_struct.properties.channels.output.id_user};
            txtField{handles.jvx_struct.properties.channels.output.id_user} = tmp;
            
            % Set zeros to bit positions
            bit_upper = bitget(selection, handles.jvx_struct.properties.channels.output.id_user+1);
            bit_lower = bitget(selection, handles.jvx_struct.properties.channels.output.id_user);
            
            selection = bitset(selection, handles.jvx_struct.properties.channels.output.id_user, bit_upper);
            selection = bitset(selection, handles.jvx_struct.properties.channels.output.id_user+1, bit_lower);
            
            exclusive = 0;
            
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_out_channels, handles.jvx_struct.properties.device.id_out_channels, selection, exclusive, txtField );
            if(a)
                handles.jvx_struct.properties.channels.output.id_user = handles.jvx_struct.properties.channels.output.id_user + 1;
                
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(43, mfilename, b.DESCRIPTION_STRING);
            end
        else
            jvxJvxHost.jvx_display_error(42, mfilename, b.DESCRIPTION_STRING);
        end
    end
end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_inchannel_up.
function pushbutton_inchannel_up_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_inchannel_up (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_in_channels >= 0)
    if(handles.jvx_struct.properties.channels.input.id_user >= 2)
        [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels);
        if(a)
            txtField =b.SUBFIELD.OPTIONS;
            selection = b.SUBFIELD.SELECTION_UINT64;
            
            tmp = txtField{handles.jvx_struct.properties.channels.input.id_user-1};
            txtField{handles.jvx_struct.properties.channels.input.id_user-1} = txtField{handles.jvx_struct.properties.channels.input.id_user};
            txtField{handles.jvx_struct.properties.channels.input.id_user} = tmp;
            
            % Set zeros to bit positions
            bit_upper = bitget(selection, handles.jvx_struct.properties.channels.input.id_user-1);
            bit_lower = bitget(selection, handles.jvx_struct.properties.channels.input.id_user);
            
            selection = bitset(selection, handles.jvx_struct.properties.channels.input.id_user, bit_upper);
            selection = bitset(selection, handles.jvx_struct.properties.channels.input.id_user-1, bit_lower);
            
            exclusive = 0;
            
            [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels, selection, exclusive, txtField );
            if(a)
                handles.jvx_struct.properties.channels.input.id_user = handles.jvx_struct.properties.channels.input.id_user - 1;
                
                % Show current status
                [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            else
                jvxJvxHost.jvx_display_error(39, mfilename, b.DESCRIPTION_STRING);
            end
        else
            jvxJvxHost.jvx_display_error(38, mfilename, b.DESCRIPTION_STRING);
        end
    end
end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_inchannel_down.
function pushbutton_inchannel_down_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_inchannel_down (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_in_channels >= 0)
    [a b]= handles.hostcall('get_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels);
    if(a)
        txtField =b.SUBFIELD.OPTIONS;
        selection = b.SUBFIELD.SELECTION_UINT64;
        if(handles.jvx_struct.properties.channels.input.id_user < size(txtField,1))
               
                tmp = txtField{handles.jvx_struct.properties.channels.input.id_user+1};
                txtField{handles.jvx_struct.properties.channels.input.id_user+1} = txtField{handles.jvx_struct.properties.channels.input.id_user};
                txtField{handles.jvx_struct.properties.channels.input.id_user} = tmp;
                
                % Set zeros to bit positions
                bit_upper = bitget(selection, handles.jvx_struct.properties.channels.input.id_user+1);
                bit_lower = bitget(selection, handles.jvx_struct.properties.channels.input.id_user);
                
                selection = bitset(selection, handles.jvx_struct.properties.channels.input.id_user, bit_upper);
                selection = bitset(selection, handles.jvx_struct.properties.channels.input.id_user+1, bit_lower);
                
                exclusive = 0;
                
                [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.devices.comp_type, handles.jvx_struct.properties.device.cat_in_channels, handles.jvx_struct.properties.device.id_in_channels, selection, exclusive, txtField );
                if(a)
                    handles.jvx_struct.properties.channels.input.id_user = handles.jvx_struct.properties.channels.input.id_user + 1;
                    
                    % Show current status
                    [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
                else
                    jvxJvxHost.jvx_display_error(41, mfilename, b.DESCRIPTION_STRING);
                end
            else
                jvxJvxHost.jvx_display_error(40, mfilename, b.DESCRIPTION_STRING);
            end
        end
    end
guidata(hObject, handles);

% --- Executes on button press in pushbutton_in_channels_direct.
function pushbutton_in_channels_direct_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_in_channels_direct (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if(handles.jvx_struct.properties.device.id_in_channels >= 0)
    if(size(handles.jvx_struct.properties.device.local.directIn,2) > 0)
        if(handles.jvx_struct.properties.channels.input.id_user > 0)
            if(handles.jvx_struct.properties.channels.input.id_user <= size(handles.jvx_struct.properties.device.local.directIn,2))
                if(handles.jvx_struct.properties.device.local.directIn(handles.jvx_struct.properties.channels.input.id_user) ~= 0)
                    handles.jvx_struct.properties.device.local.directIn(handles.jvx_struct.properties.channels.input.id_user) = 0;
                else
                    handles.jvx_struct.properties.device.local.directIn(handles.jvx_struct.properties.channels.input.id_user) = 1;
                end
            end
            
             [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
                     
             if(~jvxJvxHost.jvx_ready_for_start(handles))
                 % Set gain etc. in device
                 [hObject, handles] = jvxJvxHostRt.jvx_set_gains(hObject, handles);
             end
             
            guidata(hObject, handles);
        end
    end
end
guidata(hObject, handles);


% --- Executes on slider movement.
function slider_level_in_Callback(hObject, eventdata, handles)
% hObject    handle to slider_level_in (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
% HK: Setup gain control
if(size(handles.jvx_struct.properties.device.local.gain_in,2) > 0)
    value = get(hObject,'Value');
    value = value * 100.0;
    valuedB = value - 50;
    if(valuedB == 0)
        valueLin = 0.0;
    else
        valueLin = 10^(valuedB/20);
    end
    
    if(handles.jvx_struct.properties.channels.input.all)
        handles.jvx_struct.properties.device.local.gain_in = handles.jvx_struct.properties.device.local.gain_in * 0.0 + 1.0;
        handles.jvx_struct.properties.device.local.gain_in = handles.jvx_struct.properties.device.local.gain_in * valueLin;
    else
        if(handles.jvx_struct.properties.channels.input.id_user <= size(handles.jvx_struct.properties.device.local.gain_in,2))
            handles.jvx_struct.properties.device.local.gain_in(handles.jvx_struct.properties.channels.input.id_user) = valueLin;
        end
    end
    
    % Show current status
    [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
    
    if(~jvxJvxHost.jvx_ready_for_start(handles))
        % Set gain etc. in device
        [hObject, handles] = jvxJvxHostRt.jvx_set_gains(hObject, handles);
    end
    
    guidata(hObject, handles);
end


% --- Executes during object creation, after setting all properties.
function slider_level_in_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider_level_in (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on button press in checkbox_in_lev_all.
function checkbox_in_lev_all_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox_in_lev_all (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox_in_lev_all
 stat = get(hObject,'Value');
 handles.jvx_struct.properties.channels.input.all = stat;
 
 % Show current status
 [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
   
 guidata(hObject, handles);


% --- Executes on slider movement.
function slider_level_out_Callback(hObject, eventdata, handles)
% hObject    handle to slider_level_out (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
if(size(handles.jvx_struct.properties.device.local.gain_out,2) > 0)
    value = get(hObject,'Value');
    value = value * 100.0;
    valuedB = value - 50;
    if(valuedB == 0)
        valueLin = 0.0;
    else
        valueLin = 10^(valuedB/20);
    end
    
    if(handles.jvx_struct.properties.channels.output.all)
        handles.jvx_struct.properties.device.local.gain_out = handles.jvx_struct.properties.device.local.gain_out * 0.0 + 1.0;
        handles.jvx_struct.properties.device.local.gain_out = handles.jvx_struct.properties.device.local.gain_out * valueLin;
    else
        if(handles.jvx_struct.properties.channels.output.id_user <= size(handles.jvx_struct.properties.device.local.gain_out,2))
            handles.jvx_struct.properties.device.local.gain_out(handles.jvx_struct.properties.channels.output.id_user) = valueLin;
        end
    end
    
    % Show current status
    [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
    
    if(~jvxJvxHost.jvx_ready_for_start(handles))
        % Set gain etc. in device
        [hObject, handles] = jvxJvxHostRt.jvx_set_gains(hObject, handles);
    end
    guidata(hObject, handles);
end
 
% --- Executes during object creation, after setting all properties.
function slider_level_out_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider_level_out (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on button press in checkbox_out_lev_all.
function checkbox_out_lev_all_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox_out_lev_all (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox_out_lev_all
 stat = get(hObject,'Value');
 handles.jvx_struct.properties.channels.output.all = stat;
 
 % Show current status
 [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
   
 guidata(hObject, handles);


% --- Executes on button press in pushbutton_save.
function pushbutton_save_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[a b] = handles.hostcall('write_config_file');
if(~a)
end



function edit_start_cb_Callback(hObject, eventdata, handles)
% hObject    handle to edit_start_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_start_cb as text
%        str2double(get(hObject,'String')) returns contents of edit_start_cb as a double
newName = get(hObject,'String');
if(handles.jvx_struct.properties.node.offline_node.id_start_function)
    
    [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_start_function, handles.jvx_struct.properties.node.offline_node.id_start_function, newName);
    if(~a)
        error('Unexpected');
    end

   % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostOff.jvx_refresh_props(hObject, handles);
    
    % Show current status
    [hObject, handles] = jvxJvxHostOff.jvx_update_ui(hObject, handles);
    
    guidata(hObject, handles);
end

% --- Executes during object creation, after setting all properties.
function edit_start_cb_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_start_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit_stop_cb_Callback(hObject, eventdata, handles)
% hObject    handle to edit_stop_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_stop_cb as text
%        str2double(get(hObject,'String')) returns contents of edit_stop_cb as a double
newName = get(hObject,'String');
if(handles.jvx_struct.properties.node.offline_node.id_stop_function)
    
    [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_stop_function, handles.jvx_struct.properties.node.offline_node.id_stop_function, newName);
    if(~a)
        error('Unexpected');
    end

   % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostOff.jvx_refresh_props(hObject, handles);
    
    % Show current status
    [hObject, handles] = jvxJvxHostOff.jvx_update_ui(hObject, handles);
    
    guidata(hObject, handles);
end

% --- Executes during object creation, after setting all properties.
function edit_stop_cb_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_stop_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit_process_cb_Callback(hObject, eventdata, handles)
% hObject    handle to edit_process_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_process_cb as text
%        str2double(get(hObject,'String')) returns contents of edit_process_cb as a double
newName = get(hObject,'String');
if(handles.jvx_struct.properties.node.offline_node.id_process_function)
    
    [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_process_function, handles.jvx_struct.properties.node.offline_node.id_process_function, newName);
    if(~a)
        error('Unexpected');
    end

   % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostOff.jvx_refresh_props(hObject, handles);
    
    % Show current status
    [hObject, handles] = jvxJvxHostOff.jvx_update_ui(hObject, handles);
    
    guidata(hObject, handles);
end

% --- Executes during object creation, after setting all properties.
function edit_process_cb_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_process_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit_num_buffers_Callback(hObject, eventdata, handles)
% hObject    handle to edit_num_buffers (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_num_buffers as text
%        str2double(get(hObject,'String')) returns contents of edit_num_buffers as a double
num = str2double(get(hObject,'String'));
if(handles.jvx_struct.properties.node.offline_node.id_num_buffers >= 0)
    
    [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_num_buffers, handles.jvx_struct.properties.node.offline_node.id_num_buffers, int32(num));
    if(~a)
        error('Unexpected');
    end
 % Show current status
 [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
   
 guidata(hObject, handles);
end

% --- Executes during object creation, after setting all properties.
function edit_num_buffers_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_num_buffers (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit_lost_Callback(hObject, eventdata, handles)
% hObject    handle to edit_lost (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_lost as text
%        str2double(get(hObject,'String')) returns contents of edit_lost as a double


% --- Executes during object creation, after setting all properties.
function edit_lost_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_lost (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu_subproj.
function popupmenu_subproj_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu_subproj (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu_subproj contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu_subproj
idSel = get(hObject,'Value');
if(handles.jvx_struct.properties.node.offline_node.id_subproject_name >= 0)
    if(idSel == 1)
        nm = '';
    else
        nm = handles.jvx_struct.subprojects{idSel-1};
    end
    
     [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_subproject_name, handles.jvx_struct.properties.node.offline_node.id_subproject_name, nm);
     if(~a)
        error('Unexpected');
     end
    
        % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);
    
    % Show current status
    [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
    
    guidata(hObject, handles);
end

% --- Executes during object creation, after setting all properties.
function popupmenu_subproj_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu_subproj (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on button press in checkbox_force.
function checkbox_force_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox_force (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox_force



function edit_b4_process_cb_Callback(hObject, eventdata, handles)
% hObject    handle to edit_b4_process_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_b4_process_cb as text
%        str2double(get(hObject,'String')) returns contents of edit_b4_process_cb as a double
newName = get(hObject,'String');
if(handles.jvx_struct.properties.node.offline_node.id_b4_process_function)
    
    [a b]= handles.hostcall('set_property_uniqueid', handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_b4_process_function, handles.jvx_struct.properties.node.offline_node.id_b4_process_function, newName);
    if(~a)
        error('Unexpected');
    end

   % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);
    
    % Show current status
    [hObject, handles] = jvxJvxHostTRt.jvx_update_ui(hObject, handles);
    
    guidata(hObject, handles);
end


% --- Executes during object creation, after setting all properties.
function edit_b4_process_cb_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_b4_process_cb (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% ======================================================
% ======================================================
% Some remote access functions
function [res] = jvxHost_rt_remote_startstop(forceit)
    res = true;
    if(nargin == 0)
        forceit = false;
    end
    global jvxHost_rt_remote;
    handles = guidata(jvxHost_rt_remote.hObject);
    [handles]  =  jvxHost_rt_startstop_core(jvxHost_rt_remote.hObject, handles, forceit);
    guidata(jvxHost_rt_remote.hObject, handles);

    % Some remote access functions
function [res] = jvxHost_rt_remote_updateui()
    res = true;
    global jvxHost_rt_remote;
    handles = guidata(jvxHost_rt_remote.hObject);
    [handles]  =  jvxHost_rt_updateui_core(jvxHost_rt_remote.hObject, handles, forceit);
    guidata(jvxHost_rt_remote.hObject, handles);

 % ===================================================
 % ===================================================
 % The core functions
% ==========
function [handles] = jvxHost_rt_startstop_core(hObject, handles, forceit)

if(forceit)
        global inProcessing;
        if(isfield(inProcessing, 'inLoop'))
            inProcessing.inLoop = false;
        end
        [hObject, handles] = jvxJvxHostRt.jvx_stop_processing(hObject, handles);
else
    if(jvxJvxHost.jvx_ready_for_start(handles))
        
        [hObject, handles] = jvxJvxHostRt.jvx_start_processing(hObject, handles);
        
        if(handles.jvx_struct.properties.node.feature_hookup_audio_node)
            
            % Obtain references to all relevant properties
            [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);
            
            % Show current status
            [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
            
            nmCallObject =  'CjvxAudioNode_mexCall';
            nmCallFunction = 'triggerNextBatch';
            
            if(handles.jvx_struct.properties.node.offline_node.id_hookup_object_name)
                [a b]= handles.hostcall('get_property_uniqueid',  handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_hookup_object_name, handles.jvx_struct.properties.node.offline_node.id_hookup_object_name);
                if(a)
                    nmCallObject = b.SUBFIELD;
                end
            end
            
            if(handles.jvx_struct.properties.node.offline_node.id_hookup_function_name)
                [a b]= handles.hostcall('get_property_uniqueid',  handles.jvx_struct.nodes.comp_type, handles.jvx_struct.properties.node.offline_node.cat_hookup_function_name, handles.jvx_struct.properties.node.offline_node.id_hookup_function_name);
                if(a)
                    nmCallFunction = b.SUBFIELD;
                end
            end
            
            % Update handles before entering main loop
            guidata(hObject, handles);
            
            global inProcessing;
            inProcessing.inLoop = true;
            while(inProcessing.inLoop)
                [res, err] = handles.hostcall('external_call', nmCallObject, nmCallFunction);
                drawnow;
            end
            
            [hObject, handles] = jvxJvxHostRt.jvx_stop_processing(hObject, handles);
        end
    else
        
        if(handles.jvx_struct.properties.node.feature_hookup_audio_node)
            global inProcessing;
            inProcessing.inLoop = false;
        else
            [hObject, handles] = jvxJvxHostRt.jvx_stop_processing(hObject, handles);
        end
        
    end
end

% Obtain references to all relevant properties
[hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);

% Show current status
[hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
 

%%
function [handles] = jvxHost_rt_updateui_core(hObject, handles)

    % Obtain references to all relevant properties
    [hObject, handles] = jvxJvxHostRt.jvx_refresh_props(hObject, handles);

    % Show current status
    [hObject, handles] = jvxJvxHostRt.jvx_update_ui(hObject, handles);
