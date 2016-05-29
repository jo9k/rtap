% Matlab file for accessing (set) property nonBlockingAsync
% Property related to entry in property description file unknown, starting in line -1
% Selection 0x1(1): Blocking
% Selection 0x2(2): Non Blocking
function set_audio_property_dev_nonBlockingAsync(in)
	global jvx_host_call_global;
	try
		[p0 p1] = jvx_host_call_global('set_property_descriptor', 'JVX_COMPONENT_AUDIO_DEVICE', 'nonBlockingAsync', (in));
		if(~p0)
			warning(['Error in set_property', p1.DESCRIPTION_STRING]);
		end
	catch ME
		p0 = functions(jvx_host_call_cb);
		warning(['Function <' p0.function '> failed, reason: ' ME.message]);
	end
