% LOADDBL  read variable from file in double format
%          and store it in variable x in matlab.
% 
%    Usage: x = loaddbl('FileNameAsString.ext')
%  
        
%       Autor:    Andreas Meylahn
%      Version:  12.8.1992
%				 13.03 93  now an M-FILE
%

function x = loaddbl( name )
fid = fopen( name,'r');
x = fread( fid, 'double' );
fclose( fid );

