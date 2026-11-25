# FinanZen móvil (fase 1) — plan de implementación

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Unir los 5 módulos C++ del equipo en un motor común, usado por una versión consola (15 menús) y una app gráfica nativa Android hecha con raylib/raygui.

**Architecture:** `motor/` contiene el código original de cada integrante partido en su parte lógica (sin `cin`/`cout`), con sus arreglos paralelos globales declarados en `motor/finanzen.h`. `consola/` conserva sus `cout`/`cin` originales y llama al motor. `app/` dibuja pantallas con raylib/raygui (`switch (pantalla)`) y llama al mismo motor. Persistencia en 5 `.txt`.

**Tech Stack:** C++17 (subconjunto sencillo), raylib 5.5, raygui 4.0, MSYS2 g++/gcc (Windows), Android NDK 26.1 clang + build-tools 34 (aapt2, zipalign, apksigner), JBR keytool, adb.

**Spec:** `finanzen/docs/2026-11-25-finanzen-movil-diseno.md`

## Global Constraints

- Subconjunto C++: `struct` solo los de raylib; arreglos fijos `MAX 100`, funciones, `switch`, `if`, `for`/`while`, `string`, `<iostream>`, `<fstream>`, `<cmath>`, `<iomanip>`. Excepciones aceptadas en la revisión final: `<limits>` (lectores de Hector, ya estaba en su original), `<cstdlib>` (convertir texto a número al leer los .txt), `<cstdio>` (renombrar archivos al guardar) y `<ctime>` (fecha de hoy en la app). Prohibido: clases propias, plantillas, `new`/`delete`, lambdas, STL salvo `string`.
- Se conservan los arreglos paralelos y los nombres originales de cada integrante; cada función del motor lleva `// Origen: <función>() — <integrante>`; cada arreglo de bug lleva `// Corregido: <qué>`.
- Primera línea de cada archivo propio: `// FinanZen — módulo <n> (<integrante>) · integrado con ayuda de Claude Code (IA)`.
- Moneda Bs con 2 decimales. Categorías de ingreso: `{"Sueldo","Freelance","Ventas","Otros"}`. Categorías de gasto: `{"Comida","Transporte","Servicios","Salidas","Salud","Otros"}`.
- Saldo disponible = ingresos − gastos − ahorros registrados. El ahorro nunca se guarda como gasto.
- Fechas `dd/mm/aaaa` validadas con `fechaValidaIng()` de Camila en todos los módulos.
- Textos: no vacíos y sin `|` (separador de archivos).
- Archivos: `usuarios.txt`, `ingresos.txt`, `gastos.txt`, `metas.txt`, `ahorros.txt`. Se guardan después de cada cambio exitoso.
- Android: paquete `bo.finanzen.app`, minSdk 24, targetSdk 34, `arm64-v8a`, vertical, NativeActivity sin Java ni Gradle.
- Todo vive en `finanzen/`, que es un repositorio git propio (no se sube a Walkback).
- Compilar pruebas del motor: `g++ -std=c++17 -Wall -Wextra pruebas/prueba_motor.cpp motor/*.cpp -o build/prueba_motor.exe` (desde `finanzen/`).

## Review Focus

1. Borrar un usuario que no es el último: las metas de Hector (por posición) y los límites deben moverse con él, y los registros de los demás no deben cambiar de dueño → prueba `prueba_borrado_en_medio` (Tarea 6).
2. Registrar un ahorro mayor que el saldo disponible o que lo que falta de la meta → se rechaza → `prueba_ahorro_sin_saldo` (Tarea 6).
3. Un texto con `|` o vacío → se rechaza en el motor → `prueba_texto_invalido` (Tarea 2) y la ida y vuelta de archivos con tildes y ñ → `prueba_archivos_tildes` (Tarea 7).
4. Funciones que reciben un índice de otro usuario (modificar o borrar el ingreso o gasto de otro) → devuelven `false` → `prueba_ajeno` (Tareas 3 y 4).
5. Abrir la app por primera vez sin archivos, o con archivos de otra versión cortados → arranca vacía, sin colgarse → `prueba_carga_sin_archivos` (Tarea 7).

