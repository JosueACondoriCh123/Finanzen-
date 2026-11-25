# FinanZen móvil — diseño (fase 1)

Fecha: 25/11/2026 · Equipo: Joaquín 1, Camila, Joaquín 2, Hector, Josue

## 1. Intención

FinanZen es una app de finanzas personales para personas con ingreso mensual. El equipo ya escribió cinco módulos en C++ como programas de consola sueltos. Esta fase:

1. **Cuadra y une** los cinco módulos en un solo programa coherente, donde cada dato pertenece al usuario que inició sesión.
2. Le da a FinanZen una **interfaz gráfica nativa para celular Android**, escrita en el mismo C++ sencillo, con la biblioteca **raylib + raygui**.

Por qué: el docente revisa el código C++ y además quiere ver la app funcionando en el celular. El equipo quiere que el C++ siga siendo "su" lenguaje (arreglos, funciones, `switch`, `for`/`while`, `string`) sin renunciar a una app que se vea moderna.

## 2. Objetivos, no-objetivos y anti-objetivos

**Objetivos**
- Un único "motor" en C++ por módulo (funciones sin `cin`/`cout`) usado por dos programas:
  - `finanzen_consola`: los 15 menús por consola (lo que corre y lee el docente).
  - `finanzen_app`: la app gráfica (Windows para probar, `.apk` para Android).
- Los 15 menús del documento del proyecto existen en las dos versiones.
- Los datos se conservan al cerrar (archivos `.txt`, como dice el documento).
- Cada integrante sigue reconociendo su módulo: mismos nombres de funciones donde se pueda, mismo estilo, cabecera con su nombre.

**No-objetivos (fase 2 o futuro)**
- Funciones creativas acordadas para la fase 2: "Hoy podés gastar", racha, semáforo por categoría, día de cobro, nivel y medallas, simulador "¿y si…?" ampliado.
- Empresas afiliadas, nube, sincronización entre celulares, iPhone.

**Anti-objetivos (aunque funcione, sería un fracaso)**
- Código que el equipo no pueda explicar en la defensa: nada de clases propias, herencia, plantillas, punteros propios, `new`/`delete`, lambdas ni STL salvo `string`.
- Que la app y la consola den números distintos para los mismos datos.
- Que el resumen descuente dos veces el dinero ahorrado.

## 3. Restricciones

- **Lenguaje:** C++17 con el subconjunto sencillo del equipo: `struct`, arreglos de tamaño fijo (`MAX` 100), funciones, `switch`, `if`, `for`/`while`, `string`, `<iostream>`, `<fstream>`, `<cmath>`, `<iomanip>`. Los tipos de raylib (`Rectangle`, `Color`, `Vector2`) se usan como `struct` comunes.
- **Biblioteca gráfica:** raylib 5.5 + raygui 4.0, descargadas de sus repositorios oficiales de GitHub (licencia zlib), dentro de `finanzen/terceros/`.
- **Herramientas que ya están en la laptop:** MSYS2 `g++` (para Windows), Android SDK con NDK 26.1, CMake 3.22, build-tools 34/36/37, JDK de Android Studio y `adb`. No se instala nada más sin preguntar.
- **Teclado:** raylib no abre el teclado de Android, así que la app dibuja su propio teclado: numérico para montos y QWERTY para textos.
- **Moneda:** bolivianos (Bs), dos decimales.
- **Ubicación:** todo vive en `finanzen/`. *Nuestra decisión:* esta carpeta está dentro del repositorio de Walkback solo como lugar de trabajo; no se sube a ese repositorio. Se recomienda moverla a un repositorio propio de FinanZen.

## 4. Enfoques considerados

