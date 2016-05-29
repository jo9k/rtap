% Script to be called right after the end of the simulink simulation.
% The input variable is the specific workspace during processing which 
% can be accessed during all functional blocks to store/save data.
% Each block in the simulink model may have specific user data and specific
% configuration parameters. The blocks are grouped into DSP remote blocks
% and reference blocks. 

% Place your code here

% Post proces the processed results by reading the values from the
% toWorkspace target variable. Who the f... invented the Simulink
% output dataformat. We need this very complex processing to get 
% access to the produced audio channels
output_dsp_full = simulate_output_dsp_full.signals.values';
output_ref_full = simulate_output_ref_full.signals.values';
output_dsp_seg = simulate_output_dsp_seg.signals.values';
output_ref_seg = simulate_output_ref_seg.signals.values';

referenceDSP.output.dsp_full.signals.values = output_dsp_full;
referenceDSP.output.dsp_seg.signals.values = output_dsp_seg;
referenceDSP.output.ref_full.signals.values = output_ref_full;
referenceDSP.output.ref_seg.signals.values = output_ref_seg;

% Associate output signal
output.dsp_full = referenceDSP.output.dsp_full.signals.values;
output.dsp_seg = referenceDSP.output.dsp_seg.signals.values;
output.ref_full = referenceDSP.output.ref_full.signals.values;
output.ref_seg = referenceDSP.output.ref_seg.signals.values;

%===============================================
% Evaluate the output:
% 1) Plot output signal originating from reference (MATLAB) AND DSP
%    hardware
% 2) Plot difference between the two signals
figure;
subplot(2,1,1);
hold on; plot(output.dsp_full(1,:));plot(output.ref_full(1,:), 'g-.');
legend({'Output DSP - full', 'Output Ref - full'});

subplot(2,1,2);
plot(abs(output.dsp_full(1,:)-output.ref_full(1,:)));


figure;
subplot(2,1,1);
hold on; plot(output.dsp_seg(1,:));plot(output.ref_seg(1,:), 'g-.');
legend({'Output DSP - seg', 'Output Ref - seg'});

subplot(2,1,2);
plot(abs(output.dsp_seg(1,:)-output.ref_seg(1,:)));

%===============================================