---

### Tarea 1: Repositorio, terceros y originales

**Files:**
- Create: `finanzen/.gitignore` (`build/`), `finanzen/originales/` (copias de los 4 archivos recibidos), `finanzen/terceros/raylib/`, `finanzen/terceros/raygui/`, `finanzen/terceros/fuentes/Inter.ttf` (solo si el equipo lo autoriza), `finanzen/compilar_raylib_windows.ps1`, `finanzen/pruebas/hola_raylib.cpp`

- [ ] **Paso 1:** `git init finanzen`; copiar los originales como `originales/usuarios_joaquin1.cpp`, `ingresos_camila.cpp`, `gastos_joaquin2.cpp`, `ahorro_hector.cpp`, `reportes_josue.cpp`.
- [ ] **Paso 2:** descargar los zips de las etiquetas `5.5` de raysan5/raylib y `4.0` de raysan5/raygui en una carpeta temporal vacía; copiar `raylib/src` y `raygui/src/raygui.h` a `terceros/`. Mismo procedimiento para Inter si fue autorizado.
- [ ] **Paso 3:** `compilar_raylib_windows.ps1` compila con `gcc -c -O2 -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -Iterceros/raylib/src -Iterceros/raylib/src/external/glfw/include` los archivos `rcore.c rshapes.c rtextures.c rtext.c rmodels.c raudio.c utils.c rglfw.c` y arma `build/libraylib.a` con `ar rcs`.
- [ ] **Paso 4:** `hola_raylib.cpp` abre una ventana de 450×900, dibuja "FinanZen" y, con `--captura x.png`, guarda una captura en el segundo cuadro y sale. Compilar: `g++ -std=c++17 pruebas/hola_raylib.cpp -Iterceros/raylib/src -Lbuild -lraylib -lopengl32 -lgdi32 -lwinmm -o build/hola.exe`.
- [ ] **Paso 5:** ejecutar `build/hola.exe --captura build/hola.png`. Esperado: existe el PNG y muestra el texto.
- [ ] **Paso 6:** commit "Repositorio, raylib y originales del equipo".

### Tarea 2: Motor de usuarios (Joaquín 1) y base del motor

**Files:**
- Create: `motor/finanzen.h`, `motor/usuarios.cpp`, `motor/archivos.cpp` (por ahora `reiniciarTodo` y `textoValido`), `pruebas/prueba_motor.cpp`

**Interfaces:**
- Produce (`finanzen.h`): `#define MAX 100`; globales `extern string usu[MAX], con[MAX]; extern int usuId[MAX], total, proximoId, sesion;`
- `bool textoValido(string t)`: no vacío, no solo espacios, sin `|`.
- `bool registrar(string nombre, string clave)`: `false` si está lleno, el texto es inválido o el nombre se repite. Al registrar llama a `reiniciarMetaUsuario(total)` y a `reiniciarLimiteUsuario(total)` (definidos en las Tareas 4 y 5: hasta entonces, stubs vacíos en `archivos.cpp` marcados `// TEMPORAL`).
- `bool iniciar(string nombre, string clave)` → pone `sesion = usuId[i]`.
- `void cerrarSesion()`; `int indiceSesion()` (posición de `sesion` en `usu[]`, −1 si no hay); `string nombreSesion()`.
- `bool modificar(int pos, string nombre, string clave)`: posición 0..total−1, sin repetir el nombre de otro.
- `bool borrar(int pos)`: llama a `borrarDatosDeUsuario(pos)` (en `archivos.cpp`, que crece en cada tarea), corre los arreglos y, si era la sesión, `sesion = -1`.
- `void reiniciarTodo()`: deja todos los contadores en 0 y `sesion = -1`, `proximoId = 1`.

