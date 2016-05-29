% Matlab file for accessing (set) property spectrumEstimationSamplerate
% Property related to entry in property description file unknown, starting in line -1
function set_audio_property_node_spectrumEstimationSamplerate(in)
	global jvx_host_call_global;
	try
		[p0 p1] = jvx_host_call_global('set_property_descriptor', 'JVX_COMPONENT_AUDIO_NODE', 'spectrumEstimationSamplerate', int32(in));
		if(~p0)
			warning(['Error in set_property', p1.DESCRIPTION_STRING]);
		end
	catch ME
		p0 = functions(jvx_host_call_cb);
		warning(['Function <' p0.function '> failed, reason: ' ME.message]);
	end
