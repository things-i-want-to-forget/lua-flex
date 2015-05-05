@echo off
mkdir "C:\aim-flex"
mklink /J "C:\aim-flex\lua" "%cd%" 
pause