function [mat_id] = jvx_format_option_2_idx(oneOption, compiledForDouble)

table = jvxJvxHost.jvx_create_tables('jvxDataFormat_short', compiledForDouble);

mat_id = -1;
for(ind = 1: size(table, 2))
    oneEntry = table{ind};
    if(strcmp(oneEntry{1}, oneOption))
        mat_id = oneEntry{2};
        break;
    end
end
