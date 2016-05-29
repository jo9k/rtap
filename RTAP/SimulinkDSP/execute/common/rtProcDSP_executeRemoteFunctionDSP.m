% Function to call the remote debug function once. The command parameters are passed
% to determine the format of the output buffers and the input buffer(s).
% All fields are of format 1xN. The field with output buffersizes is of
% format DOUBLE or INT32, the inputBuffers are currently only accepted in
% DOUBLE format.
%
% Syntax: [success, bufferOut1, bufferOut2,...] = rtProcDSP_executeRemoteFunctionDSP(fID, sizeOutputBuffers, inputBuffer1, inputBuffer2...);
%
% bufferOut1, bufferOut2... realized as varagout
% bufferIn1, bufferIn2... realized as varagin
%
% Input parameter: fID to specify the ID of the remote function, start with
%                  ID = 0.
% Input parameter: sizeOutputBuffers to specify the lengths of the output
%                  buffers, e.g. [128 256] to indicate that two
%                  outputbuffers will be produced, one of length 128 and
%                  one of length 256.
% Input Parameter(s): inputBuffer1, inputBuffer2, ... to pass the input
%                     data buffers as varargin.
% Return value success: Boolean variable to indicate success or failure
% Return value bufferOutput1, bufferOutput2,...: If first parameter indicates 
%                           success, all outputbuffers are returned in these
%                           variables. The number of arguments must match
%                           the input parameter sizeOutputbuffers. If error
%                           during remote functioncall, bufferOut1 is the
%                           returned error, all other return values are
%                           empty.
% Developed by HK, RTPROCDSP, 04/2008
%%
function [success, varargout] = rtProcDSP_executeRemoteFunctionDSP(fID, sizeOutputBuffers, varargin);

numOutArguments = size(sizeOutputBuffers,2);
checkH = size(sizeOutputBuffers,1);

[success varargoutLoc] = dspDebug(131, fID, sizeOutputBuffers, varargin);

if(size(varargoutLoc,2) ~= (nargout-1))
    if(size(varargoutLoc,2) > (nargout-1))
        varargout = varargoutLoc(1:nargout-1);
    else
        for(ind = size(varargoutLoc,2)+1:(nargout-1))
            varargout{ind} = [];
        end
    end
else
    varargout = varargoutLoc;
end

