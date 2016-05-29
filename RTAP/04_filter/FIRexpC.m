%%1.&2. h(k)
h1 = [1 -1];
hstir = [-1/12 8/12 0 -8/12 1/12];
figure;
impz(h1);
figure;
impz(hstir);
close('all');
%%Parks.MCClellan
f = [0 1];
a = [1 1];
b = firpm(31,f,a, 'differentiator');

%%plotting
fvtool(h1);
fvtool(hstir);
fvtool(b);