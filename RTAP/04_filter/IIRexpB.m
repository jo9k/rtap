%%RESONANCE
hres=impz([1], [1.8 -0.7 0.9], 64);
fvtool(hres);

%%ALLPASS
hAP=impz([0.71 0.35 1], [1 0.35 0.71], 64);
fvtool(hAP);

%%NOTCH
hNOTCH=impz([1 -1.4142 1], [1 -1.4 .098], 64);
fvtool(hNOTCH);

sptool