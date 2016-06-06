% miro :: Measured Impulse Response Object
%
% miro tutorial/application : How to export Spherical Datasets to
%                             the SOFiA sound field analysis toolbox
% SOFiA Website             : http://code.google.com/p/sofia-toolbox/
%
% 2012 by Benjamin Bernschütz (bBrn)
%         Cologne University of Applied Sciences
%         Institute of Communication Systems
%         mail benjamin.bernschuetz@fh-koeln.de
%         GSM  +49 171 4176069
%
% MIRO V1, Release 1.03 - 03/oct/2012


clc
clear all

fprintf('Welcome to a miro tutorial script on SOFiA export.\n\n')
fprintf('SOFiA Website: http://code.google.com/p/sofia-toolbox/ \n\n')
fprintf('Required files in this directory or in the searchpath: miro.m, CR1_VSA_110RS_L.mat\nStatus: ')

if ~exist('miro.m') || ~exist('CR1_VSA_110RS_L.mat')
    fprintf('failed.\n\n');
    fprintf('Source to get the missing files: http://www.audiogroup.web.fh-koeln.de\n')
    error('The required files are not accessible.');
    return
else
    fprintf('okay\n\n');
end



% Loading the miro object:

fprintf('Loading: CR1_VSA_110RS_L.mat\n\n');
load('CR1_VSA_110RS_L.mat')


% Step 1 : Export CR1_VSA_110RS_L set to SOFiA-readable timedata struct. 

fprintf('Step1: Direcly export CR1_VSA_110RS_L set to SOFiA-readable timedata structs.\n'); 
fprintf('We obtain only one struct as this is a one-channel object. ');
input('<Enter to continue>');

[timeData1, timeData2] = CR1_VSA_110RS_L.miroToSOFiA();

fprintf('\nThis is struct 1 (Coming from channel 1):\n');
disp(timeData1)

fprintf('\nStruct 2 is empty in this case, as the dataset has only one audio channel. \nIf we had e.g. a spherical HRIR object, both structs would contain data (Left Ear, Right Ear).\n');
fprintf('\n\nThe struct timeData1 can now be saved or directly be forwarded to the SOFiA F/D/T function.\n');


% Step 2 : Set a shorter return tap property to CR1_VSA_110RS_L.mat array set
% This step is identical to the Tutorial 2 (advanced)

fprintf('\n\nStep 2: Set a shorter return tap property to CR1_VSA_110RS_L.mat microphone array dataset.  ');
input('<Enter to continue>');


fprintf('\nReturn taps original: %d\n', CR1_VSA_110RS_L.returnTaps);

irOld = CR1_VSA_110RS_L.getIR(1);

CR1_VSA_110RS_L = CR1_VSA_110RS_L.setReturnTaps(3500);

irNew = CR1_VSA_110RS_L.getIR(1);

fprintf('Return taps new     : %d\n', CR1_VSA_110RS_L.returnTaps);

hgr = figure(1);

plot(irOld,'k')
hold on
plot(irNew,'g')
title('Impulse response irID=1 before and after setting return taps property.');
xlabel('Samples');
ylabel('Amplitude');
input('\n\n<Enter to continue>\n\n');
close(hgr);

% Step 3 : Export the truncated CR1_VSA_110RS_L.mat array set

fprintf('Step 3 : Export the truncated CR1_VSA_110RS_L.mat array set ');
input('<Enter to continue>');

[timeData1, timeData2] = CR1_VSA_110RS_L.miroToSOFiA();

fprintf('\nThis is struct 1 (Coming from channel 1):\n');

disp(timeData1)

fprintf('\nStruct 2 is empty in this case, as the dataset has only one audio channel.');

fprintf('\n\nThe struct timeData1 can now be saved or directly be forwarded to the SOFiA F/D/T function.\n');


input('\n\n<Enter to continue>');
clc
fprintf('Done! This was the SOFiA export tutorial. Thank you for your interest. Not every detail has been covered.\n')
fprintf('This tutorial is more to give you a basic idea how to use the SOFiA export method.\n\n')
fprintf('For more information read the data type description manual.\n\n')
fprintf('Source: http://www.audiogroup.web.fh-koeln.de, Author: Benjamin Bernschütz\n')

