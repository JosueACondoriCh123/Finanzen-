# FinanZen - compila la version consola -> build/finanzen_consola.exe
$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
New-Item -ItemType Directory -Force build | Out-Null
g++ -std=c++17 -Wall -Wextra -O2 -static consola/*.cpp motor/*.cpp -o build/finanzen_consola.exe
if ($LASTEXITCODE -ne 0) { throw "Fallo la compilacion" }
Write-Host "Listo: build/finanzen_consola.exe"
