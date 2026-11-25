// FinanZen — módulo 3 (Joaquín 2): gestión de gastos · integrado con ayuda de Claude Code (IA)
// Lógica tomada de originales/gastos_joaquin2.cpp. Los cin/cout originales
// quedaron en consola/menu_gastos.cpp.
#include "finanzen.h"
#include <cmath>

// Arreglo original: en main() era "float gastos[100]; int n = 0;"
float gastos[MAX];
int nGastos = 0; // Renombrado: era el parámetro n

// Agregados: descripción, categoría, fecha y dueño de cada gasto
string gasDesc[MAX];
int gasCat[MAX];
string gasFecha[MAX];
int gasUsuario[MAX];

// Original: "float limite = 0" — ahora uno por posición de usuario
float limiteGasto[MAX];

string nombreCatGas[NUM_CAT_GAS] = {"Comida", "Transporte", "Servicios", "Salidas", "Salud", "Otros"};

// Agregado: validaciones del gasto en una sola función
bool datosGastoValidos(string desc, float monto, int cat, string fecha)
{
    if (!textoValido(desc))
    {
        return false;
    }
    if (monto <= 0 || monto > MONTO_MAXIMO) // los centavos se redondean al guardar (float no guarda exacto 1234.56)
    {
        return false;
    }
    if (cat < 0 || cat >= NUM_CAT_GAS)
    {
        return false;
    }
    return fechaValidaIng(fecha); // se usa la validación de fechas de Camila
}

// Origen: registrarGastos() opción 1 "Registrar un gasto" — Joaquín 2
bool registrarGasto(string desc, float monto, int cat, string fecha)
{
    if (sesion == -1)
    {
        return false;
    }
    if (nGastos >= MAX)
    {
        return false; // "No hay espacio disponible."
    }
    if (!datosGastoValidos(desc, monto, cat, fecha))
    {
        return false;
    }

    gastos[nGastos] = roundf(monto * 100) / 100;
    gasDesc[nGastos] = desc;
    gasCat[nGastos] = cat;
    gasFecha[nGastos] = fecha;
    gasUsuario[nGastos] = sesion;
    nGastos++;
    return true;
}

// Origen: registrarGastos() opción 3 "Establecer limite de gasto" — Joaquín 2
bool establecerLimite(float limite)
{
    int p = indiceSesion();
    if (p == -1 || limite < 0 || limite > MONTO_MAXIMO)
    {
        return false;
    }
    limiteGasto[p] = roundf(limite * 100) / 100;
    return true;
}

// Agregado: límite de gasto del usuario conectado
float limiteSesion()
{
    int p = indiceSesion();
    if (p == -1)
    {
        return 0;
    }
    return limiteGasto[p];
}

// Origen: consultarGastos() opción 2 "Mostrar total gastado" — Joaquín 2
double totalGastado()
{
    // Corregido (revisión): se suma en centavos enteros; sumar float perdía centavos
    long long centavos = 0;
    for (int i = 0; i < nGastos; i++)
    {
        if (gasUsuario[i] == sesion)
        {
            centavos += llround(gastos[i] * 100.0);
        }
    }
    return centavos / 100.0;
}

// Origen: consultarGastos() opción 3 "Mostrar gasto mayor" — Joaquín 2
float gastoMayor()
{
    float mayor = 0;
    bool primero = true; // Corregido: el original empezaba en gastos[0], que puede ser de otro usuario
    for (int i = 0; i < nGastos; i++)
    {
        if (gasUsuario[i] == sesion)
        {
            if (primero || gastos[i] > mayor)
            {
                mayor = gastos[i];
                primero = false;
            }
        }
    }
    return mayor;
}

// Origen: consultarGastos() opción 4 "Mostrar gasto menor" — Joaquín 2
float gastoMenor()
{
    float menor = 0;
    bool primero = true;
    for (int i = 0; i < nGastos; i++)
    {
        if (gasUsuario[i] == sesion)
        {
            if (primero || gastos[i] < menor)
            {
                menor = gastos[i];
                primero = false;
            }
        }
    }
    return menor;
}

// Agregado: clasificación por categoría
double totalGastadoCat(int cat)
{
    long long centavos = 0;
    for (int i = 0; i < nGastos; i++)
    {
        if (gasUsuario[i] == sesion && gasCat[i] == cat)
        {
            centavos += llround(gastos[i] * 100.0);
        }
    }
    return centavos / 100.0;
}

// Agregado: cuántos gastos tiene el usuario conectado
int cantidadGastos()
{
    int c = 0;
    for (int i = 0; i < nGastos; i++)
    {
        if (gasUsuario[i] == sesion)
        {
            c++;
        }
    }
    return c;
}

// Agregado: el gasto es del usuario conectado
bool gastoEsMio(int posicion)
{
    // "Gasto no valido."
    return sesion != -1 && posicion >= 0 && posicion < nGastos && gasUsuario[posicion] == sesion;
}

// Origen: administrarGastos() opción 1 "Modificar gasto" — Joaquín 2
bool modificarGasto(int posicion, string desc, float monto, int cat, string fecha)
{
    if (!gastoEsMio(posicion) || !datosGastoValidos(desc, monto, cat, fecha))
    {
        return false;
    }
    gastos[posicion] = roundf(monto * 100) / 100;
    gasDesc[posicion] = desc;
    gasCat[posicion] = cat;
    gasFecha[posicion] = fecha;
    return true; // "Gasto modificado correctamente."
}

// Origen: administrarGastos() opción 2 "Eliminar gasto" — Joaquín 2
bool eliminarGasto(int posicion)
{
    if (!gastoEsMio(posicion))
    {
        return false;
    }

    for (int i = posicion; i < nGastos - 1; i++)
    {
        gastos[i] = gastos[i + 1];
        gasDesc[i] = gasDesc[i + 1];
        gasCat[i] = gasCat[i + 1];
        gasFecha[i] = gasFecha[i + 1];
        gasUsuario[i] = gasUsuario[i + 1];
    }

    nGastos--;
    return true; // "Gasto eliminado correctamente."
}

// Agregado: al borrar una cuenta se borran sus gastos
void borrarGastosDeUsuario(int id)
{
    int j = 0;
    for (int i = 0; i < nGastos; i++)
    {
        if (gasUsuario[i] != id)
        {
            gastos[j] = gastos[i];
            gasDesc[j] = gasDesc[i];
            gasCat[j] = gasCat[i];
            gasFecha[j] = gasFecha[i];
            gasUsuario[j] = gasUsuario[i];
            j++;
        }
    }
    nGastos = j;
}

// Agregado: deja sin límite la posición de un usuario nuevo
void reiniciarLimiteUsuario(int pos)
{
    if (pos >= 0 && pos < MAX)
    {
        limiteGasto[pos] = 0;
    }
}

// Agregado: Corre los límites junto con los usuarios (mismo criterio que eliminarMetaDeUsuario de Hector)
void eliminarLimiteDeUsuario(int pos, int totalAntes)
{
    if (totalAntes < 1 || totalAntes > MAX || pos < 0 || pos >= totalAntes)
    {
        return;
    }
    for (int i = pos; i < totalAntes - 1; i++)
    {
        limiteGasto[i] = limiteGasto[i + 1];
    }
    reiniciarLimiteUsuario(totalAntes - 1);
}
