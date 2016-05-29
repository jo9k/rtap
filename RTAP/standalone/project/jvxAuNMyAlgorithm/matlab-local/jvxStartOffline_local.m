function [ jvx_handle ] = jvxStartOffline_local( jvx_bsize_in, jvx_srate_in, jvx_num_in, jvx_format_in, jvx_bsize_out, jvx_srate_out, jvx_num_out, jvx_format_out, jvx_is_offline, precision, jvx_add_hints)

    global inProcessing;
	    
    jvx_handle.setup.in.bsize = jvx_bsize_in;
    jvx_handle.setup.in.srate = jvx_srate_in;
    jvx_handle.setup.in.num = jvx_num_in;
    jvx_handle.setup.in.format = jvx_format_in;

    jvx_handle.setup.out.bsize = jvx_bsize_out;
    jvx_handle.setup.out.srate = jvx_srate_out;
    jvx_handle.setup.out.num = jvx_num_out;
    jvx_handle.setup.out.format = jvx_format_out;

    jvx_handle.setup.prec = precision;
    
    jvx_handle.processing.offline = jvx_is_offline;

    if(jvx_srate_in ~= jvx_srate_out)
        error('Input rate must be identical to output rate');
    end
    
    if(jvx_bsize_in ~= jvx_bsize_out)
        error('Input rate must be identical to output rate');
    end
    
    if(jvx_format_in ~= jvx_format_out)
        error('Input rate must be identical to output rate');
    end
    
	% Restore private data in global data struct
	inProcessing.jvx_handle = jvx_handle;
    
    %%init algorithm
    inProcessing.algo.ANGLE = 90;
    inProcessing.algo.CFAC = 0.8;