- [ ] **Paso 1:** en `prueba_motor.cpp`, `prueba_usuarios()`: registrar "Ana"/"1" → `true`; "Ana" otra vez → `false`; `registrar("", "x")` → `false`; `iniciar("Ana","mal")` → `false`; `iniciar("Ana","1")` → `true` y `nombreSesion()=="Ana"`; `modificar(0,"Ana B","2")` → `true`; `borrar(0)` → `true`, `total==0`, `sesion==-1`. `prueba_texto_invalido()`: `textoValido("a|b")`, `textoValido("   ")` → `false`. `main` llama a `reiniciarTodo()` antes de cada prueba e imprime "OK".
- [ ] **Paso 2:** compilar (comando de Global Constraints). Esperado: falla por funciones no definidas.
- [ ] **Paso 3:** implementar a partir de `originales/usuarios_joaquin1.cpp`, quitando `cin`/`cout`.
- [ ] **Paso 4:** compilar y ejecutar `build/prueba_motor.exe`. Esperado: `OK`.
- [ ] **Paso 5:** commit.

### Tarea 3: Motor de ingresos (Camila)

**Files:** Create `motor/ingresos.cpp`; Modify `motor/finanzen.h`, `motor/archivos.cpp`, `pruebas/prueba_motor.cpp`

**Interfaces:**
- Globales originales `ingDesc[], ingMonto[] (double), ingCat[], ingFecha[], ingTotal, nombreCatIng[4]` con `MAX_ING` 100, más `int ingUsuario[MAX_ING]`.
- `bool fechaValidaIng(string f)`: copia exacta del original.
- `bool agregarIngreso(string desc, double monto, int cat, string fecha)`: requiere sesión, monto > 0, cat 0..3, fecha válida, texto válido.
- `bool cambiarIngreso(int i, string desc, double monto, int cat, string fecha)`, `bool quitarIngreso(int i)`: `i` es índice global; `false` si no pertenece a `sesion`.
- `double sumaIngresos()`, `double sumaIngresosCat(int cat)`, `int cantidadIngresos()`: solo los de la sesión.
- `void borrarIngresosDeUsuario(int id)`.

- [ ] **Paso 1:** `prueba_ingresos()`: con sesión de "Ana", agregar 3000 Sueldo "01/11/2026" y 500 Ventas → `sumaIngresos()==3500`, `sumaIngresosCat(2)==500`; monto 0 → `false`; "31/02/2026" → `false`; `cambiarIngreso(0,...,3200,...)` → suma 3700. `prueba_ajeno()`: registrar "Beto", iniciar como él, `quitarIngreso(0)` → `false` y `cantidadIngresos()==0`.
- [ ] **Paso 2:** compilar. Esperado: falla.
- [ ] **Paso 3:** implementar desde `originales/ingresos_camila.cpp`; `borrarDatosDeUsuario` llama a `borrarIngresosDeUsuario(usuId[pos])`.
- [ ] **Paso 4:** compilar y ejecutar. Esperado: `OK`.
- [ ] **Paso 5:** commit.

### Tarea 4: Motor de gastos (Joaquín 2)

**Files:** Create `motor/gastos.cpp`; Modify `finanzen.h`, `archivos.cpp`, `prueba_motor.cpp`

**Interfaces:**
- Globales: `float gastos[MAX]` (su arreglo original) y `int nGastos` (`// Renombrado: era el parámetro n`), más `string gasDesc[MAX], gasFecha[MAX]; int gasCat[MAX], gasUsuario[MAX]; float limiteGasto[MAX]` (por posición de usuario); `string nombreCatGas[6]`.
- `bool registrarGasto(string desc, float monto, int cat, string fecha)`; `bool establecerLimite(float limite)` (≥ 0); `float limiteSesion()`.
- `float totalGastado()`, `float gastoMayor()`, `float gastoMenor()` (0 si no hay), `float totalGastadoCat(int cat)`, `int cantidadGastos()`: su bucle original, filtrando `gasUsuario[i]==sesion`.
- `bool modificarGasto(int i, string desc, float monto, int cat, string fecha)`, `bool eliminarGasto(int i)`: índice global, `false` si es ajeno.
- `void borrarGastosDeUsuario(int id)`, `void reiniciarLimiteUsuario(int pos)`, `void eliminarLimiteDeUsuario(int pos, int totalAntes)` (reemplaza el stub de la Tarea 2).

