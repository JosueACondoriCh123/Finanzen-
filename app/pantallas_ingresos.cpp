// FinanZen — módulo 2 (Camila) en la app: gestión de ingresos · integrado con ayuda de Claude Code (IA)
// Usa agregarIngreso(), cambiarIngreso(), quitarIngreso(), sumaIngresos() y fechaValidaIng() de motor/ingresos.cpp.
#include "app.h"

string ingFormDesc = "";
string ingFormMonto = "";
string ingFormFecha = "";
int ingFormCat = 0;
int ingEditando = -1; // -1 = nuevo; si no, posición en los arreglos de Camila
int ingPestana = 0;
int ingCatFiltro = 0;
float ingDesp = 0;

// Posiciones de los ingresos del usuario conectado (del más nuevo al más viejo)
int misIngresos(int mios[], int cat)
{
    int k = 0;
    for (int i = ingTotal - 1; i >= 0; i--)
    {
        if (ingUsuario[i] == sesion && (cat == -1 || ingCat[i] == cat))
        {
            mios[k] = i;
            k++;
        }
    }
    return k;
}

void prepararIngreso(int indice)
{
    ingEditando = indice;
    if (indice == -1)
    {
        ingFormDesc = "";
        ingFormMonto = "";
        ingFormFecha = hoy();
        ingFormCat = 0;
    }
    else
    {
        ingFormDesc = ingDesc[indice];
        ingFormMonto = montoATexto(ingMonto[indice]);
        ingFormFecha = ingFecha[indice];
        ingFormCat = ingCat[indice];
    }
}

// Lista de ingresos dentro de un área con desplazamiento. Devuelve la posición tocada o -1.
int listaIngresos(Rectangle area, int mios[], int k)
{
    float alto = 68;
    desplazar(area, ingDesp, k * alto);
    BeginScissorMode((int)area.x, (int)area.y, (int)area.width, (int)area.height);
    for (int j = 0; j < k; j++)
    {
        int i = mios[j];
        float y = area.y + j * alto - ingDesp;
        if (y < area.y - alto || y > area.y + area.height) continue;
        dibujarTarjeta(Rectangle{area.x, y, area.width, alto - 8}, TARJETA);
        texto(ingDesc[i], area.x + 16, y + 10, 18, TEXTO);
        texto(nombreCatIng[ingCat[i]] + " · " + ingFecha[i], area.x + 16, y + 34, 14, TEXTO2);
        string m = "+" + formatoBs(ingMonto[i]);
        textoNegrita(m, area.x + area.width - 16 - anchoTexto(m, 18, true), y + 20, 18, COLOR_ING);
    }
    EndScissorMode();
    if (k == 0)
    {
        texto("No hay ingresos registrados", area.x + 8, area.y + 10, 18, TEXTO2);
    }
    int fila = filaTocada(area, k, alto, ingDesp);
    return fila == -1 ? -1 : mios[fila];
}

// Raíz del módulo: sus 3 menús
void dibujarIngresos()
{
    encabezado("Ingresos", COLOR_ING);
    Rectangle r = {24, 92, ANCHO - 48, 110};
    dibujarTarjeta(r, TARJETA);
    texto("Total de ingresos", 44, 108, 16, TEXTO2);
    textoNegrita(formatoBs(sumaIngresos()), 44, 132, 36, COLOR_ING);
    texto(TextFormat("%d ingresos registrados", cantidadIngresos()), 44, 174, 15, TEXTO2);

    if (tarjetaMenu(226, "1", "Registro de ingresos", "Sueldo, freelance, ventas u otros", COLOR_ING))
    {
        prepararIngreso(-1);
        irA(PANT_ING_FORM);
    }
    if (tarjetaMenu(344, "2", "Consulta de ingresos", "Todos, por categoría y resumen", COLOR_ING))
    {
        ingDesp = 0;
        irA(PANT_ING_CONSULTA);
    }
    if (tarjetaMenu(462, "3", "Modificación de ingresos", "Corregir o eliminar un ingreso", COLOR_ING))
    {
        ingDesp = 0;
        irA(PANT_ING_ADMIN);
    }
    barraInferior();
}

