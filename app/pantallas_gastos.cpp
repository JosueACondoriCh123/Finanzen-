// FinanZen — módulo 3 (Joaquín 2) en la app: gestión de gastos · integrado con ayuda de Claude Code (IA)
// Usa registrarGasto(), establecerLimite(), totalGastado(), gastoMayor(), gastoMenor(),
// modificarGasto() y eliminarGasto() de motor/gastos.cpp.
#include "app.h"

string gasFormDesc = "";
string gasFormMonto = "";
string gasFormFecha = "";
string gasFormLimite = "";
int gasFormCat = 0;
int gasEditando = -1;
int gasSeguidos = 0; // "Registrar varios gastos": cuántos se cargaron seguidos
int gasPestana = 0;
float gasDesp = 0;

int misGastos(int mios[])
{
    int k = 0;
    for (int i = nGastos - 1; i >= 0; i--)
    {
        if (gasUsuario[i] == sesion)
        {
            mios[k] = i;
            k++;
        }
    }
    return k;
}

// Color tipo semáforo según cuánto del límite se usó
Color colorLimite(double gastado, double limite)
{
    if (limite <= 0) return COLOR_GAS;
    double uso = gastado / limite * 100;
    if (uso < 70) return VERDE;
    if (uso < 100) return AMARILLO;
    return ROJO;
}

void prepararGasto(int indice)
{
    gasEditando = indice;
    if (indice == -1)
    {
        gasFormDesc = "";
        gasFormMonto = "";
        gasFormFecha = hoy();
        gasFormCat = 0;
        gasSeguidos = 0;
    }
    else
    {
        gasFormDesc = gasDesc[indice];
        gasFormMonto = montoATexto(gastos[indice]);
        gasFormFecha = gasFecha[indice];
        gasFormCat = gasCat[indice];
    }
}

int listaGastos(Rectangle area, int mios[], int k)
{
    float alto = 68;
    desplazar(area, gasDesp, k * alto);
    BeginScissorMode((int)area.x, (int)area.y, (int)area.width, (int)area.height);
    for (int j = 0; j < k; j++)
    {
        int i = mios[j];
        float y = area.y + j * alto - gasDesp;
        if (y < area.y - alto || y > area.y + area.height) continue;
        dibujarTarjeta(Rectangle{area.x, y, area.width, alto - 8}, TARJETA);
        texto(gasDesc[i], area.x + 16, y + 10, 18, TEXTO);
        texto(nombreCatGas[gasCat[i]] + " · " + gasFecha[i], area.x + 16, y + 34, 14, TEXTO2);
        string m = "-" + formatoBs(gastos[i]);
        textoNegrita(m, area.x + area.width - 16 - anchoTexto(m, 18, true), y + 20, 18, COLOR_GAS);
    }
    EndScissorMode();
    if (k == 0)
    {
        texto("No hay gastos registrados.", area.x + 8, area.y + 10, 18, TEXTO2);
    }
    int fila = filaTocada(area, k, alto, gasDesp);
    return fila == -1 ? -1 : mios[fila];
}

// Raíz del módulo: sus 3 menús
void dibujarGastos()
{
    encabezado("Gastos", COLOR_GAS);
    Rectangle r = {24, 92, ANCHO - 48, 110};
    dibujarTarjeta(r, TARJETA);
    double total = totalGastado();
    float limite = limiteSesion();
    texto("Total gastado", 44, 108, 16, TEXTO2);
    textoNegrita(formatoBs(total), 44, 132, 36, COLOR_GAS);
    if (limite > 0)
    {
        texto("Límite " + formatoBs(limite), 260, 108, 15, TEXTO2);
        barraProgreso(Rectangle{44, 180, ANCHO - 88, 10}, total / limite * 100, colorLimite(total, limite));
    }
    else
    {
        texto("Sin límite de gasto", 44, 176, 15, TEXTO2);
    }

    if (tarjetaMenu(226, "1", "Registro de gastos", "Uno, varios seguidos o tu límite", COLOR_GAS))
    {
        prepararGasto(-1);
        irA(PANT_GAS_FORM);
    }
    if (tarjetaMenu(344, "2", "Consulta y clasificación", "Lista, totales y categorías", COLOR_GAS))
    {
        gasDesp = 0;
        irA(PANT_GAS_CONSULTA);
    }
    if (tarjetaMenu(462, "3", "Modificación o eliminación", "Corregir o borrar un gasto", COLOR_GAS))
    {
        gasDesp = 0;
        irA(PANT_GAS_ADMIN);
    }
    barraInferior();
}

