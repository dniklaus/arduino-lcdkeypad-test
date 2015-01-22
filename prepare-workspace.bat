@echo off
::-----------------------------------------------------------------------------
:: Configured Settings
::-----------------------------------------------------------------------------
:: Arduino IDE (see http://arduino.cc/en/Main/OldSoftwareReleases) 
set ArduinoDownloadUrl=http://arduino.cc/download.php?f=
set ArduinoVer=1.5.6-r2

:: Eclipse Arduino Workbench Bundle (see http://www.baeyens.it/eclipse/download.php)
set EclipseArduinoDownloadUrl=http://www.baeyens.it/eclipse/download/product
set EclipseArduinoVer=2014-12-28_02-06-00

:: Expected Project Location (Eclipse CDT cannot deal with relative paths)
set ArduinoProjects=C:\git\arduino-projects
set ExpectedProjectHome=%ArduinoProjects%\arduino-lcdkeypad-test\

::-----------------------------------------------------------------------------
:: Get the OS Variant
::-----------------------------------------------------------------------------
IF "%PROCESSOR_ARCHITECTURE%;%PROCESSOR_ARCHITEW6432%"=="x86;" (
  set OsVariant=win32
) ELSE (
  set OsVariant=win64
)

::-----------------------------------------------------------------------------
:: Paths
::-----------------------------------------------------------------------------
set SCRIPT_DIR=%~dp0%
set ProjectHome=%SCRIPT_DIR%
set WorkspaceDir=%ProjectHome%\workspace
set ArduinoTools=%ProjectHome%\..\Tools
set ThisProjTools=%ProjectHome%\tools

::-----------------------------------------------------------------------------
:: Assert correct path
::-----------------------------------------------------------------------------
if not "%ExpectedProjectHome%"=="%ProjectHome%" (
  msg "%username%" Please install this project %ProjectHome% here: %ExpectedProjectHome% - otherwise you would not be able to compile.
  if not exist "%ArduinoProjects%" (
    md "%ArduinoProjects%"
  )
  goto end
)


::-----------------------------------------------------------------------------
:: Set the tools' paths
::-----------------------------------------------------------------------------
:: Arduino IDE
set ArduinoRevs=%ArduinoTools%\arduino_revs
set CurArduino=%ArduinoRevs%\arduino-%ArduinoVer%

:: Eclipse Arduino Workbench Bundle
set EclipseArduinoRevs=%ArduinoTools%\eclipseArduino_revs
set CurEclipseArduino=%EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%\eclipseArduino

:: 7Zip
set Archiver=%ThisProjTools%\7za920\7za.exe

:: curl (unused)
set Curl=%ThisProjTools%\curl\curl.exe

:: wget
set Wget=%ThisProjTools%\wget\wget.exe

:: git
if "%OsVariant%"=="win32" (
  set Git="%ProgramFiles%\Git\bin\git.exe"
) else (
  set Git="%ProgramFiles(x86)%\Git\bin\git.exe"
)

::-----------------------------------------------------------------------------
:: Assert untouched .project file
::-----------------------------------------------------------------------------
:: ensure that src/.project has not been changed
set statusResult=
for /f "delims=" %%a in ('%%Git%% status --porcelain %%ProjectHome%%\src\.project') do @set statusResult=%%a
echo "%statusResult%"
if not "%statusResult%"=="" (
  msg "%username%" The file %ProjectHome%\src\.project is already touched. This script shall only be run on a vanilla project just cloned before.
  goto end
)

::-----------------------------------------------------------------------------
:: Get the tools
::-----------------------------------------------------------------------------
:: Arduino IDE
if not exist "%CurArduino%" (
  if not exist "%ArduinoRevs%" (
    md "%ArduinoRevs%"
  )
  if not exist "%ArduinoRevs%\arduino-%ArduinoVer%-windows.zip" (
    ::%Curl% -# -o "%ArduinoRevs%\arduino-%ArduinoVer%-windows.zip" "%ArduinoDownloadUrl%/arduino-%ArduinoVer%-windows.zip"
    %Wget% --tries=0 --output-document="%ArduinoRevs%\arduino-%ArduinoVer%-windows.zip" "%ArduinoDownloadUrl%/arduino-%ArduinoVer%-windows.zip"
  )
  %Archiver% x -y -o%ArduinoRevs% %ArduinoRevs%\arduino-%ArduinoVer%-windows.zip
  if %errorlevel% == 0 (
    del %ArduinoRevs%\arduino-%ArduinoVer%-windows.zip
  )
)
:: create softlink (junction) for Arduino IDE in current project tools
rmdir %ThisProjTools%\arduino
mklink /J %ThisProjTools%\arduino %CurArduino%

:: Eclipse Arduino Workbench Bundle
if not exist "%CurEclipseArduino%" (
  if not exist "%EclipseArduinoRevs%" (
    md "%EclipseArduinoRevs%"
  )
  if not exist "%EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%.tar.gz" (
    ::%Curl% -# -o "%EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%.tar.gz" "%EclipseArduinoDownloadUrl%/%OsVariant%.%EclipseArduinoVer%.tar.gz"
    %Wget% --tries=0 --output-document="%EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%.tar.gz" "%EclipseArduinoDownloadUrl%/%OsVariant%.%EclipseArduinoVer%.tar.gz"
  )
  %Archiver% x -y -o%EclipseArduinoRevs% %EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%.tar.gz
  %Archiver% x -y -o%EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer% %EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%.tar
  if %errorlevel% == 0 (
    del %EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%.tar.gz
	del %EclipseArduinoRevs%\%OsVariant%.%EclipseArduinoVer%.tar
  )
)
:: create softlink (junction) for Eclipse Arduino Workbench Bundle in current project tools
rmdir %ThisProjTools%\eclipseArduino
mklink /J %ThisProjTools%\eclipseArduino %CurEclipseArduino%

:: unpack Eclipse metadata in workspace
if not exist "%WorkspaceDir%\.metadata" (
  %Archiver% x -y -o%WorkspaceDir% "%WorkspaceDir%\.metadata.zip"
)

::-----------------------------------------------------------------------------
:: Perform Workaround Eclipse Arduino Bug (removed Library links in .project file after first [failing] build)
::-----------------------------------------------------------------------------
:: run first build, would fail
call build.bat
if %errorlevel% == 0 goto end
:: revert src/.project that has been made dirty by the failing build
%Git% checkout -- %ProjectHome%\src\.project

:: run second build
call build.bat
if %errorlevel% == 0 goto end

:error
msg "%username%" An error occured!
::pause

:end
