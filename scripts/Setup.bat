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

REM setlocal
REM Go to project root
REM pushd "%~dp0\.."

REM REM --- 0. Initialize submodules ---
REM echo Checking git submodules...
REM git submodule update --init --recursive

REM REM --- 1. Bootstrap vcpkg if missing ---
REM if not exist "extern\vcpkg\vcpkg.exe" (
    REM echo Bootstrapping vcpkg...
    REM if not exist "extern\vcpkg" (
        REM echo ERROR: Submodule extern\vcpkg is missing!
        REM echo Did you forget to add it? Aborting.
        REM popd
        REM endlocal
        REM exit /b 1
    REM )
    REM call extern\vcpkg\bootstrap-vcpkg.bat
REM ) else (
    REM echo vcpkg already bootstrapped.
REM )

REM REM --- 2. Install dependencies from vcpkg.json ---
REM if exist "vcpkg.json" (
    REM echo Installing vcpkg dependencies from manifest...
    REM pushd extern\vcpkg
    REM call vcpkg install --triplet x64-windows
    REM popd
REM ) else (
    REM echo WARNING: vcpkg.json not found! Skipping dependency installation.
REM )

REM popd
REM echo Setup complete. vcpkg is bootstrapped and dependencies installed.
REM pause
REM endlocal









REM @echo off
REM setlocal

REM REM Go to project root (parent folder of scripts)
REM pushd "%~dp0\.."

REM echo Initializing git submodules...
REM git submodule update --init --recursive


REM REM REM --- 1. Bootstrap vcpkg if missing ---
REM REM if not exist "extern\vcpkg\vcpkg.exe" (
    REM REM echo Cloning vcpkg submodule...
    REM REM git submodule update --init extern/vcpkg
	
	REM REM echo Bootstrapping vcpkg...
    REM REM call extern\vcpkg\bootstrap-vcpkg.bat
REM REM ) else (
    REM REM echo vcpkg already exists.
REM REM )

REM REM --- 2. Install dependencies from vcpkg.json ---
REM REM if exist "vcpkg.json" (
    REM REM echo Installing vcpkg dependencies from manifest...
    REM REM pushd extern\vcpkg
    REM REM call extern\vcpkg\vcpkg install --triplet x64-windows
	REM REM popd
REM REM ) else (
    REM REM echo WARNING: vcpkg.json not found! Skipping dependency installation.
REM REM )

REM popd
REM echo Setup complete. You can now build and run the project.
REM pause
