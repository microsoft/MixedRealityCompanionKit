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
call :CopyDLL "%~dp0\Samples\SharedHolograms\Assets\"


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

:CopyDLL
ECHO.
ECHO ===================================================
ECHO Copy DLL's to %~1

IF EXIST "%~1\Addons\HolographicCameraRig" (
    REM Copy UnityCompositorInterface
    ECHO.
    ECHO Copy UnityCompositorInterface files:
    
    copy "%~dp0\Compositor\x64\Release\UnityCompositorInterface.dll" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy "%~dp0\Compositor\x64\Release\UnityCompositorInterface.pdb" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )

    copy "%~dp0\Compositor\Release\UnityCompositorInterface.dll" "%~1\Addons\HolographicCameraRig\Plugins\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy "%~dp0\Compositor\Release\UnityCompositorInterface.pdb" "%~1\Addons\HolographicCameraRig\Plugins\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    
    
    REM Copy SpatialPerceptionHelperSpatialPerceptionHelper
    ECHO.
    ECHO ---------------------------------------------------
    ECHO Copy SpatialPerceptionHelper files:
    
    copy /y "%~dp0\Compositor\x64\Release\SpatialPerceptionHelper\SpatialPerceptionHelper.dll" "%~1\Addons\HolographicCameraRig\Plugins\WSA\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy /y "%~dp0\Compositor\x64\Release\SpatialPerceptionHelper\SpatialPerceptionHelper.pdb" "%~1\Addons\HolographicCameraRig\Plugins\WSA\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )

    copy /y "%~dp0\Compositor\Release\SpatialPerceptionHelper\SpatialPerceptionHelper.dll" "%~1\Addons\HolographicCameraRig\Plugins\WSA\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy /y "%~dp0\Compositor\Release\SpatialPerceptionHelper\SpatialPerceptionHelper.pdb" "%~1\Addons\HolographicCameraRig\Plugins\WSA\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    
    
    REM Copy CompositorDLL
    ECHO.
    ECHO ---------------------------------------------------
    ECHO Copy CompositorDLL:
    copy /y "%~dp0\Compositor\x64\Release\CompositorDLL.dll" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy /y "%~dp0\Compositor\x64\Release\CompositorDLL.pdb" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    
    copy /y "%~dp0\Compositor\Release\CompositorDLL.dll" "%~1\Addons\HolographicCameraRig\Plugins\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy /y "%~dp0\Compositor\Release\CompositorDLL.pdb" "%~1\Addons\HolographicCameraRig\Plugins\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    
    
    REM Copy CanonSDK
    REM NOTE: This copy can be removed if not using the Canon SDK
    ECHO.
    ECHO ---------------------------------------------------
    ECHO Copy Canon SDK:
    if EXIST "%~dp0\Compositor\x64\Release\EDSDK.dll" (
        copy /y "%~dp0\Compositor\x64\Release\EDSDK.dll" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
        if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
        copy /y "%~dp0\Compositor\x64\Release\EdsImage.dll" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
        if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    ) ELSE (
        ECHO.
        ECHO x64 Canon DLL does not exist - skipping copy.
    )
    if EXIST "%~dp0\Compositor\Release\EDSDK.dll" (
        copy /y "%~dp0\Compositor\Release\EDSDK.dll" "%~1\Addons\HolographicCameraRig\Plugins\x86\"
        if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
        copy /y "%~dp0\Compositor\Release\EdsImage.dll" "%~1\Addons\HolographicCameraRig\Plugins\x86\"
        if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    ) ELSE (
        ECHO.
        ECHO.
        ECHO x86 Canon DLL does not exist - skipping copy.
    )
    
    REM Copy OpenCV
    REM NOTE: x64 only unless built from source.
    REM NOTE: This copy can be removed if not using an OpenCV frame provider.
    ECHO.
    ECHO ---------------------------------------------------
    ECHO Copy OpenCV:
    copy /y "%~dp0\Compositor\x64\Release\OpenCV*.dll" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    
    
    REM Copy GitIgnore
    ECHO.
    ECHO ---------------------------------------------------
    ECHO Copy GitIgnore:
    copy /y "%~dp0\Compositor\PluginMetaFiles\*" "%~1\Addons\HolographicCameraRig\Plugins\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )


    REM Copy meta files
    ECHO.
    ECHO ---------------------------------------------------
    ECHO Copy meta files:
    copy /y "%~dp0\Compositor\PluginMetaFiles\WSA\x64\*.meta" "%~1\Addons\HolographicCameraRig\Plugins\WSA\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy /y "%~dp0\Compositor\PluginMetaFiles\WSA\x86\*.meta" "%~1\Addons\HolographicCameraRig\Plugins\WSA\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy /y "%~dp0\Compositor\PluginMetaFiles\x64\*.meta" "%~1\Addons\HolographicCameraRig\Plugins\x64\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    copy /y "%~dp0\Compositor\PluginMetaFiles\x86\*.meta" "%~1\Addons\HolographicCameraRig\Plugins\x86\"
    if !ERRORLEVEL! NEQ 0 ( set /a returnValue += 1 )
    
) ELSE (
    ECHO.
    ECHO "%~1\Addons\HolographicCameraRig" does not exist.
    set /a returnValue += 1
)

ECHO ===================================================

goto:eof