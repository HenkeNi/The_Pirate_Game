@echo off

REM Go up one directory from the script location
pushd "%~dp0\.."

REM Remove the build directory if it exists
if exist build (
    rmdir /s /q build
    echo Build directory removed.
) else (
    echo Build directory does not exist.
)

popd
pause