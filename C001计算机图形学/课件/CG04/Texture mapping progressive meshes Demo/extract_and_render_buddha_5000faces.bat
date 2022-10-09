@echo off

cd "%~p0"

set G3DARGS=-bigfont

if exist ..\bin\_initdemos.bat call ..\bin\_initdemos.bat
if exist .\_initdemos.bat call .\_initdemos.bat

FilterPM.exe buddhaf.tmpm.trunc.pm -nfaces 5000 -outmesh >buddhaf.nf5000.m

Filtermesh.exe buddhaf.nf5000.m -splitcorners >buddhaf.nf5000.split.m

echo .
echo Press 'De' to toggle mesh edges on/off.
echo Press 'Dt' to toggle texture-mapping off/on.
echo .

G3dOGL.exe buddhaf.nf5000.split.m -st buddhaf.s3d -texturemap buddhaf.tmpm.trunc.nor.bmp -texturenormal 1 -key Dt %G3DARGS% -maximize -key -----J
