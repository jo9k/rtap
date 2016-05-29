function [lst] = jvx_get_uitable_list_in_files(txtField, rates, lengths)
lst = {};

for(ind= 1:size(txtField, 1))
    if(ind <= size(rates,2))
        rateTxt = num2str(rates(ind));
    else
        rateTxt = '--';
    end
    
    if(ind <= size(lengths,2))
        lengthTxt = num2str(lengths(ind));
    else
        lengthTxt = '--';
    end
    oneLine = {txtField{ind}, rateTxt, lengthTxt};
   
    lst = [lst; oneLine];
end
        
