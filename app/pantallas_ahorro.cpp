// FinanZen — módulo 4 (Hector) en la app: planificación del ahorro · integrado con ayuda de Claude Code (IA)
// Usa guardarNuevaMeta(), obtenerAhorroMensual(), proyeccionAhorro(), mesesConAporte() y las funciones
// cambiar...() de motor/ahorro.cpp. Las metas van por posición del usuario, como en su original.
#include "app.h"

string metaFormNombre = "";
string metaFormMonto = "";
string metaFormActual = "";
string metaFormMeses = "";
string metaSimular = "";
float metaDesp = 0;

// Llena el formulario con la meta actual (o lo deja vacío si no hay)
void prepararMeta()
{
    int u = indiceSesion();
    if (u != -1 && tieneMeta[u])
    {
        metaFormNombre = nombreMeta[u];
        metaFormMonto = montoATexto(montoMeta[u]);
        metaFormActual = montoATexto(ahorroActual[u]);
        metaFormMeses = to_string(mesesMeta[u]);
    }
    else
    {
        metaFormNombre = "";
        metaFormMonto = "";
        metaFormActual = "0";
        metaFormMeses = "";
    }
    metaSimular = "";
    metaDesp = 0;
}

bool leerMeses(string s, int &meses)
{
    double d;
    if (!leerMontoTexto(s, d) || d != (int)d) return false;
    meses = (int)d;
    return true;
}

void camposMeta()
{
    campoTexto(Rectangle{24, 140, ANCHO - 48, 56}, "Nombre de la meta", metaFormNombre, TECLADO_TEXTO, 1);
    campoTexto(Rectangle{24, 236, ANCHO - 48, 56}, "Monto objetivo", metaFormMonto, TECLADO_NUMERO, 2);
    campoTexto(Rectangle{24, 332, (ANCHO - 60) / 2, 56}, "Ya ahorrado", metaFormActual, TECLADO_NUMERO, 3);
    campoTexto(Rectangle{36 + (ANCHO - 60) / 2, 332, (ANCHO - 60) / 2, 56}, "Meses de plazo", metaFormMeses, TECLADO_ENTERO, 4);
}

// Raíz del módulo: sus 3 menús
void dibujarAhorro()
{
    encabezado("Planificación del ahorro", COLOR_AHO);
    int u = indiceSesion();
    Rectangle r = {24, 92, ANCHO - 48, 110};
    dibujarTarjeta(r, TARJETA);
    if (tieneMeta[u])
    {
        texto("Tu meta", 44, 108, 16, TEXTO2);
        textoNegrita(nombreMeta[u], 44, 130, 24, TEXTO);
        barraProgreso(Rectangle{44, 170, ANCHO - 88, 12}, ahorroActual[u] / montoMeta[u] * 100, COLOR_AHO);
    }
    else
    {
        texto("Tu meta", 44, 108, 16, TEXTO2);
        textoNegrita("Todavía no creaste una", 44, 134, 22, TEXTO);
    }

    if (tarjetaMenu(226, "1", "Creación de metas", "Nombre, monto y plazo", COLOR_AHO))
    {
        if (tieneMeta[u])
        {
            aviso("Ya tiene una meta. Use modificar o reemplazar.");
        }
        else
        {
            prepararMeta();
            irA(PANT_META_FORM);
        }
    }
    if (tarjetaMenu(344, "2", "Cálculo del ahorro mensual", "Cuota, proyección y simulador", COLOR_AHO))
    {
        if (tieneMeta[u])
        {
            prepararMeta();
            irA(PANT_META_CALCULO);
        }
        else aviso("Primero debe crear una meta.");
    }
    if (tarjetaMenu(462, "3", "Modificar o reemplazar meta", "Cambiar datos o empezar otra", COLOR_AHO))
    {
        if (tieneMeta[u])
        {
            prepararMeta();
            irA(PANT_META_MODIFICAR);
        }
        else aviso("Primero debe crear una meta.");
    }
    barraInferior();
}

// Menú 1 (y "Reemplazar meta completa" del menú 3) — Origen: guardarNuevaMetaAhorro()
void dibujarMetaForm()
{
    int u = indiceSesion();
    encabezado(tieneMeta[u] ? "Reemplazar meta" : "Nueva meta", COLOR_AHO);
    texto(tieneMeta[u] ? "La meta actual se reemplaza al guardar." : "¿Para qué querés ahorrar?", 24, 92, 16, TEXTO2);
    camposMeta();
    if (tecladoAbierto()) return;

    double monto, actual;
    int meses;
    bool numerosOk = leerMontoTexto(metaFormMonto, monto) && leerMontoTexto(metaFormActual, actual) &&
                     leerMeses(metaFormMeses, meses) && meses > 0;
    if (numerosOk)
    {
        // Vista previa: el resumen que Hector mostraba antes de "Guardar meta?"
        texto("Vas a necesitar ahorrar por mes:", 28, 420, 16, TEXTO2);
        textoNegrita(formatoBs(cuotaMensual(monto, actual, meses)), 28, 444, 26, COLOR_AHO); // misma cuenta que Hector
    }
    if (botonGrande(Rectangle{24, 500, ANCHO - 48, 60}, "Guardar meta", COLOR_AHO))
    {
        if (!numerosOk)
        {
            aviso("Revisá los montos y los meses");
        }
        else if (monto > MONTO_MAXIMO)
        {
            aviso("El objetivo máximo es Bs 1.000.000.000");
        }
        else if (guardarNuevaMeta(u, metaFormNombre, monto, actual, meses))
        {
            guardarApp();
            aviso("Meta guardada correctamente.");
            irA(PANT_AHORRO);
        }
        else
        {
            aviso("Lo ahorrado no puede superar el objetivo");
        }
    }
}

