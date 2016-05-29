% Function to find the state of a component which is not JVX_STATE_NONE.
% If the first component has been found, the state and the component id
% is returned. If desired, the component feature class property is evaluated
% and returned.
function [idState, idCompMat, foundSpecific] = jvx_find_state_components(handles, lst, feature_class_bitset)
    
id_check_none = jvxJvxHost.jvx_lookup_type_id__name(handles.jvx_struct.tables.states, 'JVX_STATE_NONE');
idState = id_check_none;
idCompMat = -1;
if(nargout == 3)
    foundSpecific = false;
end

tt = struct2cell(lst);
for(ind = 1:size(tt,1))
    if(tt{ind}.STATE_INT32 ~= id_check_none)
        idState = tt{ind}.STATE_INT32;
        idCompMat = ind;
        if((nargin == 3) & (nargout == 3))
            if(bitand(tt{ind}.FEATURE_CLASS_UINT64, feature_class_bitset))
                foundSpecific = true;
            end      
        end
        break;
    end
end
     