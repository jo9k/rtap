% Start RTProc host, 
% parameter 'mode':
% -> 'rt-audio': Start jvxHost in realtime-audio mode
% -> 'off-audio': Start jvxHost in offline mode
% -> everything else: start fully flexible host
function [hdlRtproc] = jvxHost(mode, varargin)

hdlRtproc = [];
if(nargin == 0)
    mode = 'rt-audio';
end

if(strcmp(mode, 'rt-audio'))
    hdlRtproc = jvxHost_rt(varargin);
elseif(strcmp(mode, 'off-audio'))
    hdlRtproc = jvxHost_off(varargin);
else
    error('Wrong start option, accepts rt-audio or off-audio.');
end
