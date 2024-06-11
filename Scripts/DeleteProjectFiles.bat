@echo off
pushd %~dp0\..\
del /S /Q *.sln *.vcxproj *.filters *.user
popd
echo Deleted all project files.
pause
