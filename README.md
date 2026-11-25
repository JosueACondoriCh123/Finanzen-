# FinanZen

Sistema de gestión financiera personal en **C++**, con dos versiones que comparten el mismo código:

- **Consola** (`build/finanzen_consola.exe`): los 15 menús del proyecto.
- **App para celular Android** (`build/FinanZen.apk`): una app nativa hecha con [raylib](https://www.raylib.com/).

Integrantes: Joaquín 1 · Camila · Joaquín 2 · Hector · Josue.

## Cómo está armado

```
motor/      La lógica: el código de cada integrante, sin cin ni cout.
            La usan la consola y la app por igual.
consola/    Los menús por consola, con los textos originales de cada integrante.
app/        Las pantallas de la app, dibujadas con raylib.
originales/ Los archivos tal como los mandó cada integrante.
pruebas/    Pruebas automáticas del motor y una sesión de consola de ejemplo.
docs/       El diseño y el plan de esta fase.
terceros/   raylib 5.5 (licencia zlib). raygui 4.0 está incluido pero no se usa.
assets/     La fuente Inter (licencia OFL, ver Inter-LICENSE.txt).
```

### Dónde está el código de cada integrante

| Módulo | Integrante | Lógica (motor) | Consola | App |
|---|---|---|---|---|
| 1. Usuarios y autenticación | Joaquín 1 | `motor/usuarios.cpp` | `consola/menu_usuarios.cpp` | `app/pantallas_acceso.cpp`, `app/pantallas_perfil.cpp` |
| 2. Gestión de ingresos | Camila | `motor/ingresos.cpp` | `consola/menu_ingresos.cpp` | `app/pantallas_ingresos.cpp` |
| 3. Gestión de gastos | Joaquín 2 | `motor/gastos.cpp` | `consola/menu_gastos.cpp` | `app/pantallas_gastos.cpp` |
| 4. Planificación del ahorro | Hector | `motor/ahorro.cpp` | `consola/menu_ahorro.cpp`, `consola/leer.cpp` | `app/pantallas_ahorro.cpp` |
| 5. Seguimiento y reportes | Josue | `motor/reportes.cpp` | `consola/menu_reportes.cpp` | `app/pantallas_reportes.cpp`, `app/pantalla_inicio.cpp` |

Cada función del motor indica de dónde viene, con un comentario `// Origen: registrarIngreso() — Camila`. Los errores corregidos del código original están marcados con `// Corregido:`.

### Reglas de dinero (iguales en consola y app)

- **Saldo disponible** = ingresos − gastos − ahorros registrados. El ahorro no se cuenta como gasto, así no se descuenta dos veces.
- **Total ahorrado en la meta** = lo que ya estaba ahorrado al crearla + lo registrado en el módulo 5.
- No se puede registrar un ahorro mayor que lo que falta para la meta ni que el saldo disponible.

### Archivos de datos

`usuarios.txt`, `ingresos.txt`, `gastos.txt`, `metas.txt` y `ahorros.txt`: una línea por registro, campos separados por `|`. Se guardan después de cada cambio (primero en `.tmp` y luego se reemplazan, para no perder datos si algo falla).

### Cifrado de contraseñas

`motor/cifrado.cpp` sigue el ejemplo de cifrado del equipo: para cifrar se suma una llave a cada letra y para descifrar se resta. La llave es `LLAVE_CIFRADO = 100`. Las contraseñas solo admiten letras sin tilde, números y símbolos (códigos 32 a 126); al sumar 100 todas quedan entre 132 y 226, así el resultado nunca tiene un `|` ni un salto de línea que rompa el archivo.

- Al **guardar** `usuarios.txt`, la contraseña se escribe cifrada: abriendo el archivo no se puede leer.
- Al **cargar**, se descifra en memoria para poder iniciar sesión.

Es un cifrado por desplazamiento (tipo César): sirve para que la contraseña no quede a la vista, pero quien conozca la llave la descifra. Una app real usaría un hash criptográfico.

## Cómo compilar (Windows, PowerShell, desde esta carpeta)

Se necesita MSYS2 con `g++` en `C:\msys64\ucrt64\bin`. Para Android, además, Android Studio con el NDK 26.1 y build-tools 34.

```powershell
.\compilar_consola.ps1        # -> build\finanzen_consola.exe
.\compilar_app_windows.ps1    # -> build\finanzen_app.exe (para probar la app en la PC)
.\compilar_android.ps1        # -> build\FinanZen.apk
```

Instalar en un celular conectado por USB (con la depuración USB activada):

```powershell
adb install -r build\FinanZen.apk
```

Pruebas automáticas del motor:

```powershell
g++ -std=c++17 -Wall -Wextra pruebas/prueba_motor.cpp motor/*.cpp -o build/prueba_motor.exe; .\build\prueba_motor.exe
```

Capturas de todas las pantallas de la app con datos de ejemplo:

```powershell
.\build\finanzen_app.exe --capturas build\capturas
```
