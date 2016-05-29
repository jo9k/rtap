How to use this example:
========================

1) Open the DSP project rtprocDSP.dpj located in the talkthrough DSP code directory in VisualDSP.
2) Compile the sources (F7)
3) Start processing on DSP side (F5) - you should here the input signal clearly
4) Change the focus to Matlab
5) Make sure you are in the execute - simulink - talkthrough directory.
6) call the Script doIt.
7) The script involves the hardware in the loop functionality. At the end the two signals - one 
   is the reference, one is the DSP implementation - should be ploted in one diagram (subplot #1) 
   and the difference between the two signals in another diagram (subplot #2).

All code written and copyright by Hauke krüger, IND, RWTH Aachen, 2010