| Enfoque | A favor | En contra | Decisión |
|---|---|---|---|
| A. Consola corriendo en el celular (Cxxdroid) | 100 % su código, sin riesgo | No parece una app | Descartado |
| B. Motor C++ compilado a WebAssembly + pantalla HTML | Se ve como app, C++ en el centro | La pantalla no es C++ y no es nativa | Descartado por el equipo: piden nativa en C++ |
| **C. Motor C++ + pantalla nativa con raylib/raygui** | Todo en C++ sencillo, `.apk` real, se prueba en Windows | Hay que dibujar teclado propio | **Elegido por el equipo** |
| D. Qt / QML | La más pulida, teclado del sistema | Clases, otro lenguaje, descarga de varios GB | Descartado |
| E. SDL2 + Dear ImGui | Teclado del sistema | Aspecto de herramienta, código más difícil | Descartado |

## 5. Cómo se cuadran los módulos

### 5.1 Problemas encontrados en el código recibido
1. Solo el módulo 4 asocia los datos a un usuario; ingresos y gastos son globales.
2. Gastos guarda solo montos (sin descripción, categoría ni fecha) y su menú nunca termina (`while (opcion != 4)` cuando la salida es 0).
3. El módulo 5 no compila (`float gasto[];`, variables sin inicializar) y calcula el ahorro como "sueldo − gastos" en lugar de usar el ahorro registrado.
4. Cada archivo tiene su propio `main`; no hay menú principal ni sesión.
5. El documento pide archivos `.txt` y ningún módulo guarda nada.
6. La lectura de datos es distinta en cada módulo. Solo el módulo 4 valida bien la entrada.

### 5.2 El código del equipo es la lógica real (pedido del equipo, 25/11)

La app y la consola no usan una reescritura: usan **el código que mandó cada integrante**, con sus mismos arreglos, nombres de funciones y cálculos. Cada función original se parte en dos:

- **Parte lógica** (va al motor y la usan la app y la consola): validaciones, cálculos y escritura en los arreglos. Conserva el nombre original o uno muy cercano, y su cuerpo se mantiene lo más igual posible.
- **Parte de pantalla** (solo consola): los `cin`/`cout` originales, que ahora llaman a la parte lógica.

Ejemplo con el módulo de Camila:

```cpp
// motor/ingresos.cpp: lógica de registrarIngreso() de Camila
bool agregarIngreso(string desc, double monto, int cat, string fecha) {
    if (ingTotal >= MAX_ING) return false;          // su control de espacio
    if (monto <= 0 || !fechaValidaIng(fecha)) return false; // sus validaciones
    ingDesc[ingTotal] = desc; ingMonto[ingTotal] = monto; ...
    ingTotal++; return true;
}
// consola: su registrarIngreso() con los mismos cout, que llama a agregarIngreso()
```

Cada función del motor lleva un comentario de origen, por ejemplo `// Origen: registrarIngreso() — Camila`. Así cada integrante puede señalar en la defensa dónde está su código dentro de la app.

**Datos:** se conservan los **arreglos paralelos** de cada integrante (`usu[]`/`con[]`, `ingDesc[]`/`ingMonto[]`/`ingCat[]`/`ingFecha[]`, `gastos[]`, los arreglos `static` de metas de Hector). No se pasan a `struct`: ese cambio reemplaza al modelo con `struct` que proponía la primera versión de este diseño. A cada arreglo paralelo se le agrega lo mínimo para unir los módulos:

| Módulo | Arreglos originales | Se agrega |
|---|---|---|
| 1 Usuarios | `usu[]`, `con[]`, `total` | `usuId[]` (id fijo de cada usuario), `int sesion` (id conectado, −1 si no hay nadie), `proximoId` |
| 2 Ingresos | `ingDesc[]`, `ingMonto[]`, `ingCat[]`, `ingFecha[]`, `ingTotal` | `ingUsuario[]` |
| 3 Gastos | `gastos[]` (montos), `n`, `limite` | `gasDesc[]`, `gasCat[]`, `gasFecha[]`, `gasUsuario[]`; `limite` pasa a ser un arreglo por usuario |
| 4 Ahorro | `nombreMeta[]`, `montoMeta[]`, `ahorroActual[]`, `mesesMeta[]`, `tieneMeta[]` (por posición del usuario) | nada: su `reiniciarMetaUsuario()` y `eliminarMetaDeUsuario()` ya resuelven el alta y el borrado |
| 5 Reportes | `registro_ahorro`, `progreso_ahorro`, `resumen_financiero` | `ahoMonto[]`, `ahoFecha[]`, `ahoUsuario[]`, `ahoTotal` |

