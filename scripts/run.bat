@echo off
setlocal

:: Move to project root
pushd "%~dp0\.."

:: Ensure build folder exists in root
mkdir build 2>nul

:: Configure/build using root-relative build folder
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug

:: Run executable
build\Debug\Game.exe %*

popd
pause



REM @echo off
REM setlocal

REM :: Go to project root (parent folder of scripts)
REM cd /d "%~dp0\.."

REM :: Check for CMakePresets.json
REM if exist "CMakePresets.json" (
    REM echo Found CMakePresets.json
    REM for /f "delims=" %%i in ('cmake --list-presets ^| findstr /i "default"') do set PRESET_FOUND=1
    REM if defined PRESET_FOUND (
        REM echo Using preset 'default'
        REM cmake --preset default
    REM ) else (
        REM echo 'default' preset not found, using standard CMake configuration
        REM mkdir build 2>nul
        REM cmake -S . -B build -G "Visual Studio 17 2022" -A x64
    REM )
REM ) else (
    REM echo No CMakePresets.json found, using standard CMake configuration
    REM mkdir build 2>nul
    REM cmake -S . -B build -G "Visual Studio 17 2022" -A x64
REM )

REM :: Build the project
REM cmake --build build --config Debug

REM :: Run the executable
REM build\Debug\MyGame.exe %*
REM pause
