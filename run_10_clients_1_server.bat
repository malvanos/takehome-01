@echo off
setlocal

rem === Change this to Debug or Release ===
set BUILD_CONFIG=Debug
set EXE_PATH=x64\%BUILD_CONFIG%

start "Server" cmd /k "%EXE_PATH%\Server.exe"

for /L %%i in (1,1,10) do (
    start "Client %%i" cmd /k "%EXE_PATH%\Client.exe"
)

endlocal