# FinanZen - compila la app para Android -> build/FinanZen.apk
# Usa lo que ya trae Android Studio: NDK 26.1 (clang), build-tools 34 (aapt2, zipalign, apksigner) y su Java (keytool).
# Ejecutar desde la carpeta finanzen:  .\compilar_android.ps1
# Para instalar en un celular conectado por USB:  adb install -r build\FinanZen.apk
$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

$SDK = "$env:LOCALAPPDATA\Android\Sdk"
$NDK = "$SDK\ndk\26.1.10909125"
$TOOLS = "$SDK\build-tools\34.0.0"
$JAR = "$SDK\platforms\android-34\android.jar"
$JAVA = "C:\Program Files\Android\Android Studio\jbr\bin"
$CLANG = "$NDK\toolchains\llvm\prebuilt\windows-x86_64\bin\clang.exe"
$CLANGXX = "$NDK\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe"
$OBJETIVO = "--target=aarch64-linux-android24"
$GLUE = "$NDK\sources\android\native_app_glue"
$B = "build\android"

foreach ($p in $NDK, $TOOLS, $JAR, $JAVA) { if (-not (Test-Path $p)) { throw "No encuentro $p" } }
New-Item -ItemType Directory -Force "$B\obj", "$B\apk\lib\arm64-v8a", "$B\assets", "$B\res\mipmap" | Out-Null

function Revisar($msg) { if ($LASTEXITCODE -ne 0) { throw $msg } }

# 1) raylib para Android (una sola vez)
$src = "terceros/raylib/src"
# Se compila cada archivo que falte (si una compilación anterior quedó a medias, se completa)
foreach ($a in "rcore", "rshapes", "rtextures", "rtext", "rmodels", "raudio", "utils") {
    if (-not (Test-Path "$B\obj\$a.o")) {
        Write-Host "raylib: $a.c"
        & $CLANG $OBJETIVO -c -O2 -fPIC -DPLATFORM_ANDROID -DGRAPHICS_API_OPENGL_ES2 -I"$src" -I"$GLUE" "$src/$a.c" -o "$B\obj\$a.o"
        if ($LASTEXITCODE -ne 0) { Remove-Item -ErrorAction SilentlyContinue "$B\obj\$a.o"; throw "Fallo raylib $a.c" }
    }
}
if (-not (Test-Path "$B\obj\glue.o")) {
    & $CLANG $OBJETIVO -c -O2 -fPIC -I"$GLUE" "$GLUE\android_native_app_glue.c" -o "$B\obj\glue.o"
    if ($LASTEXITCODE -ne 0) { Remove-Item -ErrorAction SilentlyContinue "$B\obj\glue.o"; throw "Fallo native_app_glue" }
}

# 2) FinanZen: motor del equipo + app -> libfinanzen.so
Write-Host "FinanZen: motor + app"
$fuentes = (Get-ChildItem app\*.cpp, motor\*.cpp | ForEach-Object { $_.FullName })
$objs = (Get-ChildItem "$B\obj\*.o" | ForEach-Object { $_.FullName })
& $CLANGXX $OBJETIVO -std=c++17 -O2 -fPIC -shared -DPLATFORM_ANDROID -I"$src" -I"$GLUE" $fuentes $objs `
    -o "$B\apk\lib\arm64-v8a\libfinanzen.so" -static-libstdc++ `
    -llog -landroid -lEGL -lGLESv2 -lOpenSLES -lm -u ANativeActivity_onCreate "-Wl,-z,max-page-size=16384"
Revisar "Fallo al compilar FinanZen para Android"

# 3) Recursos: fuente Inter (assets) e ícono
Remove-Item -Recurse -Force "$B\assets\assets" -ErrorAction SilentlyContinue
Copy-Item assets\Inter-Regular.ttf, assets\Inter-Bold.ttf "$B\assets\" -Force
if (-not (Test-Path build\finanzen_app.exe)) { & .\compilar_app_windows.ps1 }
Remove-Item -ErrorAction SilentlyContinue "$B\res\mipmap\ic_launcher.png"
& .\build\finanzen_app.exe --icono "$B\res\mipmap\ic_launcher.png"
if ($LASTEXITCODE -ne 0 -or -not (Test-Path "$B\res\mipmap\ic_launcher.png")) { throw "Fallo al generar el icono" }
& "$TOOLS\aapt2.exe" compile "$B\res\mipmap\ic_launcher.png" -o "$B\res_compilados.zip"
Revisar "Fallo aapt2 compile"

# 4) Empaquetar: manifest + ícono + assets, luego la .so
& "$TOOLS\aapt2.exe" link -o "$B\sin_lib.apk" --manifest android\AndroidManifest.xml -I "$JAR" -A "$B\assets" "$B\res_compilados.zip"
Revisar "Fallo aapt2 link"
Add-Type -AssemblyName System.IO.Compression, System.IO.Compression.FileSystem
Copy-Item "$B\sin_lib.apk" "$B\sin_firmar.apk" -Force
$zip = [System.IO.Compression.ZipFile]::Open((Resolve-Path "$B\sin_firmar.apk"), "Update")
[System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile($zip, (Resolve-Path "$B\apk\lib\arm64-v8a\libfinanzen.so"), "lib/arm64-v8a/libfinanzen.so") | Out-Null
$zip.Dispose()

# 5) Alinear y firmar con una clave de depuración
& "$TOOLS\zipalign.exe" -f -p 4 "$B\sin_firmar.apk" "$B\alineado.apk"
Revisar "Fallo zipalign"
if (-not (Test-Path build\debug.keystore)) {
    & "$JAVA\keytool.exe" -genkeypair -keystore build\debug.keystore -storepass android -keypass android `
        -alias finanzen -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=FinanZen Debug"
    Revisar "Fallo keytool"
}
$env:JAVA_HOME = Split-Path $JAVA
& "$TOOLS\apksigner.bat" sign --ks build\debug.keystore --ks-pass pass:android --ks-key-alias finanzen --out build\FinanZen.apk "$B\alineado.apk"
Revisar "Fallo apksigner"
Write-Host "Listo: build\FinanZen.apk"
