// FinanZen — pantalla de inicio: resumen de todos los módulos · integrado con ayuda de Claude Code (IA)
// Reúne resumen_financiero() de Josue, la meta de Hector, el límite de Joaquín 2 y los ingresos de Camila.
#include "app.h"

string primerNombre(string nombre)
{
    size_t espacio = nombre.find(' ');
    if (espacio == string::npos) return nombre;
    return nombre.substr(0, espacio);
}

void dibujarInicio()
{
    string nombre = nombreSesion();
    int pos = indiceSesion();

    // Saludo + avatar (abre el perfil: módulo 1)
    texto("Hola,", 24, 26, 20, TEXTO2);
    textoNegrita(primerNombre(nombre), 24, 50, 32, TEXTO);
    Rectangle avatar = {ANCHO - 84, 28, 60, 60};
    DrawCircle((int)(avatar.x + 30), (int)(avatar.y + 30), 30, COLOR_PERFIL);
    string inicial = primeraLetra(nombre);
    textoNegrita(inicial, avatar.x + 30 - anchoTexto(inicial, 28, true) / 2, avatar.y + 15, 28, FONDO);
    if (tocado(avatar)) irA(PANT_PERFIL);

    // Saldo disponible (módulo 5)
    double ingresos, gastosTot, ahorrado, saldo;
    resumen_financiero(ingresos, gastosTot, ahorrado, saldo);
    Rectangle rs = {24, 110, ANCHO - 48, 190};
    dibujarTarjeta(rs, TARJETA);
    texto("Saldo disponible", 44, 128, 18, TEXTO2);
    textoNegrita(formatoBs(saldo), 44, 154, 42, saldo < 0 ? ROJO : TEXTO);
    float col = (ANCHO - 88) / 3.0f;
    string etiquetas[3] = {"Ingresos", "Gastos", "Ahorrado"};
    double valores[3] = {ingresos, gastosTot, ahorrado};
    Color colores[3] = {COLOR_ING, COLOR_GAS, COLOR_AHO};
    for (int i = 0; i < 3; i++)
    {
        float x = 44 + i * col;
        DrawCircle((int)x + 5, 228, 5, colores[i]);
        texto(etiquetas[i], x + 16, 219, 15, TEXTO2);
        textoNegrita(formatoBs(valores[i]), x, 244, 16, TEXTO);
    }
    if (tocado(rs)) irA(PANT_REP_RESUMEN);

    // Meta de ahorro (módulo 4 + módulo 5)
    Rectangle rm = {24, 316, ANCHO - 48, 130};
    dibujarTarjeta(rm, TARJETA);
    if (pos != -1 && tieneMeta[pos])
    {
        double progreso = progreso_ahorro();
        texto("Meta de ahorro", 44, 332, 16, TEXTO2);
        textoNegrita(nombreMeta[pos], 44, 356, 22, TEXTO);
        string pct = TextFormat("%.0f%%", progreso);
        textoNegrita(pct, ANCHO - 44 - anchoTexto(pct, 22, true), 356, 22, COLOR_AHO);
        barraProgreso(Rectangle{44, 394, ANCHO - 88, 14}, progreso, COLOR_AHO);
        texto(formatoBs(ahorroActual[pos]) + " de " + formatoBs(montoMeta[pos]), 44, 416, 15, TEXTO2);
        if (tocado(rm)) irA(PANT_REP_PROGRESO);
    }
    else
    {
        texto("Meta de ahorro", 44, 332, 16, TEXTO2);
        textoNegrita("Todavía no tenés una meta", 44, 360, 21, TEXTO);
        texto("Tocá aquí para crear la primera", 44, 396, 16, COLOR_AHO);
        if (tocado(rm))
        {
            prepararMeta();
            irA(PANT_META_FORM);
        }
    }

    // Límite de gasto (módulo 3)
    Rectangle rl = {24, 462, ANCHO - 48, 96};
    dibujarTarjeta(rl, TARJETA);
    float limite = limiteSesion();
    texto("Gastos del período", 44, 478, 16, TEXTO2);
    if (limite > 0)
    {
        double uso = gastosTot / limite * 100;
        Color c = uso < 70 ? VERDE : (uso < 100 ? AMARILLO : ROJO);
        string linea = formatoBs(gastosTot) + " de " + formatoBs(limite);
        textoNegrita(linea, 44, 500, 18, TEXTO);
        barraProgreso(Rectangle{44, 532, ANCHO - 88, 10}, uso, c);
    }
    else
    {
        textoNegrita(formatoBs(gastosTot), 44, 500, 18, TEXTO);
        texto("Sin límite definido", 44, 528, 15, TEXTO2);
    }
    if (tocado(rl)) irA(PANT_GAS_CONSULTA);

    // Accesos rápidos
    float w = (ANCHO - 48 - 16) / 3.0f;
    if (botonGrande(Rectangle{24, 578, w, 54}, "+ Ingreso", COLOR_ING))
    {
        prepararIngreso(-1);
        irA(PANT_ING_FORM);
    }
    if (botonGrande(Rectangle{24 + w + 8, 578, w, 54}, "+ Gasto", COLOR_GAS))
    {
        prepararGasto(-1);
        irA(PANT_GAS_FORM);
    }
    if (botonGrande(Rectangle{24 + 2 * (w + 8), 578, w, 54}, "+ Ahorro", COLOR_AHO))
    {
        prepararAhorro();
        irA(PANT_REP_AHORRO);
    }

    // Últimos gastos
    texto("Últimos gastos", 24, 650, 17, TEXTO2);
    int mostrados = 0;
    for (int i = nGastos - 1; i >= 0 && mostrados < 3; i--)
    {
        if (gasUsuario[i] != sesion) continue;
        float y = 680 + mostrados * 44;
        texto(gasDesc[i], 24, y, 18, TEXTO);
        texto(nombreCatGas[gasCat[i]] + " · " + gasFecha[i].substr(0, 5), 24, y + 21, 13, TEXTO2);
        string m = "-" + formatoBs(gastos[i]);
        textoNegrita(m, ANCHO - 24 - anchoTexto(m, 18, true), y + 6, 18, COLOR_GAS);
        mostrados++;
    }
    if (mostrados == 0)
    {
        texto("Sin gastos todavía", 24, 684, 17, TEXTO2);
    }

    barraInferior();
}
