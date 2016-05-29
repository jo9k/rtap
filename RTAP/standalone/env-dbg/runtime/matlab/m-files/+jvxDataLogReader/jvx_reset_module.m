% This script has been written and copyright by Hauke Krueger, 
% Javox Solutions GmbH, 2011.
id = findIDName(tableReaderCommands, 'JVX_FIO_MX_TERMINATE');
[result1 result2] = rtpFileReaderMatlab(18);
if(~result1)
    result2.DESCRIPTION
    return;
else
    result2
end