// Menú 2 — Origen: menuCalculoAhorro(): calcularAhorroMensual(), mostrarProyeccionAhorro(), simularAporteAhorro()
void dibujarMetaCalculo()
{
    int u = indiceSesion();
    encabezado("Cálculo del ahorro", COLOR_AHO);
    Rectangle r = {24, 90, ANCHO - 48, 150};
    dibujarTarjeta(r, TARJETA);
    texto("Ahorro mensual recomendado", 44, 106, 16, TEXTO2);
    textoNegrita(formatoBs(obtenerAhorroMensual(u)), 44, 130, 38, COLOR_AHO);
    texto("Faltan " + formatoBs(calcularFaltanteAhorro(u)) + TextFormat(" en %d meses", mesesMeta[u]), 44, 184, 16, TEXTO);
    texto("Meta: " + nombreMeta[u], 44, 208, 15, TEXTO2);

    // Simulador
    if (!tecladoAbierto()) texto("¿Y si ahorrás otro monto por mes?", 28, 262, 17, TEXTO);
    campoTexto(Rectangle{24, 318, ANCHO - 48, 56}, "Cuánto podrías ahorrar al mes", metaSimular, TECLADO_NUMERO, 1);
    double aporte;
    if (leerMontoTexto(metaSimular, aporte) && aporte > 0)
    {
        long long meses = mesesConAporte(u, aporte);
        if (meses > 0)
        {
            bool alcanza = meses <= mesesMeta[u];
            texto(TextFormat("Llegarías en %lld meses: ", meses) + string(alcanza ? "cumplís el plazo" : "no alcanza el plazo"),
                  28, 386, 16, alcanza ? VERDE : ROJO);
        }
    }
    if (tecladoAbierto()) return;

    // Proyección mes a mes
    texto("Proyección (sin intereses)", 28, 420, 17, TEXTO2);
    double aportes[1200], acumulados[1200];
    int filas = proyeccionAhorro(u, aportes, acumulados, 1200);
    Rectangle area = {24, 452, ANCHO - 48, ALTO - 452 - 20};
    desplazar(area, metaDesp, filas * 40.0f);
    BeginScissorMode((int)area.x, (int)area.y, (int)area.width, (int)area.height);
    for (int i = 0; i < filas; i++)
    {
        float y = area.y + i * 40 - metaDesp;
        if (y < area.y - 40 || y > area.y + area.height) continue;
        if (i % 2 == 0) DrawRectangleRounded(Rectangle{area.x, y, area.width, 36}, 0.3f, 6, TARJETA);
        texto(TextFormat("Mes %d", i + 1), area.x + 14, y + 9, 16, TEXTO2);
        texto("+" + formatoBs(aportes[i]), area.x + 120, y + 9, 16, TEXTO);
        string a = formatoBs(acumulados[i]);
        textoNegrita(a, area.x + area.width - 14 - anchoTexto(a, 16, true), y + 9, 16, COLOR_AHO);
    }
    EndScissorMode();
    if (filas == 0)
    {
        texto("La meta ya está alcanzada.", 28, 460, 18, VERDE);
    }
}

// Menú 3 — Origen: modificarMetaAhorro() (las 6 opciones de Hector)
void dibujarMetaModificar()
{
    int u = indiceSesion();
    encabezado("Modificar meta", COLOR_AHO);
    texto("Meta actual: " + nombreMeta[u], 24, 92, 16, TEXTO2);
    camposMeta();
    if (tecladoAbierto()) return;

    if (botonGrande(Rectangle{24, 430, ANCHO - 48, 60}, "Guardar cambios", COLOR_AHO))
    {
        double monto, actual;
        int meses;
        if (!leerMontoTexto(metaFormMonto, monto) || !leerMontoTexto(metaFormActual, actual) || !leerMeses(metaFormMeses, meses))
        {
            aviso("Revisá los montos y los meses");
        }
        else
        {
            // Origen: modificarMetaAhorro() — valida todo antes de cambiar algo
            if (modificarMetaCompleta(u, metaFormNombre, monto, actual, meses))
            {
                guardarApp();
                aviso("Meta actualizada");
                irA(PANT_AHORRO);
            }
            else if (monto > MONTO_MAXIMO)
            {
                aviso("El objetivo máximo es Bs 1.000.000.000");
            }
            else
            {
                aviso("Lo ahorrado no puede superar el objetivo");
            }
        }
    }
    if (botonSecundario(Rectangle{24, 504, ANCHO - 48, 56}, "Reemplazar meta completa"))
    {
        metaFormNombre = "";
        metaFormMonto = "";
        metaFormActual = "0";
        metaFormMeses = "";
        irA(PANT_META_FORM);
    }
    // Detalle (opción 6 de Hector)
    texto(TextFormat("Progreso: %.2f%%  ·  Faltante: ", ahorroActual[u] / montoMeta[u] * 100) +
              formatoBs(calcularFaltanteAhorro(u)), 24, 586, 15, TEXTO2);
}
