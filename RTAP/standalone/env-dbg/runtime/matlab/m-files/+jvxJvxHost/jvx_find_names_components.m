function [lst, idLastState] = jvx_find_names_components(lstIn, stateFind)

lst = {};
idLastState = -1;

if(~isempty(lstIn))
    tt = struct2cell(lstIn);
    for(ind = 1:size(tt,1))
        lst{ind} = tt{ind}.DESCRIPTION_STRING;
        
        if(nargin == 2) && (nargout == 2)
            if(tt{ind}.STATE_INT32 >= stateFind)
                idLastState = ind;
            end
        end
    end
end