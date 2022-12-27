@echo off

setlocal

if %1.==. (
  echo Provide a test name to build!
  exit /b 1
)

echo ---- Test: %1

cl.exe /nologo /I..\ /std:c++17 /EHsc %1\%1.cpp /Fe..\bin\test_%1.exe /Fo..\bin\test_%1.obj SDL2.lib SDL2main.lib Shell32.lib /link /SUBSYSTEM:CONSOLE

cd %1
..\..\bin\test_%1.exe
set ret=%ERRORLEVEL%
cd ..

if %ret%==0 (
  echo ==== PASSED
) else (
  echo ==== FAILED
)

exit /b %ret%

endlocal
