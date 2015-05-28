mkdir scripts
cd scripts
if not exist ".\loader.lau" cd. > loader.lau
cd ..

if not exist "C:\aim-flex" mkdir C:\aim-flex
mklink /J "C:\aim-flex\lua" "%cd%"
pause
