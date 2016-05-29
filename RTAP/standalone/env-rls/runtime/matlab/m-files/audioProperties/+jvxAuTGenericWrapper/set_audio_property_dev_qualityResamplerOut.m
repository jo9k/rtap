% Matlab file for accessing (set) property qualityResamplerOut
% Property related to entry in property description file unknown, starting in line -1
% Selection 0x1(1): 32 taps
% Selection 0x2(2): 64 taps
% Selection 0x4(4): 128 taps
function set_audio_property_dev_qualityResamplerOut(in)
	global jvx_host_call_global;
	try
		[p0 p1] = jvx_host_call_global('set_property_descriptor', 'JVX_COMPONENT_AUDIO_DEVICE', 'JVX_GENW_DEV_QRESAMPLER_OUT', (in));
		if(~p0)
			warning(['Error in set_property', p1.DESCRIPTION_STRING]);
		end
	catch ME
		p0 = functions(jvx_host_call_cb);
		warning(['Function <' p0.function '> failed, reason: ' ME.message]);
	end
