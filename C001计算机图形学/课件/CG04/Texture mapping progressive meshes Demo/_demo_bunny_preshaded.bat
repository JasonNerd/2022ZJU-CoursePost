@echo off

cd "%~p0"

set G3DARGS=-geom 700x700 -bigfont

if exist ..\bin\_initdemos.bat call ..\bin\_initdemos.bat
if exist .\_initdemos.bat call .\_initdemos.bat

echo .
echo Drag the slider using the right mouse button to change level-of-detail.
echo Press 'Dt' to toggle texture on/off.
echo .

rem start /b "C:\Program Files\Common Files\Microsoft Shared\PhotoEd.exe" bunny.bmp

G3dOGL.exe -pm_mode bunny.pm -st bunny.s3d -key DeDtDaDc -lighta .65 -lights .65 %G3DARGS% -maximize -key -----J
