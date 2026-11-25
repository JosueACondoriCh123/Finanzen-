// FinanZen — módulo 1 (Joaquín 1) en la app: bienvenida, registro e inicio de sesión · integrado con ayuda de Claude Code (IA)
// Usa registrar() e iniciar() de motor/usuarios.cpp.
#include "app.h"

string accNombre = "";
string accClave = "";
string accClave2 = "";

void limpiarAcceso()
{
    accNombre = "";
    accClave = "";
    accClave2 = "";
}

// Logo: un círculo "zen" con una moneda adentro
void dibujarLogo(float cx, float cy, float r)
{
    DrawRing(Vector2{cx, cy}, r - 10, r, 20, 340, 48, COLOR_ING);
    DrawCircle((int)cx, (int)cy, r - 24, ColorAlpha(COLOR_AHO, 0.22f));
    DrawCircle((int)cx, (int)cy, r - 38, COLOR_AHO);
    textoNegrita("Bs", cx - anchoTexto("Bs", r * 0.42f, true) / 2, cy - r * 0.24f, r * 0.42f, FONDO);
}

void dibujarBienvenida()
{
    dibujarLogo(ANCHO / 2, 250, 92);
    textoCentrado("FinanZen", 380, 52, TEXTO, true);
    textoCentrado("Tu dinero, en calma.", 446, 22, TEXTO2, false);

    string puntos[3] = {"Registrá ingresos y gastos", "Planificá tus metas de ahorro", "Mirá tu progreso de un vistazo"};
    Color colores[3] = {COLOR_ING, COLOR_AHO, COLOR_REP};
    for (int i = 0; i < 3; i++)
    {
        DrawCircle(70, 530 + i * 40, 6, colores[i]);
        texto(puntos[i], 90, 520 + i * 40, 18, TEXTO);
    }

    if (botonGrande(Rectangle{24, 690, ANCHO - 48, 60}, "Crear cuenta", COLOR_ING))
    {
        limpiarAcceso();
        irA(PANT_REGISTRO);
    }
    if (botonSecundario(Rectangle{24, 764, ANCHO - 48, 60}, "Ya tengo cuenta"))
    {
        limpiarAcceso();
        irA(PANT_LOGIN);
    }
    textoCentrado("Proyecto académico · 5 módulos en C++", 850, 14, TEXTO2, false);
}

// Menú 1 del módulo 1: Registro de usuarios
void dibujarRegistro()
{
    encabezado("Crear cuenta", COLOR_PERFIL);
    texto("Tus datos quedan guardados solo en este celular.", 24, 92, 16, TEXTO2);
    campoTexto(Rectangle{24, 160, ANCHO - 48, 56}, "Nombre completo", accNombre, TECLADO_TEXTO, 1);
    campoTexto(Rectangle{24, 260, ANCHO - 48, 56}, "Contraseña", accClave, TECLADO_CLAVE, 2);
    campoTexto(Rectangle{24, 360, ANCHO - 48, 56}, "Repetir contraseña", accClave2, TECLADO_CLAVE, 3);

    if (!tecladoAbierto() && botonGrande(Rectangle{24, 460, ANCHO - 48, 60}, "Crear cuenta", COLOR_ING))
    {
        if (!textoValido(accNombre) || !textoValido(accClave))
        {
            aviso("Completá nombre y contraseña");
        }
        else if (!claveValida(accClave))
        {
            aviso("Contraseña: sin ñ ni tildes");
        }
        else if (accClave != accClave2)
        {
            aviso("Las contraseñas no coinciden");
        }
        else if (!registrar(accNombre, accClave))
        {
            aviso("Ya existe un usuario con ese nombre");
        }
        else
        {
            iniciar(accNombre, accClave);
            guardarApp();
            limpiarAcceso();
            irA(PANT_INICIO);
            aviso("¡Bienvenido/a a FinanZen!");
        }
    }
}

// Menú 2 del módulo 1: Inicio de sesión
void dibujarLogin()
{
    encabezado("Iniciar sesión", COLOR_PERFIL);
    campoTexto(Rectangle{24, 160, ANCHO - 48, 56}, "Nombre completo", accNombre, TECLADO_TEXTO, 1);
    campoTexto(Rectangle{24, 260, ANCHO - 48, 56}, "Contraseña", accClave, TECLADO_CLAVE, 2);

    if (!tecladoAbierto() && botonGrande(Rectangle{24, 360, ANCHO - 48, 60}, "Entrar", COLOR_AHO))
    {
        if (iniciar(accNombre, accClave))
        {
            limpiarAcceso();
            irA(PANT_INICIO);
        }
        else
        {
            aviso("Nombre o contraseña incorrectos");
        }
    }
    if (!tecladoAbierto() && total > 0)
    {
        texto("Usuarios en este celular:", 24, 460, 16, TEXTO2);
        for (int i = 0; i < total && i < 5; i++)
        {
            Rectangle r = {24, 492.0f + i * 52, ANCHO - 48, 44};
            dibujarTarjeta(r, TARJETA);
            texto(usu[i], 44, r.y + 12, 18, TEXTO);
            if (tocado(r))
            {
                accNombre = usu[i]; // Origen: lista() de Joaquín 1
            }
        }
    }
}
