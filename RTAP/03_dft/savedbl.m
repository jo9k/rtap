% SAVEDBL  write variable x from matlabs workspace 
%          to file in double format.
% 
%    Usage: savedbl(x, 'FileNameAsString.ext')
%  	
%	Attention: if x is a matrix it will be saved columnwise
%              x[1][1], x[2][1], x[3][1],  ... x[1][2], x[2][2] ... 
%
        
%      Autor:    Andreas Meylahn
%      Version:  12.8.1992

function savedbl( x, name )
fid = fopen( name, 'w+');
fwrite( fid, x, 'double' );
fclose( fid );


