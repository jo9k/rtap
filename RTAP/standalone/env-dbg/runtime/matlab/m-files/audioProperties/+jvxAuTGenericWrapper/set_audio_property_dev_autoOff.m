% Matlab file for accessing (set) property autoOff
% Property related to entry in property description file unknown, starting in line -1
% Selection 0x1(1): Auto On
% Selection 0x2(2): Auto Off
function set_audio_property_dev_autoOff(in)
	global jvx_host_call_global;
	try
		[p0 p1] = jvx_host_call_global('set_property_descriptor', 'JVX_COMPONENT_AUDIO_DEVICE', 'JVX_GENW_AUTO_OFF', (in));
		if(~p0)
			warning(['Error in set_property', p1.DESCRIPTION_STRING]);
		end
	catch ME
		p0 = functions(jvx_host_call_cb);
		warning(['Function <' p0.function '> failed, reason: ' ME.message]);
	end
