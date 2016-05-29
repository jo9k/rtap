@ECHO OFF
REM ================================================================
REM Start host...
REM One optional parameter: A config file may be specified directly, e.g.,
REM "jvxRtprocHost configfile.jvx"
REM ================================================================
set PATH=.\lib\;%PATH%
IF "%~1"=="" GOTO startnormal
GOTO startspecific
:startnormal
bin\jvxRtprocHost.exe  --jvxdir jvxComponents --config jvxRtprocHost.jvx
goto endscript
:startspecific
bin\jvxRtprocHost.exe
:endscript
REM ENDSCRIPT