bool guardarGastoForm()
{
    double monto;
    if (gasEditando == -1 && nGastos >= MAX)
    {
        aviso("No hay espacio disponible.");
        return false;
    }
    if (leerMontoTexto(gasFormMonto, monto) && monto > MONTO_MAXIMO)
    {
        aviso("El monto máximo es Bs 1.000.000.000");
        return false;
    }
    if (!leerMontoTexto(gasFormMonto, monto) || monto <= 0)
    {
        aviso("Monto inválido");
        return false;
    }
    if (!fechaValidaIng(gasFormFecha))
    {
        aviso("Fecha inválida (dd/mm/aaaa)");
        return false;
    }
    bool ok;
    if (gasEditando == -1) ok = registrarGasto(gasFormDesc, (float)monto, gasFormCat, gasFormFecha);
    else ok = modificarGasto(gasEditando, gasFormDesc, (float)monto, gasFormCat, gasFormFecha);
    if (!ok)
    {
        aviso("Revisá la descripción");
        return false;
    }
    guardarApp();
    float limite = limiteSesion();
    if (limite > 0 && totalGastado() > limite) aviso("Atención: superaste tu límite de gasto");
    else aviso(gasEditando == -1 ? "Gasto registrado" : "Gasto modificado correctamente.");
    return true;
}

// Menú 1 (y edición del menú 3) — Origen: registrarGastos() y administrarGastos()
void dibujarGasForm()
{
    string titulo = gasEditando != -1 ? "Modificar gasto" : (gasSeguidos > 0 ? TextFormat("Gasto %d", gasSeguidos + 1) : "Nuevo gasto");
    encabezado(titulo, COLOR_GAS);
    campoTexto(Rectangle{24, 130, ANCHO - 48, 56}, "Descripción", gasFormDesc, TECLADO_TEXTO, 1);
    campoTexto(Rectangle{24, 226, (ANCHO - 60) / 2, 56}, "Monto", gasFormMonto, TECLADO_NUMERO, 2);
    campoTexto(Rectangle{36 + (ANCHO - 60) / 2, 226, (ANCHO - 60) / 2, 56}, "Fecha", gasFormFecha, TECLADO_FECHA, 3);
    texto("Categoría", 28, 302, 16, TEXTO2);
    chips(328, nombreCatGas, NUM_CAT_GAS, gasFormCat, COLOR_GAS);

    if (tecladoAbierto()) return;

    if (botonGrande(Rectangle{24, 480, ANCHO - 48, 60}, "Guardar", COLOR_GAS))
    {
        if (guardarGastoForm())
        {
            irA(gasEditando == -1 ? PANT_GASTOS : PANT_GAS_ADMIN);
        }
    }
    if (gasEditando == -1)
    {
        // "2. Registrar varios gastos" de Joaquín 2
        if (botonSecundario(Rectangle{24, 554, ANCHO - 48, 56}, "Guardar y cargar otro"))
        {
            if (guardarGastoForm())
            {
                int seguidos = gasSeguidos + 1;
                prepararGasto(-1);
                gasSeguidos = seguidos;
            }
        }
        // "3. Establecer limite de gasto" de Joaquín 2
        if (botonSecundario(Rectangle{24, 624, ANCHO - 48, 56}, "Establecer límite de gasto"))
        {
            float l = limiteSesion();
            gasFormLimite = l > 0 ? montoATexto(l) : "";
            irA(PANT_GAS_LIMITE);
        }
    }
    else if (botonSecundario(Rectangle{24, 554, ANCHO - 48, 56}, "Eliminar este gasto"))
    {
        if (eliminarGasto(gasEditando))
        {
            guardarApp();
            aviso("Gasto eliminado correctamente.");
            irA(PANT_GAS_ADMIN);
        }
    }
}