Los registros de ingresos, gastos y ahorros guardan el **id** del usuario, que no cambia aunque se borre a otro. Las metas de Hector siguen indexadas por **posición**, como él las diseñó, y su `eliminarMetaDeUsuario()` las corre junto con los usuarios.

**Arreglos necesarios del código original** (cada uno se marca con un comentario `// Corregido:`):
- Gastos: el menú sale con 0, no con 4.
- Módulo 5: arreglo sin tamaño y variables sin inicializar.
- Módulo 5: el ahorro se toma de lo registrado, no de "sueldo − gastos".
- Módulo 1: el submenú "Mod meta" llama a la modificación de metas de Hector.

Categorías:
- Ingresos (Camila, sin cambios): Sueldo, Freelance, Ventas, Otros.
- Gastos (nuestra decisión, reemplaza "solo monto"): Comida, Transporte, Servicios, Salidas, Salud, Otros.

### 5.3 Reglas de dinero (las mismas en consola y app)
- **Total ahorrado en la meta** = `inicial` + suma de los ahorros registrados (módulo 5).
- **Saldo disponible** = ingresos − gastos − ahorros registrados. El ahorro **no** se guarda como gasto: así no se descuenta dos veces.
- **Ahorro mensual recomendado** (Hector, sin cambios): falta ÷ meses, redondeado hacia arriba en centavos.
- **Progreso de la meta** = total ahorrado ÷ objetivo × 100, con tope en 100 %.

### 5.4 Los 15 menús → funciones del motor

| Módulo (dueño) | Menú | Funciones del motor (ejemplos) |
|---|---|---|
| 1 Usuarios (Joaquín 1) | Registro · Inicio de sesión · Perfil (modificar, borrar cuenta, cerrar sesión) | `registrarUsuario`, `iniciarSesion`, `modificarUsuario`, `borrarUsuario`, `cerrarSesion` |
| 2 Ingresos (Camila) | Registro · Consulta (todos / por categoría / resumen) · Modificación y borrado | `registrarIngreso`, `contarIngresos`, `totalIngresos`, `totalIngresosCat`, `modificarIngreso`, `borrarIngreso` |
| 3 Gastos (Joaquín 2) | Registro (uno, varios, límite) · Consulta y clasificación (lista, total, mayor, menor, por categoría) · Modificación o eliminación | `registrarGasto`, `fijarLimite`, `totalGastos`, `gastoMayor`, `gastoMenor`, `totalGastosCat`, `modificarGasto`, `borrarGasto` |
| 4 Ahorro (Hector) | Crear meta · Cálculo mensual (recomendado, proyección, simulación) · Modificar o reemplazar | `crearMeta`, `ahorroMensual`, `mesesConAporte`, `cambiarMeta…` (su lógica en centavos se conserva) |
| 5 Reportes (Josue) | Registrar ahorro realizado · Progreso de la meta · Resumen financiero | `registrarAhorro`, `totalAhorrado`, `progresoMeta`, `saldoDisponible` |

Las funciones del motor devuelven `bool` o un número; nunca leen ni escriben en la pantalla. Las validaciones (monto > 0 con dos decimales, fecha real con `fechaValidaIng` de Camila, nombre no vacío, usuario único) viven en el motor, para que las dos versiones las cumplan igual.

Borrar un usuario borra también sus ingresos, gastos, meta y ahorros.

