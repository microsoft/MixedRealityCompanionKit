@ECHO OFF
REM // Copyright (c) Microsoft Corporation. All rights reserved.
REM // Licensed under the MIT License. See LICENSE in the project root for license information.

ECHO.
ECHO ===================================================
ECHO Copy DLL's to your Unity projects.
ECHO Include an optional path to the Assets path of your Unity project to copy DLLs to that project.
ECHO Otherwise, just copy to the included sample projects.
ECHO ===================================================
ECHO.

Setlocal EnableDelayedExpansion

set /a returnValue=0

if "%~1" NEQ "" (
    call :CopyDLL "%~1"
)

REM Copy DLLs to Sample Unity projects.
REM call :CopyDLL "%~dp0\Samples\SharedHolograms\Assets\"
call :CopyDLL "%~dp0\..\SpectatorViewSample\Assets\"

if %returnValue% NEQ 0 (
    ECHO.
    ECHO.
    ECHO.
    ECHO ***************************************************
    ECHO ***************************************************
    ECHO.
    ECHO    ERROR: One or more files did not copy.
    ECHO           Please review log.
    ECHO.
    ECHO ***************************************************
    ECHO ***************************************************
    ECHO.
    ECHO.
    ECHO.
)

pause
goto:eof

REM Only copying x64 DLLs since Unity is now only an x64 executable.
REM Spectator view DLLs are only meant to run in the editor.
:CopyDLL
ECHO.
ECHO ===================================================
ECHO Copy DLL's to %~1

Call :CreateSubdirectories %~1

REM Copy UnityCompositorInterface
ECHO.
ECHO Copy UnityCompositorInterface files:

REM x64
copy "%~dp0\x64\Release\UnityCompositorInterface.dll" "%~1\Addons\SpectatorViewRig\Plugins\x64\"
if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
copy "%~dp0\x64\Release\UnityCompositorInterface.pdb" "%~1\Addons\SpectatorViewRig\Plugins\x64\"
if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )


REM Copy CompositorDLL
ECHO.
ECHO ---------------------------------------------------
ECHO Copy CompositorDLL:
copy /y "%~dp0\x64\Release\CompositorDLL.dll" "%~1\Addons\SpectatorViewRig\Plugins\x64\"
if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
copy /y "%~dp0\x64\Release\CompositorDLL.pdb" "%~1\Addons\SpectatorViewRig\Plugins\x64\"
if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
	
	
REM Copy OpenCV
REM NOTE: This copy can be removed if not using an OpenCV frame provider.
ECHO.
ECHO ---------------------------------------------------
ECHO Copy OpenCV:
copy /y "%~dp0\x64\Release\OpenCV*.dll" "%~1\Addons\SpectatorViewRig\Plugins\x64\"
if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )


ECHO ===================================================

goto:eof

:CreateSubDirectories
IF NOT EXIST "%~1\Addons\" (
	ECHO Creating Addons directory.
	MKDIR "%~1\Addons\"
)

IF NOT EXIST "%~1\Addons\SpectatorViewRig\" (
	ECHO Creating SpectatorView directory.
	MKDIR "%~1\Addons\SpectatorViewRig\"
)

IF NOT EXIST "%~1\Addons\SpectatorViewRig\Plugins\" (
	ECHO Creating Plugins directory.
	MKDIR "%~1\Addons\SpectatorViewRig\Plugins\"
)

IF NOT EXIST "%~1\Addons\SpectatorViewRig\Plugins\x64\" (
	ECHO Creating Plugins x64 directory.
	MKDIR "%~1\Addons\SpectatorViewRig\Plugins\x64\"
)

goto:eof