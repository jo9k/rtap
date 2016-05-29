@ECHO OFF
REM ================================================================
REM Start host...
REM One optional parameter: A config file may be specified directly, e.g.,
REM "jvx-qt-host-1 configfile.jvx"
REM ================================================================
set PATH=.\lib\;%PATH%
IF "%~1"=="" GOTO startnormal
GOTO startspecific
:startnormal
bin\jvx-qt-host-1.exe  --jvxdir jvxComponents --config jvx-qt-host-1.jvx
goto endscript
:startspecific
bin\jvx-qt-host-1.exe
:endscript
REM ENDSCRIPT
