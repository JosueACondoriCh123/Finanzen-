// FinanZen — app gráfica: ventana, navegación entre pantallas y bucle principal · integrado con ayuda de Claude Code (IA)
// Igual que los menús de consola: una variable "pantalla" y un switch que dibuja la que toca.
#include "app.h"

#if defined(PLATFORM_ANDROID)
#include <android_native_app_glue.h>
extern "C" struct android_app *GetAndroidApp(void); // de raylib: da la carpeta interna de la app
#endif

int pantalla = PANT_BIENVENIDA;

void irA(int nueva)
{
    cerrarTeclado();
    pantalla = nueva;
}

int padreDe(int p)
{
    switch (p)
    {
        case PANT_REGISTRO:
        case PANT_LOGIN:
            return PANT_BIENVENIDA;
        case PANT_PERFIL:
        case PANT_INGRESOS:
        case PANT_GASTOS:
        case PANT_AHORRO:
        case PANT_REPORTES:
            return PANT_INICIO;
        case PANT_PERFIL_EDITAR:
        case PANT_PERFIL_BORRAR:
            return PANT_PERFIL;
        case PANT_ING_FORM:
        case PANT_ING_CONSULTA:
        case PANT_ING_ADMIN:
            return PANT_INGRESOS;
        case PANT_GAS_FORM:
        case PANT_GAS_LIMITE:
        case PANT_GAS_CONSULTA:
        case PANT_GAS_ADMIN:
            return PANT_GASTOS;
        case PANT_META_FORM:
        case PANT_META_CALCULO:
        case PANT_META_MODIFICAR:
            return PANT_AHORRO;
        case PANT_REP_AHORRO:
        case PANT_REP_PROGRESO:
        case PANT_REP_RESUMEN:
            return PANT_REPORTES;
    }
    return p; // Bienvenida e Inicio no tienen "atrás"
}

void dibujarPantalla()
{
    // Sin sesión solo se ven las pantallas de acceso
    if (sesion == -1 && pantalla != PANT_BIENVENIDA && pantalla != PANT_REGISTRO && pantalla != PANT_LOGIN)
    {
        pantalla = PANT_BIENVENIDA;
    }
    switch (pantalla)
    {
        case PANT_BIENVENIDA: dibujarBienvenida(); break;
        case PANT_REGISTRO: dibujarRegistro(); break;
        case PANT_LOGIN: dibujarLogin(); break;
        case PANT_INICIO: dibujarInicio(); break;
        case PANT_PERFIL: dibujarPerfil(); break;
        case PANT_PERFIL_EDITAR: dibujarPerfilEditar(); break;
        case PANT_PERFIL_BORRAR: dibujarPerfilBorrar(); break;
        case PANT_INGRESOS: dibujarIngresos(); break;
        case PANT_ING_FORM: dibujarIngForm(); break;
        case PANT_ING_CONSULTA: dibujarIngConsulta(); break;
        case PANT_ING_ADMIN: dibujarIngAdmin(); break;
        case PANT_GASTOS: dibujarGastos(); break;
        case PANT_GAS_FORM: dibujarGasForm(); break;
        case PANT_GAS_LIMITE: dibujarGasLimite(); break;
        case PANT_GAS_CONSULTA: dibujarGasConsulta(); break;
        case PANT_GAS_ADMIN: dibujarGasAdmin(); break;
        case PANT_AHORRO: dibujarAhorro(); break;
        case PANT_META_FORM: dibujarMetaForm(); break;
        case PANT_META_CALCULO: dibujarMetaCalculo(); break;
        case PANT_META_MODIFICAR: dibujarMetaModificar(); break;
        case PANT_REPORTES: dibujarReportes(); break;
        case PANT_REP_AHORRO: dibujarRepAhorro(); break;
        case PANT_REP_PROGRESO: dibujarRepProgreso(); break;
        case PANT_REP_RESUMEN: dibujarRepResumen(); break;
    }
    dibujarTeclado();
    dibujarAviso();
}

// Datos de demostración para las capturas automáticas (--capturas)
void cargarDemo()
{
    reiniciarTodo();
    registrar("Camila Rojas", "1234");
    iniciar("Camila Rojas", "1234");
    agregarIngreso("Sueldo de noviembre", 3500, 0, "01/11/2026");
    agregarIngreso("Diseño de un logo", 800, 1, "10/11/2026");
    registrarGasto("Alquiler", 1200, 2, "02/11/2026");
    registrarGasto("Supermercado", 450, 0, "05/11/2026");
    registrarGasto("Micro y trufi", 120, 1, "08/11/2026");
    registrarGasto("Cine con amigos", 90, 3, "12/11/2026");
    registrarGasto("Farmacia", 60, 4, "14/11/2026");
    establecerLimite(2500);
    guardarNuevaMeta(0, "Viaje a Cochabamba", 2000, 400, 6);
    registro_ahorro(300, "15/11/2026");
}

