% Matlab file for accessing (get) property levelHandlingInput
% Property related to entry in property description file unknown, starting in line -1
% Selection 0x1(1): On
% Selection 0x2(2): Off
function [out] = get_audio_property_dev_levelHandlingInput()
	out = [];
	global jvx_host_call_global;
	try
		[p0 p1] = jvx_host_call_global('get_property_descriptor', 'JVX_COMPONENT_AUDIO_DEVICE', 'JVX_GENW_ACT_ILEVEL');
		if(~p0)
			warning(['Error in get_property', p1.DESCRIPTION_STRING]);
		else
			out = p1;
		end
	catch ME
		p0 = functions(jvx_host_call_cb);
		warning(['Function <' p0.function '> failed, reason: ' ME.message]);
	end