- [ ] **Paso 1:** `prueba_gastos()`: gastos 100 Comida, 40 Transporte, 250 Salidas → total 390, mayor 250, menor 40, `totalGastadoCat(0)==100`; `establecerLimite(800)` → `limiteSesion()==800`; `eliminarGasto` del de 40 → total 350. Extender `prueba_ajeno()` con `eliminarGasto` de un gasto de "Ana" estando como "Beto" → `false`.
- [ ] **Paso 2:** compilar. Esperado: falla.
- [ ] **Paso 3:** implementar desde `originales/gastos_joaquin2.cpp`.
- [ ] **Paso 4:** compilar y ejecutar. Esperado: `OK`.
- [ ] **Paso 5:** commit.

### Tarea 5: Motor de ahorro (Hector)

**Files:** Create `motor/ahorro.cpp`; Modify `finanzen.h`, `archivos.cpp`, `prueba_motor.cpp`

**Interfaces:**
- Globales originales `nombreMeta[], montoMeta[], ahorroActual[], mesesMeta[], tieneMeta[]` con `MAX_METAS_AHORRO`, sin `static` (`// Corregido: sin static para que la app y los archivos los lean`), indexados por **posición** de usuario.
- Copias exactas: `calcularFaltanteAhorro(int)`, `obtenerAhorroMensual(int)`, `reiniciarMetaUsuario(int)`, `eliminarMetaDeUsuario(int, int)`.
- Nuevas desde sus funciones con `cin`: `bool guardarNuevaMeta(int u, string nombre, double monto, double actual, int meses)` con sus límites (monto 0,01..1e9 con 2 decimales, actual 0..monto, meses 1..1200); `bool cambiarNombreMeta(int u, string)`, `bool cambiarMontoMeta(int u, double)` (≥ acumulado), `bool actualizarAhorroAcumulado(int u, double)`, `bool cambiarPlazoMeta(int u, int)`; `long long mesesConAporte(int u, double aporte)` (desde `simularAporteAhorro`); `int proyeccionAhorro(int u, double aporte[], double acumulado[], int max)` (desde `mostrarProyeccionAhorro`, devuelve la cantidad de filas). Todas `false`/0 si `!tieneMeta[u]`.
- `bool montoConDosDecimales(double v)` (su chequeo de centavos), usado también por ingresos y gastos.

- [ ] **Paso 1:** `prueba_ahorro()`: `guardarNuevaMeta(0,"Viaje",1000,250,3)` → `true`; `obtenerAhorroMensual(0)==250`; `mesesConAporte(0,200)==4`; `proyeccionAhorro` → 3 filas, último acumulado 1000; `cambiarMontoMeta(0,100)` → `false` (menor que lo acumulado); `guardarNuevaMeta(0,"x",10.555,0,1)` → `false`.
- [ ] **Paso 2:** compilar. Esperado: falla.
- [ ] **Paso 3:** implementar desde `originales/ahorro_hector.cpp`, moviendo sus lectores `leer…Ahorro` a la consola (Tarea 8).
- [ ] **Paso 4:** compilar y ejecutar. Esperado: `OK`.
- [ ] **Paso 5:** commit.

### Tarea 6: Motor de reportes (Josue) y borrado en cascada

**Files:** Create `motor/reportes.cpp`; Modify `finanzen.h`, `archivos.cpp`, `prueba_motor.cpp`

