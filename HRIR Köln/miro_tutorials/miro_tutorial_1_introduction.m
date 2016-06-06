% miro :: Measured Impulse Response Object
%
% miro tutorial/application : Tutorial I Basic Introduction
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


fprintf('Welcome to the basic introduction tutorial script\n\n')
fprintf('Required files in this directory or in the searchpath: miro.m, HRIR_CIRC360.mat and CR1_VSA_110RS_L.mat\nStatus: ')

if ~exist('miro.m') || ~exist('HRIR_CIRC360.mat') || ~exist('CR1_VSA_110RS_L.mat')
    fprintf('failed.\n\n');
    fprintf('Source to get the missing files: http://www.audiogroup.web.fh-koeln.de\n')
    error('The required files are not accessible.');
    return
else
    fprintf('okay\n\n');
end

% Step 1 : Loading miro objects:

fprintf('Step1: Loading objects using the load() command');
input('  <Enter to continue>');
fprintf('Loading: HRIR_CIRC360.mat\n');
load('HRIR_CIRC360.mat')
fprintf('Loading: CR1_VSA_110RS_L.mat\n\n');
load('CR1_VSA_110RS_L.mat')

% Step 2 : Showing all properties and methods

if startAtStep <= 2
    
    fprintf('Step2: Showing all properties and methods (of one object)');
    input('  <Enter to continue>');
    
    CR1_VSA_110RS_L             % <<< Here is the command
    
end

if startAtStep <= 3
    
    fprintf('Step3: Reading a single property (e.g. location)');
    input('  <Enter to continue>');
    
    HRIR_CIRC360.location       % <<< Here is the command
    CR1_VSA_110RS_L.location    % <<< Here is the command
    
end

if startAtStep <= 4
    
    fprintf('Step4a: Using Methods: Show me the sample grid of the CR1_VSA_110RS_L object.');
    input('  <Enter to continue>');

    hgr = figure(1);
    
    CR1_VSA_110RS_L.plotQuadrature()    % <<< Here is the command
    
    % Here we use the Dot-Operator to call a method. Using the Dot-Operator
    % makes the first Argument (Obj) obsolete, as the refered instance is 
    % unique. 
    
    input('  <Enter to continue>');
    
    close(hgr);
    
    fprintf('\nStep4b: (Methods) Show me the sample grid of the HRIR_CIRC360 object.');
    
    hgr = figure(1);  
    
    HRIR_CIRC360.plotQuadrature()     % <<< Here is the command
   
    input('  <Enter to continue>');
    close(hgr);
    
end

if startAtStep <= 5
    
    fprintf('\nStep5: (Methods) Get the center omni impulse response of HRIR_CIRC360');

      
    centerIR = HRIR_CIRC360.getIR(0);  % <<< Here is the command
    
    hgr = figure(1);
    
    plot(centerIR,'r')
    title('Center IR of HRIR\_CIRC360');
    xlabel('Samples');
    ylabel('Amplitude');
    
    input('  <Enter to continue>');
    close(hgr);
end

if startAtStep <= 6
    
    fprintf('\nStep6: (Methods) Look for the closest angle to e.g 110DEG AZ and 45DEG EL in CR1_VSA_110RS_L and get the corresponding IR');
    input('  <Enter to continue>');

    %1. Change the object's reference system to DEG (Easier to imagine/handle in this case)
    
    CR1_VSA_110RS_L = CR1_VSA_110RS_L.setDEG(); % <<< Here is the command

    % (The object must be rewritten into the same instance or a new
    % instance as it is a "Value Class"!)
    
    
    %2. Get the irID of the closes impulse response and the closest angles
    
    [irID, azFound, elFound] = CR1_VSA_110RS_L.closestIr(110, 45); % <<< Here is the command
      
    fprintf('\nirID    : %d\n', irID);
    fprintf('azFound : %f DEG\n', azFound);
    fprintf('azFound : %f DEG\n\n', elFound);
    
    fprintf('Get irID %d and plot it.', irID);
      
    %3. Get the impulse response at irID 
    
    closestIR = CR1_VSA_110RS_L.getIR(irID);  % <<< Here is the command
  
    hgr = figure(1);
    
    plot(closestIR ,'r')
    title(['CR1\_VSA\_110RS\_L,  irID:', num2str(irID)]);
    xlabel('Samples');
    ylabel('Amplitude');
    input('  <Enter to continue>');
    close(hgr);
end

if startAtStep <= 7
    
    fprintf('\nStep7: (Methods) Get a stereo impulse response from HRIR_CIRC360, e.g. irID = 91');
    input('  <Enter to continue>');

          
    stereoIR = HRIR_CIRC360.getIR(91);  % <<< Here is the command
  
    fprintf('\nThe matrix size indicates the two channels:');
    
    disp(size(stereoIR))
    
    hgr = figure(1);
    
    plot(stereoIR)
    title('Stereo impulse response (HRIR) with irID=91 of HRIR\_CIRC360');
    xlabel('Samples');
    ylabel('Amplitude');
    input('<Enter to continue>');
    close(hgr);
end


clc
fprintf('Done! This was the basic tutorial. Thank you for your interest. Of course not all methods etc. were covered.\n')
fprintf('This tutorial is more to give you a basic idea how to use the miro data type.\n\n')
fprintf('For more information read the data type description manual or do one of the advanced tutorials on special functions.\n\n')
fprintf('Source: http://www.audiogroup.web.fh-koeln.de, Author: Benjamin Bernschütz\n')

