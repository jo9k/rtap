function[] = jvx_set_property_single_selection_mat_id(handles, tpC, catProp, idProp, newValMatId)
    mask = bitshift(1, newValMatId-1);
     
    % Simplified property setting: only selection
    [a b]= handles.hostcall('set_property_uniqueid', tpC, catProp, idProp, mask);
     if(~a)
         % ERROR
     end
