%%EXPB

signal2=wavread('Signal2.wav');
figure
plot(t2, signal2)
delay=[241, 71, 17, 157, 373, 431, 877, 907];
signal2echo=makeEcho(signal2, delay);
figure
plot(t2, signal2echo)