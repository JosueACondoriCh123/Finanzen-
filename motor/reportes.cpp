// FinanZen — módulo 5 (Josue): seguimiento y reportes · integrado con ayuda de Claude Code (IA)
// Lógica tomada de originales/reportes_josue.cpp (registro_ahorro, progreso_ahorro,
// resumen_financiero). Su menu5() y sus cout quedaron en consola/menu_reportes.cpp.
#include "finanzen.h"
#include <cmath>

// Agregado: registros del ahorro realizado (el original no los guardaba)
double ahoMonto[MAX];
string ahoFecha[MAX];
int ahoUsuario[MAX];
int ahoTotal = 0;

// Agregado: Suma de lo ahorrado (registrado aquí) por el usuario conectado
double totalAhorradoRegistrado()
{
    double suma = 0;
    for (int i = 0; i < ahoTotal; i++)
    {
        if (ahoUsuario[i] == sesion)
        {
            suma += ahoMonto[i];
        }
    }
    return suma;
}

// Agregado: Saldo disponible = ingresos - gastos - ahorro registrado.
// El ahorro NO se guarda como gasto, así no se descuenta dos veces.
double saldoDisponible()
{
    return sumaIngresos() - totalGastado() - totalAhorradoRegistrado();
}

// Origen: registro_ahorro() — Josue
// Corregido: el original calculaba "sueldo - gastos" con un arreglo sin tamaño
// (float gasto[]) y variables sin valor inicial. Ahora registra lo que la
// persona realmente ahorró y lo suma a la meta de Hector.
bool registro_ahorro(double monto, string fecha)
{
    int pos = indiceSesion();
    if (pos == -1 || !tieneMeta[pos] || ahoTotal >= MAX)
    {
        return false;
    }
    if (monto <= 0 || !montoConDosDecimales(monto) || !fechaValidaIng(fecha))
    {
        return false;
    }
    if (monto > calcularFaltanteAhorro(pos) + 0.001)
    {
        return false; // no se ahorra más de lo que falta para la meta
    }
    if (monto > saldoDisponible() + 0.001)
    {
        return false; // no se puede ahorrar dinero que no está disponible
    }

    ahoMonto[ahoTotal] = monto;
    ahoFecha[ahoTotal] = fecha;
    ahoUsuario[ahoTotal] = sesion;
    ahoTotal++;
    ahorroActual[pos] = round((ahorroActual[pos] + monto) * 100) / 100; // Corregido: sin error de redondeo
    return true;
}

// Origen: progreso_ahorro() — Josue: progreso = (ahorro / meta) * 100
// Corregido: usa la meta de Hector y el ahorro acumulado, con tope en 100.
// Devuelve -1 si no hay meta.
double progreso_ahorro()
{
    int pos = indiceSesion();
    if (pos == -1 || !tieneMeta[pos])
    {
        return -1;
    }
    double progreso = (ahorroActual[pos] / montoMeta[pos]) * 100;
    if (progreso > 100)
    {
        progreso = 100;
    }
    return progreso;
}

// Origen: resumen_financiero() — Josue ("El saldo de la cuenta es: ...")
// Devuelve los valores por referencia para que la consola y la app los muestren.
void resumen_financiero(double &ingresos, double &gastosTotales, double &ahorrado, double &saldo)
{
    ingresos = sumaIngresos();
    gastosTotales = totalGastado();
    ahorrado = totalAhorradoRegistrado();
    saldo = ingresos - gastosTotales - ahorrado;
}

// Agregado: al borrar una cuenta se borran sus ahorros registrados
void borrarAhorrosDeUsuario(int id)
{
    int j = 0;
    for (int i = 0; i < ahoTotal; i++)
    {
        if (ahoUsuario[i] != id)
        {
            ahoMonto[j] = ahoMonto[i];
            ahoFecha[j] = ahoFecha[i];
            ahoUsuario[j] = ahoUsuario[i];
            j++;
        }
    }
    ahoTotal = j;
}
