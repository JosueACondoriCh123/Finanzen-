// FinanZen — módulo 1 (Joaquín 1) en la app: administración del perfil · integrado con ayuda de Claude Code (IA)
// Usa modificar(), borrar() y cerrarSesion() de motor/usuarios.cpp.
#include "app.h"

string perNombre = "";
string perClave = "";

// Menú 3 del módulo 1: Administración del perfil
void dibujarPerfil()
{
    encabezado("Mi perfil", COLOR_PERFIL);
    string nombre = nombreSesion();
    DrawCircle(ANCHO / 2, 150, 50, COLOR_PERFIL);
    string inicial = primeraLetra(nombre);
    textoNegrita(inicial, ANCHO / 2 - anchoTexto(inicial, 46, true) / 2, 124, 46, FONDO);
    textoCentrado(nombre, 216, 26, TEXTO, true);
    textoCentrado(TextFormat("Usuario %d de %d en este celular", indiceSesion() + 1, total), 252, 16, TEXTO2, false);

    if (tarjetaMenu(300, "1", "Modificar mis datos", "Cambiar nombre y contraseña", COLOR_PERFIL))
    {
        perNombre = nombre;
        perClave = con[indiceSesion()];
        irA(PANT_PERFIL_EDITAR);
    }
    if (tarjetaMenu(418, "2", "Modificar mi meta", "Ir a la meta de ahorro", COLOR_AHO))
    {
        prepararMeta();
        irA(tieneMeta[indiceSesion()] ? PANT_META_MODIFICAR : PANT_META_FORM);
    }
    if (tarjetaMenu(536, "3", "Borrar mi cuenta", "Se borran también tus datos", ROJO))
    {
        irA(PANT_PERFIL_BORRAR);
    }
    if (botonSecundario(Rectangle{24, 680, ANCHO - 48, 56}, "Cerrar sesión"))
    {
        cerrarSesion();
        irA(PANT_BIENVENIDA);
    }
}

// Origen: modificar() — Joaquín 1
void dibujarPerfilEditar()
{
    encabezado("Modificar mis datos", COLOR_PERFIL);
    campoTexto(Rectangle{24, 160, ANCHO - 48, 56}, "Nuevo nombre", perNombre, TECLADO_TEXTO, 1);
    campoTexto(Rectangle{24, 260, ANCHO - 48, 56}, "Nueva contraseña", perClave, TECLADO_CLAVE, 2);
    if (!tecladoAbierto() && botonGrande(Rectangle{24, 360, ANCHO - 48, 60}, "Guardar cambios", COLOR_PERFIL))
    {
        if (modificar(indiceSesion(), perNombre, perClave))
        {
            guardarApp();
            aviso("Usuario modificado correctamente");
            irA(PANT_PERFIL);
        }
        else if (!claveValida(perClave))
        {
            aviso("Contraseña: sin ñ ni tildes");
        }
        else
        {
            aviso("Nombre vacío o ya usado por otro usuario");
        }
    }
}

// Origen: borrar() — Joaquín 1
void dibujarPerfilBorrar()
{
    encabezado("Borrar mi cuenta", ROJO);
    DrawCircle(ANCHO / 2, 200, 56, ColorAlpha(ROJO, 0.2f));
    textoNegrita("!", ANCHO / 2 - anchoTexto("!", 60, true) / 2, 168, 60, ROJO);
    textoCentrado("¿Seguro que querés borrar tu cuenta?", 290, 21, TEXTO, true);
    textoCentrado("Se borran también tus ingresos, gastos,", 334, 17, TEXTO2, false);
    textoCentrado("tu meta y tus ahorros. No se puede deshacer.", 358, 17, TEXTO2, false);
    if (botonGrande(Rectangle{24, 440, ANCHO - 48, 60}, "Sí, borrar definitivamente", ROJO))
    {
        borrar(indiceSesion());
        guardarApp();
        aviso("Usuario eliminado correctamente");
        irA(PANT_BIENVENIDA);
    }
    if (botonSecundario(Rectangle{24, 514, ANCHO - 48, 60}, "Cancelar"))
    {
        irA(PANT_PERFIL);
    }
}
