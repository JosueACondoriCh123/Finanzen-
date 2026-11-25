// FinanZen — módulo 5 (Josue) en la app: seguimiento y reportes · integrado con ayuda de Claude Code (IA)
// Usa registro_ahorro(), progreso_ahorro() y resumen_financiero() de motor/reportes.cpp.
#include "app.h"

string repMonto = "";
string repFecha = "";
float repDesp = 0;

// Deja el formulario de ahorro limpio, con la fecha de hoy
void prepararAhorro()
{
    repMonto = "";
    repFecha = hoy();
    repDesp = 0;
}

// Raíz del módulo: sus 3 menús (Origen: menu5() de Josue)
void dibujarReportes()
{
    encabezado("Seguimiento y reportes", COLOR_REP);
    double ingresos, gastosTot, ahorrado, saldo;
    resumen_financiero(ingresos, gastosTot, ahorrado, saldo);
    Rectangle r = {24, 92, ANCHO - 48, 110};
    dibujarTarjeta(r, TARJETA);
    texto("Saldo disponible", 44, 108, 16, TEXTO2);
    textoNegrita(formatoBs(saldo), 44, 132, 36, saldo < 0 ? ROJO : COLOR_REP);
    texto("Ahorrado: " + formatoBs(ahorrado), 44, 176, 15, TEXTO2);

    if (tarjetaMenu(226, "1", "Registro del ahorro", "Anotá lo que pudiste ahorrar", COLOR_REP))
    {
        prepararAhorro();
        irA(PANT_REP_AHORRO);
    }
    if (tarjetaMenu(344, "2", "Progreso del ahorro", "Cuánto te falta para tu meta", COLOR_REP))
    {
        irA(PANT_REP_PROGRESO);
    }
    if (tarjetaMenu(462, "3", "Resumen financiero", "Ingresos, gastos, ahorro y saldo", COLOR_REP))
    {
        irA(PANT_REP_RESUMEN);
    }
    barraInferior();
}

// Menú 1 — Origen: registro_ahorro()
void dibujarRepAhorro()
{
    int u = indiceSesion();
    encabezado("Registrar ahorro", COLOR_REP);
    if (!tieneMeta[u])
    {
        textoCentrado("Primero creá una meta de ahorro.", 140, 19, TEXTO, false);
        if (botonGrande(Rectangle{24, 200, ANCHO - 48, 60}, "Crear meta", COLOR_AHO))
        {
            prepararMeta();
            irA(PANT_META_FORM);
        }
        return;
    }
    if (repFecha == "") repFecha = hoy();
    texto("Meta: " + nombreMeta[u], 24, 92, 17, TEXTO);
    texto("Faltan " + formatoBs(calcularFaltanteAhorro(u)) + "  ·  Disponible " + formatoBs(saldoDisponible()), 24, 118, 15, TEXTO2);
    campoTexto(Rectangle{24, 186, (ANCHO - 60) / 2, 56}, "¿Cuánto ahorraste?", repMonto, TECLADO_NUMERO, 1);
    campoTexto(Rectangle{36 + (ANCHO - 60) / 2, 186, (ANCHO - 60) / 2, 56}, "Fecha", repFecha, TECLADO_FECHA, 2);
    if (tecladoAbierto()) return;

    if (botonGrande(Rectangle{24, 270, ANCHO - 48, 60}, "Registrar ahorro", COLOR_REP))
    {
        double monto;
        if (!leerMontoTexto(repMonto, monto)) aviso("Monto inválido");
        else if (!fechaValidaIng(repFecha)) aviso("Fecha inválida (dd/mm/aaaa)");
        else if (registro_ahorro(monto, repFecha))
        {
            guardarApp();
            repMonto = "";
            aviso(progreso_ahorro() >= 100 ? "¡Meta cumplida! Felicitaciones" : "Ahorro registrado");
        }
        else aviso("Supera lo que falta o tu saldo disponible");
    }

    // Historial de ahorros
    texto("Tus ahorros registrados", 28, 356, 17, TEXTO2);
    int mios[MAX];
    int k = 0;
    for (int i = ahoTotal - 1; i >= 0; i--)
    {
        if (ahoUsuario[i] == sesion) mios[k++] = i;
    }
    Rectangle area = {24, 388, ANCHO - 48, ALTO - 388 - 20};
    desplazar(area, repDesp, k * 60.0f);
    BeginScissorMode((int)area.x, (int)area.y, (int)area.width, (int)area.height);
    for (int j = 0; j < k; j++)
    {
        int i = mios[j];
        float y = area.y + j * 60 - repDesp;
        dibujarTarjeta(Rectangle{area.x, y, area.width, 52}, TARJETA);
        texto(ahoFecha[i], area.x + 16, y + 15, 17, TEXTO2);
        string m = "+" + formatoBs(ahoMonto[i]);
        textoNegrita(m, area.x + area.width - 16 - anchoTexto(m, 18, true), y + 14, 18, COLOR_AHO);
    }
    EndScissorMode();
    if (k == 0) texto("Todavía no registraste ahorros", 28, 392, 17, TEXTO2);
}

