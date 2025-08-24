@echo off
REM -------------------------------
REM setup.bat - bootstrap vcpkg and install dependencies from vcpkg.json
REM -------------------------------

cd /d "%~dp0\.."

git submodule update --init --recursive

if %errorlevel% neq 0 (
	echo Error occured when updating submodules
	pause
	exit /b 1
) 
	
echo submodules updated successfully!

REM Build vcpkg (Windows)
cd extern/vcpkg
bootstrap-vcpkg.bat

if %errorlevel% neq 0 (
	echo Failed to run the vcpkg/bootstrap.bat script!
	pause
	exit /b 1
)

echo vcpkg bootstrapped successfully!
pause