@echo off
set source_fold=.\output
set target_fold=..\adapt

if exist "%source_fold%\cfg_autogen_c1.h" copy "%source_fold%\cfg_autogen_c1.h" %target_fold% /y
if exist "%source_fold%\cfg_autogen_ca.c" copy "%source_fold%\cfg_autogen_ca.c" %target_fold% /y
if exist "%source_fold%\cfg_autogen_ct.h" copy "%source_fold%\cfg_autogen_ct.h" %target_fold% /y

echo "Copy auto-generated files to compile path finished."
pause