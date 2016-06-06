% miro :: Measured Impulse Response Object
%
% miro tutorial/application : Tutorial II Advanced
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
startAtStep = 1; % <<< Change this number to skip the tutorial steps you don't like to see


fprintf('Welcome to the advanced tutorial script\n\n')
fprintf('Required files in this directory or in the searchpath: miro.m, HRIR_CIRC360.mat and CR1_VSA_110RS_L.mat\nStatus: ')

if ~exist('miro.m') || ~exist('HRIR_CIRC360.mat') || ~exist('CR1_VSA_110RS_L.mat') || ~exist('AKG-K601.mat')
    fprintf('failed.\n\n');
    fprintf('Source to get the missing files: http://www.audiogroup.web.fh-koeln.de\n')
    error('The required files are not accessible.');
    return
else
    fprintf('okay\n\n');
end

% Loading miro objects:

fprintf('Loading: HRIR_CIRC360.mat\n');
load('HRIR_CIRC360.mat')
fprintf('Loading: CR1_VSA_110RS_L.mat\n\n');
load('CR1_VSA_110RS_L.mat')



if startAtStep <= 1
    
    fprintf('\nStep 1: Set a shorter return tap property to CR1_VSA_110RS_L.mat microphone array dataset.  ');
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
   
end

if startAtStep <= 2 % Set a headphone compensation filter for an AKG-K601 to HRIR_CIRC360.mat
    
    fprintf('Step 2: Set a headphone compensation filter for an AKG-K601 to HRIR_CIRC360.mat');
    input('  <Enter to continue>');
    fprintf('Loading: AKG-K601.mat\n');
    load('AKG-K601.mat', 'hpcf')    
    HRIR_CIRC360 = HRIR_CIRC360.setHeadPhones(hpcf); % <<< Here is the command

    fprintf('\nLooking at the properties shows the active headphone filter:');
    input('  <Enter to continue>');
    HRIR_CIRC360                                         % <<< Here is the command
    input('<Enter to continue>');

end

if startAtStep <= 3 % Step 3: Set resampling target fs and write a wave file of irID = 1'
    
    fprintf('\n\nStep 3: Set resampling target fs and write a wave file');
    input('  <Enter to continue>');
    
    targetFS = 44100;
    irID     = 1;
    
    HRIR_CIRC360 = HRIR_CIRC360.setResampling(targetFS); % <<< Here is the command
    HRIR_CIRC360.dropWaveFile(irID);                     % <<< Here is the command
    
    fprintf('\nA wave file of irID at fs=44100Hz with the AKG-601 filters has been dropped.');

    input('  <Enter to continue>');
end



clc
fprintf('Done! This was the advanced tutorial. Thank you for your interest. Of course not all methods etc. were covered.\n')
fprintf('For more information read the data type description manual or do one of the tutorials on special functions.\n\n')
fprintf('Source: http://www.audiogroup.web.fh-koeln.de, Author: Benjamin Bernschütz\n')