void exportarCaptura(RenderTexture2D lienzo, string archivo)
{
    Image img = LoadImageFromTexture(lienzo.texture);
    ImageFlipVertical(&img);
    ExportImage(img, archivo.c_str());
    UnloadImage(img);
}

int main(int argc, char *argv[])
{
    SetTraceLogLevel(LOG_WARNING);
    string carpetaCapturas = "";
    if (argc == 3 && string(argv[1]) == "--capturas")
    {
        carpetaCapturas = string(argv[2]) + "/";
    }
#if !defined(PLATFORM_ANDROID)
    // Genera el ícono de la app (192x192) para el .apk
    if (argc == 3 && string(argv[1]) == "--icono")
    {
        SetConfigFlags(FLAG_WINDOW_HIDDEN);
        InitWindow(192, 192, "icono");
        cargarEstilo();
        RenderTexture2D rt = LoadRenderTexture(192, 192);
        BeginTextureMode(rt);
        ClearBackground(FONDO);
        dibujarLogo(96, 96, 84);
        EndTextureMode();
        exportarCaptura(rt, argv[2]);
        UnloadRenderTexture(rt);
        descargarEstilo();
        CloseWindow();
        return 0;
    }
#endif

#if defined(PLATFORM_ANDROID)
    InitWindow(0, 0, "FinanZen");
    carpetaDatos = string(GetAndroidApp()->activity->internalDataPath) + "/";
#else
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(405, 810, "FinanZen");
    MakeDirectory("datos");
    carpetaDatos = "datos/";
#endif
    SetExitKey(KEY_NULL); // el botón atrás de Android no cierra la app
    SetTargetFPS(60);
    cargarEstilo();

    RenderTexture2D lienzo = LoadRenderTexture(ANCHO, ALTO);
    SetTextureFilter(lienzo.texture, TEXTURE_FILTER_BILINEAR);

    if (carpetaCapturas != "")
    {
        MakeDirectory(carpetaCapturas.c_str());
        carpetaDatos = carpetaCapturas; // los datos de demo se guardan junto a las capturas
        cargarDemo();
    }
    else
    {
        cargarTodo();
    }

    int capturaActual = 0;
    int cuadro = 0;

    while (!WindowShouldClose())
    {
        // Escala la pantalla virtual 450x900 al tamaño real, centrada
        float escalaX = (float)GetScreenWidth() / ANCHO;
        float escalaY = (float)GetScreenHeight() / ALTO;
        float escala = escalaX < escalaY ? escalaX : escalaY;
        float offX = (GetScreenWidth() - ANCHO * escala) / 2;
        float offY = (GetScreenHeight() - ALTO * escala) / 2;
        SetMouseOffset((int)-offX, (int)-offY);
        SetMouseScale(1 / escala, 1 / escala);

        // Modo capturas: recorre todas las pantallas y guarda un PNG de cada una
        if (carpetaCapturas != "" && cuadro == 0)
        {
            if (capturaActual >= TOTAL_PANTALLAS + 1)
            {
                break;
            }
            int p = capturaActual < TOTAL_PANTALLAS ? capturaActual : PANT_ING_FORM;
            irA(p);
            if (p == PANT_BIENVENIDA || p == PANT_REGISTRO || p == PANT_LOGIN) cerrarSesion();
            else iniciar("Camila Rojas", "1234");
            if (p == PANT_ING_FORM) prepararIngreso(-1);
            if (p == PANT_GAS_FORM) prepararGasto(-1);
            if (p == PANT_META_FORM || p == PANT_META_MODIFICAR) prepararMeta();
            if (capturaActual == TOTAL_PANTALLAS) campoActivo = 1; // teclado abierto
        }

        actualizarToques();
        BeginTextureMode(lienzo);
        ClearBackground(FONDO);
        dibujarPantalla();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(lienzo.texture, Rectangle{0, 0, (float)ANCHO, (float)-ALTO},
                       Rectangle{offX, offY, ANCHO * escala, ALTO * escala}, Vector2{0, 0}, 0, WHITE);
        EndDrawing();

        if (carpetaCapturas != "")
        {
            cuadro++;
            if (cuadro == 3)
            {
                string nombre = (capturaActual < 10 ? "0" : "") + to_string(capturaActual);
                if (capturaActual == TOTAL_PANTALLAS) nombre += "_teclado";
                exportarCaptura(lienzo, carpetaCapturas + nombre + ".png");
                capturaActual++;
                cuadro = 0;
            }
        }
    }

    UnloadRenderTexture(lienzo);
    descargarEstilo();
    CloseWindow();
    return 0;
}
