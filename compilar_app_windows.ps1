# FinanZen - compila la app grafica para Windows -> build/finanzen_app.exe
# Antes, una sola vez: .\compilar_raylib_windows.ps1
$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
if (-not (Test-Path build/libraylib.a)) { & .\compilar_raylib_windows.ps1 }
g++ -std=c++17 -Wall -Wextra -O2 -static app/*.cpp motor/*.cpp -Iterceros/raylib/src -Lbuild -lraylib -lopengl32 -lgdi32 -lwinmm -o build/finanzen_app.exe
if ($LASTEXITCODE -ne 0) { throw "Fallo la compilacion" }
Write-Host "Listo: build/finanzen_app.exe (ejecutar desde la carpeta finanzen para que encuentre assets/)"
