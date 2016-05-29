function [out] = jvx_fft(in)
    ll = size(in, 2);
    num = size(in, 1);
    out = zeros(num, ll/2+1);
    
    for(ind = 1:num)
        tmp = fft(in(ind,:));
        out(ind,:) = tmp(1:ll/2+1);
    end