// Menú 1 (y edición del menú 3) — Origen: registrarIngreso() / modificarIngreso()
void dibujarIngForm()
{
    encabezado(ingEditando == -1 ? "Nuevo ingreso" : "Modificar ingreso", COLOR_ING);
    campoTexto(Rectangle{24, 130, ANCHO - 48, 56}, "Descripción", ingFormDesc, TECLADO_TEXTO, 1);
    campoTexto(Rectangle{24, 226, (ANCHO - 60) / 2, 56}, "Monto", ingFormMonto, TECLADO_NUMERO, 2);
    campoTexto(Rectangle{36 + (ANCHO - 60) / 2, 226, (ANCHO - 60) / 2, 56}, "Fecha", ingFormFecha, TECLADO_FECHA, 3);
    texto("Categoría", 28, 302, 16, TEXTO2);
    chips(328, nombreCatIng, 4, ingFormCat, COLOR_ING);

    if (tecladoAbierto()) return;

    if (botonGrande(Rectangle{24, 470, ANCHO - 48, 60}, "Guardar", COLOR_ING))
    {
        double monto;
        bool ok = false;
        if (ingEditando == -1 && ingTotal >= MAX_ING) aviso("No hay espacio para más ingresos");
        else if (!leerMontoTexto(ingFormMonto, monto)) aviso("Monto inválido");
        else if (monto > MONTO_MAXIMO) aviso("El monto máximo es Bs 1.000.000.000");
        else if (!fechaValidaIng(ingFormFecha)) aviso("Fecha inválida (dd/mm/aaaa)");
        else if (ingEditando == -1) ok = agregarIngreso(ingFormDesc, monto, ingFormCat, ingFormFecha);
        else ok = cambiarIngreso(ingEditando, ingFormDesc, monto, ingFormCat, ingFormFecha);

        if (ok)
        {
            guardarApp();
            aviso(ingEditando == -1 ? "Ingreso registrado correctamente" : "Ingreso modificado correctamente");
            irA(ingEditando == -1 ? PANT_INGRESOS : PANT_ING_ADMIN);
        }
        else if (leerMontoTexto(ingFormMonto, monto) && monto <= MONTO_MAXIMO && fechaValidaIng(ingFormFecha) &&
                 !(ingEditando == -1 && ingTotal >= MAX_ING))
        {
            aviso("Revisá la descripción y que el monto sea mayor a 0");
        }
    }
    if (ingEditando != -1 && botonSecundario(Rectangle{24, 544, ANCHO - 48, 56}, "Eliminar este ingreso"))
    {
        if (quitarIngreso(ingEditando))
        {
            guardarApp();
            aviso("Ingreso eliminado correctamente");
            irA(PANT_ING_ADMIN);
        }
    }
}

// Menú 2 — Origen: menuConsultaIngresos(), verTodosIngresos(), buscarIngresosPorCategoria(), resumenIngresos()
void dibujarIngConsulta()
{
    encabezado("Consulta de ingresos", COLOR_ING);
    string pestanas[3] = {"Todos", "Por categoría", "Resumen"};
    if (chips(90, pestanas, 3, ingPestana, COLOR_ING)) ingDesp = 0;

    int mios[MAX_ING];
    if (ingPestana == 0)
    {
        int k = misIngresos(mios, -1);
        listaIngresos(Rectangle{24, 146, ANCHO - 48, ALTO - 146 - 90}, mios, k);
    }
    else if (ingPestana == 1)
    {
        if (chips(146, nombreCatIng, 4, ingCatFiltro, COLOR_ING)) ingDesp = 0;
        int k = misIngresos(mios, ingCatFiltro);
        texto("Total en " + nombreCatIng[ingCatFiltro] + ": " + formatoBs(sumaIngresosCat(ingCatFiltro)), 28, 254, 17, TEXTO);
        listaIngresos(Rectangle{24, 286, ANCHO - 48, ALTO - 286 - 90}, mios, k);
    }
    else
    {
        int cantidad = cantidadIngresos();
        double suma = sumaIngresos();
        dibujarTarjeta(Rectangle{24, 146, ANCHO - 48, 150}, TARJETA);
        texto("Cantidad de ingresos", 44, 164, 16, TEXTO2);
        textoNegrita(TextFormat("%d", cantidad), 44, 186, 26, TEXTO);
        texto("Total acumulado", 230, 164, 16, TEXTO2);
        textoNegrita(formatoBs(suma), 230, 186, 22, COLOR_ING);
        texto("Promedio por ingreso", 44, 232, 16, TEXTO2);
        textoNegrita(formatoBs(cantidad > 0 ? suma / cantidad : 0), 44, 254, 22, TEXTO);
        texto("Por categoría", 28, 320, 17, TEXTO2);
        for (int c = 0; c < 4; c++)
        {
            float y = 352 + c * 70;
            double v = sumaIngresosCat(c);
            texto(nombreCatIng[c], 28, y, 18, TEXTO);
            string m = formatoBs(v);
            textoNegrita(m, ANCHO - 28 - anchoTexto(m, 18, true), y, 18, TEXTO);
            barraProgreso(Rectangle{28, y + 30, ANCHO - 56, 12}, suma > 0 ? v / suma * 100 : 0, COLOR_ING);
        }
    }
    barraInferior();
}

// Menú 3 — Origen: modificarIngreso() y borrarIngreso()
void dibujarIngAdmin()
{
    encabezado("Modificar ingresos", COLOR_ING);
    texto("Tocá un ingreso para corregirlo o eliminarlo.", 24, 92, 16, TEXTO2);
    int mios[MAX_ING];
    int k = misIngresos(mios, -1);
    int elegido = listaIngresos(Rectangle{24, 126, ANCHO - 48, ALTO - 126 - 90}, mios, k);
    if (elegido != -1)
    {
        prepararIngreso(elegido);
        irA(PANT_ING_FORM);
    }
    barraInferior();
}
