% miro :: Measured Impulse Response Object
%
% miro tutorial/application : How to export HRIR/BRIR Datasets to
%                             the Sound Scape Renderer
% SSR Website               : http://www.tu-berlin.de/?id=ssr
%
% 2012 by Benjamin Bernschütz (bBrn)
%         Cologne University of Applied Sciences
%         Institute of Communication Systems
%         mail benjamin.bernschuetz@fh-koeln.de
%         GSM  +49 171 4176069
%
% MIRO V1, Release 1.03 - 03/oct/2012

% This script tutorial shows how to generate a HRIR dataset. To generate a
% BRIR dataset the procedure is identical.

clc
clear all

fprintf('Welcome to a miro tutorial script on SSR export.\n\n')
fprintf('SSR Website: http://www.tu-berlin.de/?id=ssr \n\n')
fprintf('Required files in this directory or in the searchpath: miro.m, HRIR_CIRC360.mat and AKG-K601.mat\nStatus: ')

if ~exist('miro.m') || ~exist('HRIR_CIRC360.mat') || ~exist('AKG-K601.mat')
    fprintf('failed.\n\n');
    fprintf('Source to get the missing files: http://www.audiogroup.web.fh-koeln.de\n')
    error('The required files are not accessible.');
    return
else
    fprintf('okay\n\n');
end

% Loading miro objects:

fprintf('Loading: HRIR_CIRC360.mat\n\n');
load('HRIR_CIRC360.mat')

% Step 1 : Loading the headphone compensation filter for the AKG-K601

fprintf('Step1: Loading the headphone compensation filters for an AKG-K601.');
input('  <Enter to continue>\n\n');
load('AKG-K601.mat', 'hpcf')

% Step 2 : Setting the headphone filter to the object:

fprintf('Step2: Setting the Headphone Filters');
input('  <Enter to continue>');

HRIR_CIRC360 = HRIR_CIRC360.setHeadPhones(hpcf); % <<< Here is the command

fprintf('\nLooking at the properties shows the active headphone filter:');
input('  <Enter to continue>');

HRIR_CIRC360 % <<< Here is the command

fprintf('\nStep 3: Export to the 720ch interleaved file:');
input('  <Enter to continue>');

% Step 3 : Export:

HRIR_CIRC360.miroToSSR(); % <<< Here is the command
input('\n\n<Enter to continue>');

clc
fprintf('Done! This was the SSR export tutorial. Thank you for your interest. Not every detail has been covered.\n')
fprintf('This tutorial is more to give you a basic idea how to use the SSR export method.\n\n')
fprintf('For more information read the data type description manual.\n\n')
fprintf('Source: http://www.audiogroup.web.fh-koeln.de, Author: Benjamin Bernschütz\n')

return

% Condensed code:

load('HRIR_CIRC360.mat')
load('AKG-K601.mat', 'hpFilter')
HRIR_CIRC360 = HRIR_CIRC360.setHeadPhones(hpFilter);
HRIR_CIRC360.miroToSSR();
