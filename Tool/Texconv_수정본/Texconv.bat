@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

REM 현재 스크립트 위치 기준 한 칸 상위 폴더로 이동
cd /d "%~dp0\.."

REM texconv.exe 경로 설정 (원래 위치 기준 상대 경로로 설정)
set TEXCONV="%~dp0texconv.exe"

for /R %%F in (*.png *.jpg) do (
    set "FULL=%%~F"
    set "OUT=%%~dpF"
    call set "OUT=!OUT:~0,-1!"

    set "REL=!FULL:%~dp0=!"

    echo ▶ 실행: %TEXCONV% -y -f BC3_UNORM -m 0 -o "!OUT!" "!FULL!"
    %TEXCONV% -y -f BC3_UNORM -m 0 -o "!OUT!" "!FULL!"
)

pause