// Menú 2 — Origen: progreso_ahorro()
void dibujarRepProgreso()
{
    int u = indiceSesion();
    encabezado("Progreso del ahorro", COLOR_REP);
    double progreso = progreso_ahorro();
    if (progreso < 0)
    {
        textoCentrado("Todavía no tenés una meta de ahorro.", 160, 19, TEXTO, false);
        return;
    }
    Vector2 centro = {ANCHO / 2, 270};
    DrawRing(centro, 104, 128, 0, 360, 72, TARJETA2);
    DrawRing(centro, 104, 128, -90, (float)(-90 + 360 * progreso / 100), 72, progreso >= 100 ? VERDE : COLOR_AHO);
    string pct = TextFormat("%.0f%%", progreso);
    textoNegrita(pct, centro.x - anchoTexto(pct, 52, true) / 2, centro.y - 34, 52, TEXTO);
    textoCentrado("de tu meta", (int)centro.y + 24, 16, TEXTO2, false);
    textoCentrado(nombreMeta[u], 430, 26, TEXTO, true);

    string etiquetas[4] = {"Ahorrado", "Objetivo", "Falta", "Por mes"};
    double valores[4] = {ahorroActual[u], montoMeta[u], calcularFaltanteAhorro(u), obtenerAhorroMensual(u)};
    for (int i = 0; i < 4; i++)
    {
        Rectangle r = {24.0f + (i % 2) * ((ANCHO - 60) / 2.0f + 12), 486.0f + (i / 2) * 96, (ANCHO - 60) / 2.0f, 84};
        dibujarTarjeta(r, TARJETA);
        texto(etiquetas[i], r.x + 16, r.y + 14, 15, TEXTO2);
        textoNegrita(formatoBs(valores[i]), r.x + 16, r.y + 40, 21, i == 0 ? COLOR_AHO : TEXTO);
    }
    string msg = progreso >= 100 ? "La meta de ahorro está cumplida" : TextFormat("Te quedan %d meses de plazo", mesesMeta[u]);
    textoCentrado(msg, 700, 18, progreso >= 100 ? VERDE : TEXTO2, false);
}

// Menú 3 — Origen: resumen_financiero()
void dibujarRepResumen()
{
    encabezado("Resumen financiero", COLOR_REP);
    double ingresos, gastosTot, ahorrado, saldo;
    resumen_financiero(ingresos, gastosTot, ahorrado, saldo);

    // Barra repartida: en qué se fue cada boliviano que entró
    texto("¿A dónde fue tu dinero?", 24, 92, 17, TEXTO2);
    Rectangle barra = {24, 124, ANCHO - 48, 28};
    DrawRectangleRounded(barra, 0.5f, 8, TARJETA2);
    if (ingresos > 0)
    {
        float x = barra.x;
        double partes[3] = {gastosTot, ahorrado, saldo > 0 ? saldo : 0};
        Color colores[3] = {COLOR_GAS, COLOR_AHO, COLOR_ING};
        for (int i = 0; i < 3; i++)
        {
            float w = (float)(barra.width * partes[i] / ingresos);
            if (x + w > barra.x + barra.width) w = barra.x + barra.width - x;
            if (w > 0) DrawRectangle((int)x, (int)barra.y, (int)w, (int)barra.height, colores[i]);
            x += w;
        }
    }

    string nombres[4] = {"Ingresos", "Gastos", "Ahorro registrado", "Saldo disponible"};
    double valores[4] = {ingresos, gastosTot, ahorrado, saldo};
    Color colores[4] = {COLOR_ING, COLOR_GAS, COLOR_AHO, saldo < 0 ? ROJO : TEXTO};
    for (int i = 0; i < 4; i++)
    {
        Rectangle r = {24, 180.0f + i * 92, ANCHO - 48, 80};
        dibujarTarjeta(r, i == 3 ? TARJETA2 : TARJETA);
        DrawCircle(48, (int)(r.y + 40), 7, colores[i]);
        texto(nombres[i], 66, r.y + 29, 18, TEXTO);
        string m = formatoBs(valores[i]);
        textoNegrita(m, r.x + r.width - 20 - anchoTexto(m, 22, true), r.y + 27, 22, colores[i]);
        if (ingresos > 0 && i > 0 && i < 3)
        {
            string p = TextFormat("%.0f%% de tus ingresos", valores[i] / ingresos * 100);
            texto(p, 66, r.y + 52, 13, TEXTO2);
        }
    }
    int u = indiceSesion();
    if (tieneMeta[u])
    {
        texto("El saldo para alcanzar la meta es: " + formatoBs(calcularFaltanteAhorro(u)), 24, 560, 16, TEXTO2);
    }
    texto("El ahorro no se cuenta como gasto: no se descuenta dos veces.", 24, 600, 14, TEXTO2);
}
