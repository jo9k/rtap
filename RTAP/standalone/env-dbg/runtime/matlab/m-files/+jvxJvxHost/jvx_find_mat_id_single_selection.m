function [int_id_mat] = jvx_find_mat_id_single_selection(selection)
    int_id_mat = 0;    
    mask = 1;
    for(ind = 1:64)
        res = bitand(selection, mask);
        if(res)
            int_id_mat = ind;
            break;
        end
        mask = bitshift(mask, 1);
    end
