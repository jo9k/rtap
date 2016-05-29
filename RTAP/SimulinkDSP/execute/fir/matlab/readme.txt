How to use this example:
========================

1) Open the DSP project rtprocDSP.dpj located in the fir DSP code directory in VisualDSP.
2) In userDef.h, deactivate the macro SWITCH_OFF_AUDIO_DEBUG_MODE.
3) Compile the sources (F7)
4) Start processing on DSP side (F5) - you should here the input signal clearly
5) Change the focus to Matlab
6) Make sure you are in the execute - matlab - fir directory.
7) call the Script doIt.
8) The script loads a set of filter coefficients and transmitts those to the DSP. The 
   filter has a fixed length of 31 (order of 30).
9) The filter charactersitic may be changed to LP, BP, HP or nothing. In order to do so, edit the doIt Script!

All code written and copyright by Hauke krüger, IND, RWTH Aachen, 2010
