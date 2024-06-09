@echo off

if exist *.pdb del *.pdb

set WFLAGS=/W4 /WX /wd4100 /wd4101 /wd4189
set CFLAGS=/nologo /Zi /Gm- /GR- /GS- /MT %WFLAGS%
set LFLAGS=/DEBUG:FULL /INCREMENTAL:NO

cl eval_app.c %CFLAGS% /Fe:eval.exe /link %LFLAGS%
if %errorlevel% neq 0 exit /b %errorlevel%

cl eval_test.c %CFLAGS% /link %LFLAGS%
if %errorlevel% neq 0 exit /b %errorlevel%

eval_test.exe
if %errorlevel% neq 0 exit /b %errorlevel%

echo Success