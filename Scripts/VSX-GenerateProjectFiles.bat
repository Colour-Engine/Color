@echo off
pushd %~dp0\..\
call premake5 vs%1
popd
pause