**Interfaces:**
- Globales `double ahoMonto[MAX]; string ahoFecha[MAX]; int ahoUsuario[MAX], ahoTotal;`
- `bool registro_ahorro(double monto, string fecha)`: requiere meta; monto > 0, ≤ `calcularFaltanteAhorro(pos)` y ≤ `saldoDisponible()`; suma a `ahorroActual[pos]`.
- `double progreso_ahorro()`: % con tope 100; −1 sin meta.
- `void resumen_financiero(double &ingresos, double &gastos, double &ahorrado, double &saldo)`.
- `double totalAhorradoRegistrado()`, `double saldoDisponible()`, `void borrarAhorrosDeUsuario(int id)`.
- `borrarDatosDeUsuario(pos)` queda completo: ingresos, gastos, ahorros (por id), meta y límite (por posición).

- [ ] **Paso 1:** `prueba_reportes()`: ingresos 3000, gastos 1000, meta 1000 con 250 iniciales; `registro_ahorro(500,…)` → `true`; resumen → ingresos 3000, gastos 1000, ahorrado 500, saldo 1500; `progreso_ahorro()==75`. `prueba_ahorro_sin_saldo()`: con saldo 100, `registro_ahorro(200,…)` → `false`; con faltante 50, `registro_ahorro(60,…)` → `false`. `prueba_borrado_en_medio()`: usuarios A, B, C, cada uno con meta y gasto; `borrar(1)` → C ahora en posición 1 conserva su meta, su gasto y su límite, y A no cambia.
- [ ] **Paso 2:** compilar. Esperado: falla.
- [ ] **Paso 3:** implementar desde `originales/reportes_josue.cpp` con los `// Corregido:` del diseño (§5.2).
- [ ] **Paso 4:** compilar y ejecutar. Esperado: `OK`.
- [ ] **Paso 5:** commit.

### Tarea 7: Persistencia

**Files:** Modify `motor/archivos.cpp`, `finanzen.h`, `prueba_motor.cpp`

**Interfaces:**
- `extern string carpetaDatos;` `bool guardarTodo();` `bool cargarTodo();` (devuelve `true` aunque falten archivos: arranca vacío).
- Formato: una línea por registro, campos con `|`. `usuarios.txt`: `id|nombre|clave|limite`. `metas.txt`: `posicion|nombre|monto|actual|meses`. `ingresos.txt`/`gastos.txt`: `usuario|desc|monto|cat|fecha`. `ahorros.txt`: `usuario|monto|fecha`. Montos con 2 decimales. Línea mal formada → se salta. `proximoId` = mayor id + 1.

- [ ] **Paso 1:** `prueba_archivos_tildes()`: con carpeta `build/prueba_datos/`, datos completos con nombre "Íñigo Peña" → `guardarTodo()`, `reiniciarTodo()`, `cargarTodo()` → mismos totales, nombre y progreso. `prueba_carga_sin_archivos()`: carpeta vacía → `cargarTodo()==true`, `total==0`; un `gastos.txt` con una línea cortada → se carga el resto.
- [ ] **Paso 2:** compilar. Esperado: falla.
- [ ] **Paso 3:** implementar con `ifstream`/`ofstream` y `getline(linea, campo, '|')`.
- [ ] **Paso 4:** compilar y ejecutar. Esperado: `OK`.
- [ ] **Paso 5:** commit.

### Tarea 8: Versión consola (15 menús)

**Files:** Create `consola/main_consola.cpp`, `consola/menu_usuarios.cpp`, `menu_ingresos.cpp`, `menu_gastos.cpp`, `menu_ahorro.cpp`, `menu_reportes.cpp`, `consola/leer.cpp` (los `leerEnteroAhorro`/`leerMontoAhorro`/`leerTextoAhorro` de Hector, renombrados `leerEntero`/`leerMonto`/`leerTexto`, más `leerFecha`), `pruebas/entrada_consola.txt`, `compilar_consola.ps1`

