@echo // Copyright (C) 1996-2008 Markus F.X.J. Oberhumer
@echo //
@echo //   OS/2 16-bit
@echo //   Watcom C/C++
@echo //
@call b\prepare.bat
@if "%BECHO%"=="n" echo off


set CC=wcl -zq -ml -2 -bt#os2 -l#os2
set CF=-ox %CFI%
set LF=%BLIB%

%CC% %CF% -c src\*.c
@if errorlevel 1 goto error
wlib -q -b -n -t %BLIB% @b\dos16\wc.rsp
@if errorlevel 1 goto error

%CC% %CF% examples\dict.c %LF%
@if errorlevel 1 goto error
%CC% %CF% examples\lzopack.c %LF%
@if errorlevel 1 goto error
%CC% %CF% examples\precomp.c %LF%
@if errorlevel 1 goto error
%CC% %CF% examples\precomp2.c %LF%
@if errorlevel 1 goto error
%CC% %CF% examples\simple.c %LF%
@if errorlevel 1 goto error

%CC% %CF% lzotest\lzotest.c %LF%
@if errorlevel 1 goto error

%CC% %CF% -Iinclude\lzo minilzo\testmini.c minilzo\minilzo.c
@if errorlevel 1 goto error


@call b\done.bat
@goto end
:error
@echo ERROR during build!
:end
@call b\unset.bat
