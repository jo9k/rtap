% This file has been written and copyright by Hauke Krueger, 
% Javox Solutions GmbH, 2011.
function [entry ] = findNameID(table, id)
sz = size(table);
for(ind = 1:sz(2))
    val = cell2mat(table{ind}(2));
    if(val == id)
        entry = table{ind}(1);
        return;
    end
end
