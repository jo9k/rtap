%%erste teil!
N=512;
f=1000;
fa=8000;
t1=0:1/fa:(N-1)/fa;
signal1=cos(2*pi*f*t1);
wavwrite(signal1, 'signal1.wav');
%%zweite teil!
figure;
plot(t1,signal1);
SIGNAL1=fft(signal1);
%%dritte teil
signal2=wavread('Signal2.wav')
l=length(signal2);
t2=0:1/fa:(l-1)/fa;
figure;
specgram(signal2);
figure
plot(t2, signal2)
%%minmax
min (signal1)
max (signal1)
min (signal2)
max (signal2)
%%power of signals
p1RMS = rms(signal1)^2;
p2RMS = rms(signal2)^2;
pow2db(p2RMS);
