@echo off

cd "%~p0"

set G3DARGS=-bigfont

if exist ..\bin\_initdemos.bat call ..\bin\_initdemos.bat
if exist .\_initdemos.bat call .\_initdemos.bat

echo .
echo Drag the slider using the right mouse button to change level-of-detail.
echo Press 'Dt' to toggle texture on/off.
echo .

G3dOGL.exe -pm_mode tyra.tmpm.trunc.pm -st tyra.s3d -key DeDtDtDtDaDc %G3DARGS% -maximize -key -----J