- [ ] **Paso 1:** `entrada_consola.txt`: registrar "Ana", iniciar sesión, cargar un ingreso de 3000, un gasto de 1000, crear meta de 1000 con 250, registrar un ahorro de 500, ver el resumen, salir.
- [ ] **Paso 2:** `main_consola.cpp`: `carpetaDatos="datos/"`, `cargarTodo()`, menú de inicio (1 Registrar, 2 Iniciar sesión, 0 Salir) → menú principal (1..5 módulos, 9 Cerrar sesión). Cada `menu_*.cpp` conserva los `cout` originales de su integrante y llama al motor; `guardarTodo()` después de cada cambio exitoso.
- [ ] **Paso 3:** `compilar_consola.ps1` → `build/finanzen_consola.exe`. Ejecutar `Get-Content pruebas/entrada_consola.txt | build/finanzen_consola.exe` con `carpetaDatos` en `build/datos_consola/` vacía. Esperado: la salida contiene "Saldo disponible: 1500.00" y "Progreso: 75.00%", sin bucles infinitos.
- [ ] **Paso 4:** commit.

### Tarea 9: App — base, teclado, acceso e inicio

**Files:** Create `app/main_app.cpp`, `app/estilo.h`, `app/estilo.cpp`, `app/teclado.cpp`, `app/pantallas_acceso.cpp`, `app/pantalla_inicio.cpp`, `compilar_app_windows.ps1`

**Interfaces:**
- `estilo.h`: colores (fondo `#0F1420`, tarjeta `#1A2233`, uno por módulo), `void dibujarTarjeta(Rectangle r, Color c)`, `bool botonGrande(Rectangle r, string texto, Color c)`, `void texto(string s, float x, float y, float tam, Color c)` (usa Inter si existe en `assets/`, si no la fuente por defecto), `void aviso(string mensaje)` (cartel 2 s).
- `int pantalla` con `enum` de constantes `PANT_BIENVENIDA, PANT_REGISTRO, PANT_LOGIN, PANT_INICIO, …` (agregadas en las Tareas 10–11); `main` con `switch (pantalla)`.
- Resolución virtual 450×900 en un `RenderTexture2D`, escalada al centro de la ventana; mouse/touch convertidos a coordenadas virtuales con `SetMouseScale`/`SetMouseOffset`.
- `teclado.cpp`: `const int TECLADO_TEXTO=0, TECLADO_NUMERO=1, TECLADO_FECHA=2;` `bool campoTexto(Rectangle r, string etiqueta, string &valor, int tipo, int id)` (al tocarlo, lo activa), `bool dibujarTeclado()` (dibuja el teclado del campo activo en la parte inferior; `true` al tocar "Listo"). En escritorio también acepta `GetCharPressed`/Backspace.
- `--capturas <carpeta>`: carga datos de demostración en una carpeta temporal, recorre cada pantalla existente, guarda `<pantalla>.png` y sale.

- [ ] **Paso 1:** implementar base, estilo, teclado, Bienvenida, Registro, Iniciar sesión e Inicio (saludo, saldo disponible, ingresos, gastos, barra de la meta, barra inferior de 5 pestañas), según el boceto del diseño §7.
- [ ] **Paso 2:** `compilar_app_windows.ps1` → `build/finanzen_app.exe`; ejecutar con `--capturas build/capturas`. Esperado: PNG de las 4 pantallas; revisarlas: nada se superpone, las tildes se ven bien y el teclado no tapa el campo activo.
- [ ] **Paso 3:** commit.

### Tarea 10: App — pantallas de Usuarios, Ingresos y Gastos

**Files:** Create `app/pantallas_perfil.cpp`, `app/pantallas_ingresos.cpp`, `app/pantallas_gastos.cpp`, `app/lista.cpp`

