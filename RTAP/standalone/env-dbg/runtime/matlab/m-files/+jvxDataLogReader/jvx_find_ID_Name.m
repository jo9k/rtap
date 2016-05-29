% This file has been written and copyright by Hauke Krueger, 
% Javox Solutions GmbH, 2011.
function [entry ] = findIDName(table, name)
sz = size(table);
entry = -1;
for(ind = 1:sz(2))
    val = cell2mat(table{ind}(1));
    if(strcmp(val, name))
        entry = cell2mat(table{ind}(2));
        return;
    end
end