// Origen: registrarGastos() opción 3 — Joaquín 2
void dibujarGasLimite()
{
    encabezado("Límite de gasto", COLOR_GAS);
    texto("FinanZen te avisa cuando tus gastos lo superan.", 24, 92, 16, TEXTO2);
    campoTexto(Rectangle{24, 160, ANCHO - 48, 56}, "Límite (0 = sin límite)", gasFormLimite, TECLADO_NUMERO, 1);
    if (!tecladoAbierto() && botonGrande(Rectangle{24, 260, ANCHO - 48, 60}, "Guardar límite", COLOR_GAS))
    {
        double v;
        if (gasFormLimite == "") v = 0;
        if ((gasFormLimite == "" || leerMontoTexto(gasFormLimite, v)) && establecerLimite((float)v))
        {
            guardarApp();
            aviso("Límite guardado");
            irA(PANT_GASTOS);
        }
        else
        {
            aviso("Límite inválido");
        }
    }
}

// Menú 2 — Origen: consultarGastos()
void dibujarGasConsulta()
{
    encabezado("Consulta de gastos", COLOR_GAS);
    string pestanas[3] = {"Lista", "Totales", "Categorías"};
    if (chips(90, pestanas, 3, gasPestana, COLOR_GAS)) gasDesp = 0;

    double total = totalGastado();
    float limite = limiteSesion();
    if (gasPestana == 0)
    {
        int mios[MAX];
        int k = misGastos(mios);
        listaGastos(Rectangle{24, 146, ANCHO - 48, ALTO - 146 - 90}, mios, k);
    }
    else if (gasPestana == 1)
    {
        string nombres[3] = {"Total gastado", "Gasto mayor", "Gasto menor"};
        double valores[3] = {total, gastoMayor(), gastoMenor()};
        for (int i = 0; i < 3; i++)
        {
            Rectangle r = {24, 146.0f + i * 96, ANCHO - 48, 84};
            dibujarTarjeta(r, TARJETA);
            texto(nombres[i], 44, r.y + 14, 16, TEXTO2);
            textoNegrita(formatoBs(valores[i]), 44, r.y + 38, 28, i == 0 ? COLOR_GAS : TEXTO);
        }
        Rectangle rl = {24, 434, ANCHO - 48, 110};
        dibujarTarjeta(rl, TARJETA);
        texto("Límite de gasto", 44, 450, 16, TEXTO2);
        if (limite > 0)
        {
            double uso = total / limite * 100;
            textoNegrita(TextFormat("%.0f%% usado de ", uso) + formatoBs(limite), 44, 474, 20, TEXTO);
            barraProgreso(Rectangle{44, 512, ANCHO - 88, 12}, uso, colorLimite(total, limite));
        }
        else
        {
            textoNegrita("No definiste un límite", 44, 476, 20, TEXTO);
        }
    }
    else
    {
        texto("Cómo se reparte lo que gastaste", 28, 146, 17, TEXTO2);
        for (int c = 0; c < NUM_CAT_GAS; c++)
        {
            float y = 182 + c * 74;
            double v = totalGastadoCat(c);
            texto(nombreCatGas[c], 28, y, 18, TEXTO);
            string m = formatoBs(v) + TextFormat("  (%.0f%%)", total > 0 ? v / total * 100 : 0);
            textoNegrita(m, ANCHO - 28 - anchoTexto(m, 17, true), y, 17, TEXTO);
            barraProgreso(Rectangle{28, y + 30, ANCHO - 56, 12}, total > 0 ? v / total * 100 : 0, COLOR_GAS);
        }
    }
    barraInferior();
}

// Menú 3 — Origen: administrarGastos()
void dibujarGasAdmin()
{
    encabezado("Modificar gastos", COLOR_GAS);
    texto("Tocá un gasto para modificarlo o eliminarlo.", 24, 92, 16, TEXTO2);
    int mios[MAX];
    int k = misGastos(mios);
    int elegido = listaGastos(Rectangle{24, 126, ANCHO - 48, ALTO - 126 - 90}, mios, k);
    if (elegido != -1)
    {
        prepararGasto(elegido);
        irA(PANT_GAS_FORM);
    }
    barraInferior();
}
