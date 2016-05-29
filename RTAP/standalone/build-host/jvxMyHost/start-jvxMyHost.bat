@ECHO OFF
REM ================================================================
REM Start host...
REM One optional parameter: A config file may be specified directly, e.g.,
REM "jvxMyHost configfile.jvx"
REM ================================================================
set PATH=.\lib\;%PATH%
IF "%~1"=="" GOTO startnormal
GOTO startspecific
:startnormal
bin\jvxMyHost.exe  --jvxdir jvxComponents --config jvxMyHost.jvx
goto endscript
:startspecific
bin\jvxMyHost.exe
:endscript
REM ENDSCRIPT
