mkdir scripts
cd scripts
cd. > loader.lau
cd ..

if not exist "C:\aim-flex" mkdir C:\aim-flex
mklink /J "C:\aim-flex\lua" "%cd%"
pause
