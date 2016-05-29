%%LOWPASS FIR ANALYSIS

%%INIT
%%Lengths
N16 = 16;
N64 = 64;
hammwindow = hamming(N64);
blackwindow = blackman(N64);
%%Initiating y[n] vectors - pretty much rectangular window itself?
y16 = zeros (1, N16);
for i = 1:1:N16
    y16(i) = sin((pi/2)*(i-8.5))/((pi/2)*(i-8.5));
end
y64 = zeros (1, N64);
yhamm = zeros (1, N64);
yblack = zeros (1, N64);
for i = 1:1:N64
    y64(i) = sin((pi/2)*(i-8.5))/((pi/2)*(i-8.5));
    yhamm(i) = (sin((pi/2)*(i-8.5))/((pi/2)*(i-8.5)))*hammwindow(i);
    yblack(i) = (sin((pi/2)*(i-8.5))/((pi/2)*(i-8.5)))*blackwindow(i);
end

hd16 = dfilt.dffir(y16);
hd64 = dfilt.dffir(y64);
hdhamm = dfilt.dffir(yhamm);
hdblack = dfilt.dffir(yblack);
h1 = [hd16 hd64];
h2 = [hdhamm, hdblack]
freqz(h1);
freqz(h2);