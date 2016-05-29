function [lst] = jvx_get_uitable_list_multiple_selection(txtField, selection)
lst = {};
mask = 1;

for(ind= 1:size(txtField, 1))
    res = bitand(selection, mask);
    if(res)
           oneLine = {txtField{ind}, 'yes'};
    else
           oneLine = {txtField{ind}, 'no'};
    end
    
    lst = [lst;oneLine];
    mask = bitshift(mask, 1);
end
        