### 5.5 Persistencia
Archivos del documento: `usuarios.txt`, `ingresos.txt`, `gastos.txt`, `metas.txt`, `ahorros.txt`. Un registro por línea, campos separados por `|` (los textos no pueden contener `|`). Se cargan al abrir y se guardan después de cada cambio. En Android van a la carpeta interna de la app. Las contraseñas se guardan en texto plano, como en el código del equipo (es un proyecto académico; se dice así en la defensa).

## 6. Versión consola
`consola/main_consola.cpp`: menú de inicio (registrarse / iniciar sesión / salir) → menú principal con los 5 módulos → los 3 menús de cada uno. Los textos de los menús se conservan lo más posible. Las funciones de lectura robusta del módulo 4 (`leerEntero`, `leerMonto`, `leerTexto`) se usan en todos los menús.

## 7. Versión app (raylib)

**Pantalla:** vertical. Se dibuja en una resolución virtual de 450 × 900 y se escala al celular. Estilo propio: fondo oscuro, tarjetas redondeadas, un color por módulo y tipografía grande.

**Navegación:** `int pantalla` y un `switch (pantalla)` que llama a `dibujarPantallaX()`, igual que los menús de consola.

```
 Bienvenida ─► Registro / Iniciar sesión
                    │
                    ▼
 ┌──────────── Inicio ─────────────┐
 │ Hola, Camila                    │
 │ Saldo disponible   Bs 1.240,50  │
 │ Ingresos  Bs 3.000  Gastos  1.5k│
 │ Meta: Viaje  ▓▓▓▓░░ 62 %        │
 ├─────────────────────────────────┤
 │ [Perfil][Ingresos][Gastos][Ahorro][Reportes] │  ← barra inferior
 └─────────────────────────────────┘
```

Cada pestaña muestra sus 3 menús como tarjetas grandes. Cada menú abre:
- **Formularios** (campos + teclado propio + botón Guardar),
- **Listas** desplazables con el dedo, donde tocar un elemento permite modificarlo o borrarlo (con confirmación),
- **Resúmenes** con números grandes y barras.

Los mensajes de error del motor se muestran como aviso en la pantalla.

**Teclado propio:** numérico (0-9, coma, borrar) para montos; QWERTY con mayúsculas, espacio y borrar para textos. La fecha viene precargada con la de hoy y se puede editar con el teclado numérico (dd/mm/aaaa).

## 8. Estructura de carpetas
```
finanzen/
  docs/                 este diseño
  motor/                datos.h + usuarios.cpp ingresos.cpp gastos.cpp ahorro.cpp reportes.cpp archivos.cpp
  consola/              main_consola.cpp
  app/                  main_app.cpp, pantallas_*.cpp, teclado.cpp, estilo.h
  pruebas/              prueba_motor.cpp (comprueba las reglas de dinero con assert)
  android/              AndroidManifest.xml, ícono, script de compilación del .apk
  terceros/             raylib, raygui (descargados)
  compilar_*.ps1        un script por versión
```

## 9. Cómo se prueba
1. `prueba_motor.cpp`: registrar usuarios, ingresos, gastos, meta y ahorros; comprobar totales, saldo (sin doble descuento), progreso, borrado en cascada y guardado/carga de archivos.
2. La consola recorre los 15 menús con una entrada de prueba.
3. La app en Windows: se revisa cada pantalla con capturas.
4. El `.apk` se instala con `adb` en el celular de un integrante y lo prueba una persona del equipo.

## 10. Decisiones que quedan para quien construye
- Compilar el `.apk` con un script propio sobre el NDK (NativeActivity, sin Java ni Gradle) o con un proyecto de Android Studio: se prefiere el script, si funciona.
- Los colores exactos, los íconos (dibujados con formas de raylib, sin imágenes de terceros) y el nombre del paquete Android (`bo.finanzen.app`).
- La versión mínima de Android: API 24 (Android 7), salvo que algún celular del equipo sea más viejo.
