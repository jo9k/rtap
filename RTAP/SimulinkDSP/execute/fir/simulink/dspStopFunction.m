% Script to be called right after the end of the simulink simulation.
% In this function, the test run should be evaluated.
% function [referenceDSP] = dspStopFunction(referenceDSP )

% Post proces the processed results by reading the values from the
% toWorkspace target variable. Who the f... invented the Simulink
% output dataformat. We need this very complex processing to get 
% access to the produced audio channels
output_dsp_f = simulate_output_dsp.signals.values;
sz = size(output_dsp_f);
output_dsp = [];
for(ind = 1:sz(2))
    output_dsp = [output_dsp; output_dsp_f(:,ind,:)]; 
end
output_ref_f = simulate_output_ref.signals.values;
sz = size(output_ref_f);
output_ref = [];
for(ind = 1:sz(2))
    output_ref = [output_ref; output_ref_f(:,ind,:)]; 
end

referenceDSP.output.dsp.signals.values = output_dsp;
referenceDSP.output.ref.signals.values = output_ref;

% Associate output signal
output.dsp = referenceDSP.output.dsp.signals.values;
output.ref = referenceDSP.output.ref.signals.values;

% Place your code here
%===============================================
% Evaluate the output:
% 1) Plot output signal originating from reference (MATLAB) AND DSP
%    hardware
% 2) Plot difference between the two signals
figure;
subplot(2,1,1);
hold on; plot(output.dsp(1,:), 'r:');plot(output.ref(1,:), 'g-.');
legend({'Output DSP', 'Output Ref'});

subplot(2,1,2);
plot(abs(output.dsp(1,:)-output.ref(1,:)));
%===============================================