**Interfaces:**
- `lista.cpp`: `int listaDesplazable(Rectangle area, int cantidad, float altoFila, int &desplazamiento, void (*dibujarFila)(int fila, Rectangle r))`, que devuelve la fila tocada o −1 y se arrastra con el dedo. (Si un puntero a función se considera fuera del subconjunto, cada pantalla dibuja sus filas con su propio `for` y solo usa `int filaTocada(Rectangle area, int cantidad, float altoFila, int desplazamiento)`: preferir esta versión.)
- Cada pestaña muestra sus 3 menús como tarjetas: Perfil (Modificar datos · Borrar cuenta con confirmación · Cerrar sesión), Ingresos (Registrar · Consultar: todos, por categoría y resumen · Modificar o borrar), Gastos (Registrar uno, varios seguidos o límite · Consultar: lista, total, mayor, menor, por categoría con barra contra el límite · Modificar o eliminar).
- Cada guardado llama a la función del motor; si devuelve `false`, `aviso` con la causa; si `true`, `guardarTodo()`.

- [ ] **Paso 1:** implementar.
- [ ] **Paso 2:** agregar estas pantallas a `--capturas`, compilar y ejecutar. Esperado: un PNG por pantalla, con datos de demostración visibles y sin superposiciones.
- [ ] **Paso 3:** commit.

### Tarea 11: App — pantallas de Ahorro y Reportes

**Files:** Create `app/pantallas_ahorro.cpp`, `app/pantallas_reportes.cpp`

- [ ] **Paso 1:** Ahorro: Crear meta (formulario + confirmación, como `guardarNuevaMetaAhorro`) · Cálculo (recomendado, proyección mes a mes en tabla, simulador "¿cuánto podrías ahorrar al mes?" con `mesesConAporte`) · Modificar o reemplazar (las 6 opciones de Hector). Reportes: Registrar ahorro realizado · Progreso (barra grande con %, faltante y meses) · Resumen financiero (ingresos, gastos, ahorrado y saldo, con saldo negativo en rojo).
- [ ] **Paso 2:** agregar a `--capturas`, compilar y ejecutar. Esperado: los PNG de esta tarea muestran los números de la demostración iguales a los de la consola para los mismos datos.
- [ ] **Paso 3:** commit.

### Tarea 12: Compilar el .apk de Android

**Files:** Create `android/AndroidManifest.xml`, `android/icono.png` (dibujado por el propio script con raylib: logo FinanZen), `compilar_android.ps1`

- [ ] **Paso 1:** manifest: paquete `bo.finanzen.app`, `android:hasCode="false"`, `android.app.NativeActivity` con `android.app.lib_name = finanzen`, `screenOrientation="portrait"`, `extractNativeLibs="true"`, minSdk 24, targetSdk 34, etiqueta "FinanZen".
- [ ] **Paso 2:** `compilar_android.ps1`: compilar con el `clang++` del NDK (`--target=aarch64-linux-android24`) raylib (`-DPLATFORM_ANDROID -DGRAPHICS_API_OPENGL_ES2`), `android_native_app_glue.c`, `motor/*.cpp` y `app/*.cpp` → `lib/arm64-v8a/libfinanzen.so` (`-shared -llog -landroid -lEGL -lGLESv2 -lOpenSLES -lm -u ANativeActivity_onCreate`). En Android, `carpetaDatos` = `GetAndroidApp()->activity->internalDataPath + "/"`. Empaquetar con `aapt2 link` (con `android-34/android.jar`, `assets/` con la fuente), agregar la `.so` al zip, `zipalign -p 4` y `apksigner` con una clave de depuración creada con `keytool` del JBR en `build/debug.keystore`.
- [ ] **Paso 3:** ejecutar. Esperado: `build/FinanZen.apk`; `aapt2 dump badging` muestra `bo.finanzen.app` y `native-code: 'arm64-v8a'`.
- [ ] **Paso 4:** si hay un celular por USB (`adb devices`), `adb install -r build/FinanZen.apk`, abrirla, registrar un usuario, cerrar y reabrir: el usuario sigue. Si no hay celular, decirlo y dejar el comando.
- [ ] **Paso 5:** commit y `README.md` con cómo compilar cada versión y dónde está el código de cada integrante.
