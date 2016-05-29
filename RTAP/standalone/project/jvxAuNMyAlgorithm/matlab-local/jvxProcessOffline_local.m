function [jvx_handle, jvx_out_frame] =jvxProcessOffline_local(jvx_handle, jvx_in_frame)
    
% Obtain processing handle
global inProcessing;

% Separate input signals
signal = jvx_in_frame(1,:);

%processing

fac = sind(inProcessing.algo.ANGLE);
gLeft = 1+inProcessing.algo.CFAC*fac;
gRight = 1-inProcessing.algo.CFAC*fac;
gLeft = gLeft/(1+inProcessing.algo.CFAC);
gRight = gRight/(1+inProcessing.algo.CFAC);

jvx_out_frame = [gLeft*signal; gRight*signal];

