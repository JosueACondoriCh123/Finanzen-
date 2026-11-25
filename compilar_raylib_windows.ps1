# FinanZen - compila raylib 5.5 para Windows (una sola vez) -> build/libraylib.a
# Usa el gcc de MSYS2 (C:\msys64\ucrt64\bin). Ejecutar desde la carpeta finanzen.
$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
New-Item -ItemType Directory -Force build\raylib_win | Out-Null
$src = "terceros/raylib/src"
$archivos = "rcore","rshapes","rtextures","rtext","rmodels","raudio","utils","rglfw"
foreach ($a in $archivos) {
    Write-Host "Compilando $a.c"
    gcc -c -O2 -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -I"$src" -I"$src/external/glfw/include" "$src/$a.c" -o "build/raylib_win/$a.o"
    if ($LASTEXITCODE -ne 0) { throw "Fallo al compilar $a.c" }
}
Remove-Item -ErrorAction SilentlyContinue build\libraylib.a
ar rcs build/libraylib.a (Get-ChildItem build\raylib_win\*.o | ForEach-Object { $_.FullName })
Write-Host "Listo: build/libraylib.a"
