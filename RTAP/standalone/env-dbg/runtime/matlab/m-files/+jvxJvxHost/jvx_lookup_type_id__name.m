function [id] = jvx_lookup_type_id__name(table, name)
    id = -1;
    for(ind=1:size(table,2))
        entry = table{ind};
        if(strcmp(name, entry{1}) == 1)
            id = entry{2};
            break;
        end
    end
    
    if(id == -1)
        error(['Failed to lookup ' name ' in lookup table']);
    end