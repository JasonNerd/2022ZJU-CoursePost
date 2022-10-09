@echo off

cd "%~p0"

if exist ..\bin\_initdemos.bat call ..\bin\_initdemos.bat
if exist .\_initdemos.bat call .\_initdemos.bat

echo .
echo Drag the slider in the left window using the right mouse button.
echo Press 'Dt' in the left window to toggle texture-mapping.
echo .

rem If I could use csh.exe:
rem csh -fc "G3dcmp -g3d 'G3dOGL -pm_mode' buddhaf.{tmpm,orig}.trunc.pm -win1 '-key DeDtDaDc-----J' -win2 '-key ,i' -st buddhaf_right.s3d -bigfont"



rem Set the DISPLAYHEIGHT DISPLAYWIDTH DISPLAYDEPTH variables.
for /F "usebackq delims== tokens=1,2" %%i in (`cscript //Nologo "%~p0vbgetdisplaydims.vbs"`) do set %%i=%%j

rem set DISPLAYWIDTH=1024

set A=-st buddhaf_right.s3d -bigfont

set G1=-geom 500x500+0+0
set G2=-geom 500x500+508+0

if %DISPLAYWIDTH% leq 1024 goto skipbigscreen
set G1=-geom 600x600+44+0
set G2=-geom 600x600+652+0
:skipbigscreen

G3dOGL -pm_mode buddhaf.tmpm.trunc.pm %A% %G1% -key "&O" -key DeDtDaDc-----J  |  G3dOGL -pm_mode buddhaf.orig.trunc.pm %A% %G2% -async -killeof -input
