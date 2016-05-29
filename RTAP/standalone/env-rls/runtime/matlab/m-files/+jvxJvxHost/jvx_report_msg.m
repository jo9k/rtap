function [] = jvx_report_msg(id_msg, sub_id_msg, load)
	
    global jvxHost_rt_remote;

    table_events = jvxJvxHost.jvx_create_tables('jvxReportEventType');
    id_check_stop = jvxJvxHost.jvx_lookup_type_id__name(table_events, 'JVX_REPORT_REQUEST_TRY_TRIGGER_STOP_SEQUENCER');
    id_check_start = jvxJvxHost.jvx_lookup_type_id__name(table_events, 'JVX_REPORT_REQUEST_TRY_TRIGGER_START_SEQUENCER');
    id_check_update_ui = jvxJvxHost.jvx_lookup_type_id__name(table_events, 'JVX_REPORT_REQUEST_UPDATE_WINDOW');
    id_check_user = jvxJvxHost.jvx_lookup_type_id__name(table_events, 'JVX_REPORT_REQUEST_USER_MESSAGE');
    
    if(id_msg == id_check_stop)
        jvxHost_rt_remote.jvx_start_stop();
    end
    
    if(id_msg == id_check_start)
        jvxHost_rt_remote.jvx_start_stop();
    end

    if(id_msg == id_check_update_ui)
        jvxHost_rt_remote.jvx_update_ui();
    end
    
    if(id_msg == id_check_user)
        switch(sub_id_msg)
            % What to do here?
        end
    end
