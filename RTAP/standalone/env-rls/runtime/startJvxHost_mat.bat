@ECHO OFF

REM ================================================================
REM ================================================================
set SYSTEM_JVX_MEX_PATH='./matlab/'
set SYSTEM_JVX_MATLAB_PATH={'./matlab/m-files' './matlab/m-files/subprojects-audionode' './matlab/m-files/audioProperties'}
set SYSTEM_JVX_MATLAB_DEMO={'Realtime Processing: jvxHost(''rt-audio'', ''--config'', ''config.jvx'', ''--jvxdir'', ''jvxComponents'');';'Offline Processing: jvxHost(''off-audio'', ''--config'', ''config.jvx'', ''--jvxdir'', ''jvxComponents'');'}

IF "%~1"=="" GOTO no_extra_matpath
GOTO extra_precommand
:no_extra_matpath
set JVX_MATLAB_START_MATPATH=
set JVX_MATLAB_START_COMMAND=start_inmat
GOTO cont_extra_precommand
:extra_precommand
set JVX_MATLAB_START_MATPATH=addpath %~1;

IF "%~2"=="" GOTO no_extra_startcommand
GOTO extra_startcommand
:no_extra_startcommand
set JVX_MATLAB_START_COMMAND=start_inmat
GOTO cont_extra_startcommand
:extra_startcommand
set JVX_MATLAB_START_COMMAND=%~2
:cont_extra_startcommand

:cont_extra_precommand


set JVX_MATLAB_START_ARGUMENT="%JVX_MATLAB_START_MATPATH% jvx = %JVX_MATLAB_START_COMMAND%(%SYSTEM_JVX_MEX_PATH%, %SYSTEM_JVX_MATLAB_PATH%, %SYSTEM_JVX_MATLAB_DEMO%);"

REM ================================================================
REM Start Matlab
REM ================================================================
echo start "Jvx" "%MATLAB_PATH_64%/bin/matlab" -desktop -r %JVX_MATLAB_START_ARGUMENT%
start "Jvx" "%MATLAB_PATH_64%/bin/matlab" -desktop -r %JVX_MATLAB_START_ARGUMENT%
