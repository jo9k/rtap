function[] = set_property_value(handles, tpC, catProp, idProp, tpProp, newVal)
     
id_check_64bit = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.formats, 'JVX_DATAFORMAT_64BIT_LE');
id_check_32bit = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.formats, 'JVX_DATAFORMAT_32BIT_LE');
id_check_24bit = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.formats, 'JVX_DATAFORMAT_24BIT_LE');
id_check_16bit = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.formats, 'JVX_DATAFORMAT_16BIT_LE');
id_check_8bit = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.formats, 'JVX_DATAFORMAT_8BIT');
id_check_data = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.formats, 'JVX_DATAFORMAT_DATA');

if(tpProp == id_check_64bit)
    newValFormat = int64(newVal);
elseif(tpProp == id_check_32bit)
    newValFormat = int32(newVal);
elseif(tpProp == id_check_24bit)
    newValFormat = int32(newVal);
elseif(tpProp == id_check_16bit)
    newValFormat = int16(newVal);
elseif(tpProp == id_check_8bit)
    newValFormat = int8(newVal);
elseif(tpProp == id_check_data)
	if(handles.jvx_struct.configuredForDouble)
		newValFormat = double(newVal);
	else
		newValFormat = single(newVal);
	end
end

% Simplified property setting: only selection
[a b]= handles.hostcall('set_property_uniqueid', tpC, catProp, idProp, newValFormat);
if(~a)
    % ERROR
end