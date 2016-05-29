	set(LOCAL_LIBS ${LOCAL_LIBS}		
		jvxLFileIO_static
		jvxHJvx_static_nd
		jvxLthreads_static
		jvxTconfigProcessor_static
		jvxTDataConverter_static
		jvxTDataLogger_static
		jvxTResampler_static		
		jvxTrtAudioFileReader_static
		jvxTrtAudioFileWriter_static
		jvxAuTGenericWrapper_static
		qwt-${JVX_QWT_VERSION}
		qwtPolar-${JVX_QWT_POLAR_VERSION}
		jvx-dsp-base_static
		jvx-dsp-base-cpp_static
		jvx-dsp_static
		
	)

	if(JVX_USE_PART_WEBSERVER)
		set(LOCAL_LIBS ${LOCAL_LIBS}
			jvxTWebServer_static
			)
	endif()

	if(JVX_USE_PART_PAUDIO)
		set(LOCAL_LIBS ${LOCAL_LIBS} jvxAuTPortAudio_static portAudio)
	else()
		set(LOCAL_COMPILE_DEFINITIONS ${LOCAL_COMPILE_DEFINITIONS}
			JVX_AUDIO_EXCLUDE_PORTAUDIO
		)
	endif()

	if(JVX_USE_PART_RS232)
		set(LOCAL_LIBS ${LOCAL_LIBS} jvxTRs232_static)
	endif()

	if(JVX_USE_PART_ASIO)
		set(LOCAL_LIBS ${LOCAL_LIBS} jvxAuTAsio_static)
	endif()

	if(JVX_USE_PART_COREAUDIO)
		set(LOCAL_LIBS ${LOCAL_LIBS} jvxAuTCoreAudio_static ${COREAUDIO} ${COREFOUNDATION} ${AUDIOUNIT} ${COREAUDIOKIT} ${AUDIOTOOLBOX} ${CORESERVICES})
	endif()

	if(JVX_OS MATCHES "windows")
		set(LOCAL_LIBS ${LOCAL_LIBS} imm32 Setupapi jvxTThreadController_static)
	endif()

	if(JVX_USE_PART_ALSA)
		set(LOCAL_LIBS ${LOCAL_LIBS} jvxAuTAlsa_static asound)
	endif()