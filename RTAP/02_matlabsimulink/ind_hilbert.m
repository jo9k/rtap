function [h,err]=ind_hilbert(n)

% [h,err]=ind_hilbert(n)
% 
% Approximiert einen zeitdiskreten Hilberttransformator der Ordnung n.
% n sollte gerade sein, sonst wird n-1 verwendet und der Fehlerindikator
% err=1 gesetzt.

err=0;

if n/2 ~=floor(n/2),        % n ungerade?
  n=n-1;                    % => n=n-1
  err=1;
end

for k=(0:n/2-1),
  h(k+1)=1/(pi*(k-n/2))*(1-cos(pi*(k-n/2)));
end

h(n/2+1)=0;

for k=(n/2+1:n),
  h(k+1)=1/(pi*(k-n/2))*(1-cos(pi*(k-n/2)));
end