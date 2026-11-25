// FinanZen — módulo 4 (Hector): planificación del ahorro · integrado con ayuda de Claude Code (IA)
// Lógica tomada de originales/ahorro_hector.cpp. Sus lectores con cin (leerEnteroAhorro,
// leerMontoAhorro, leerTextoAhorro) y sus menús quedaron en consola/.
// Como en el original, las metas se guardan por POSICIÓN del usuario (0 a 99).
#include "finanzen.h"
#include <cmath>

// Datos del módulo: una meta por posición de usuario.
// Corregido: sin static, para que la app y los archivos .txt puedan leerlos.
string nombreMeta[MAX_METAS_AHORRO];
double montoMeta[MAX_METAS_AHORRO] = {};
double ahorroActual[MAX_METAS_AHORRO] = {};
int mesesMeta[MAX_METAS_AHORRO] = {};
bool tieneMeta[MAX_METAS_AHORRO] = {};

// Origen: chequeo de centavos de leerMontoAhorro() — Hector
bool montoConDosDecimales(double v)
{
    return std::isfinite(v) && fabs(v * 100.0 - round(v * 100.0)) < 0.00001;
}

// Agregado: posición dentro del arreglo de metas
bool usuarioValidoAhorro(int usuario)
{
    return usuario >= 0 && usuario < MAX_METAS_AHORRO;
}

// Origen: leerMontoAhorro(mensaje, minimo, maximo) — Hector (la validación, sin cin)
bool montoEnRango(double valor, double minimo, double maximo)
{
    return montoConDosDecimales(valor) && valor >= minimo && valor <= maximo;
}

// Origen: existeMetaAhorro() — Hector
bool existeMetaAhorro(int usuario)
{
    return usuarioValidoAhorro(usuario) && tieneMeta[usuario];
}

// Agregado: la meta en esa posición es la del usuario conectado
// (consola y app siempre pasan indiceSesion(); el motor lo verifica igual)
bool esMiPosicion(int usuario)
{
    return usuarioValidoAhorro(usuario) && usuario == indiceSesion();
}

// Origen: calcularFaltanteAhorro() — Hector
double calcularFaltanteAhorro(int usuario)
{
    if (!existeMetaAhorro(usuario))
    {
        return 0; // Corregido: sin meta (o posición inválida) no lee fuera del arreglo
    }
    return montoMeta[usuario] - ahorroActual[usuario];
}

// Agregado: la cuenta en centavos de obtenerAhorroMensual() de Hector, para cualquier monto
// (la app la usa en la vista previa antes de guardar la meta)
double cuotaMensual(double monto, double actual, int meses)
{
    long long faltante = llround(monto * 100.0) - llround(actual * 100.0);
    if (faltante <= 0 || meses < 1)
    {
        return 0;
    }
    return ((faltante + meses - 1) / meses) / 100.0;
}

// Origen: obtenerAhorroMensual() — Hector (copia exacta)
double obtenerAhorroMensual(int usuario)
{
    if (!existeMetaAhorro(usuario) || mesesMeta[usuario] < 1)
    {
        return 0; // Corregido: sin meta no divide por mesesMeta = 0
    }
    // Los calculos en centavos evitan errores de redondeo de punto flotante.
    long long faltante = llround(montoMeta[usuario] * 100.0) - llround(ahorroActual[usuario] * 100.0);
    return ((faltante + mesesMeta[usuario] - 1) / mesesMeta[usuario]) / 100.0;
}

// Origen: las validaciones de guardarNuevaMetaAhorro() — Hector
bool metaValida(string nombre, double monto, double actual, int meses)
{
    if (!textoValido(nombre))
    {
        return false;
    }
    if (!montoEnRango(monto, 0.01, MONTO_MAXIMO)) // "Monto objetivo: "
    {
        return false;
    }
    if (!montoEnRango(actual, 0.0, monto)) // "Ahorro acumulado hasta ahora: "
    {
        return false;
    }
    return meses >= 1 && meses <= 1200; // "Meses restantes para alcanzar la meta: "
}

// Origen: guardarNuevaMetaAhorro() — Hector (crea o reemplaza la meta)
bool guardarNuevaMeta(int usuario, string nombre, double monto, double actual, int meses)
{
    if (!esMiPosicion(usuario) || !metaValida(nombre, monto, actual, meses))
    {
        return false;
    }
    nombreMeta[usuario] = nombre;
    montoMeta[usuario] = monto;
    ahorroActual[usuario] = actual;
    mesesMeta[usuario] = meses;
    tieneMeta[usuario] = true;
    return true; // "Meta guardada correctamente."
}

// Origen: modificarMetaAhorro() — Hector. Cambia nombre, monto, acumulado y plazo juntos:
// primero valida todo y, si algo no vale, no cambia nada (la app usa esta función).
bool modificarMetaCompleta(int usuario, string nombre, double monto, double actual, int meses)
{
    if (!existeMetaAhorro(usuario) || !esMiPosicion(usuario) || !metaValida(nombre, monto, actual, meses))
    {
        return false;
    }
    nombreMeta[usuario] = nombre;
    montoMeta[usuario] = monto;
    ahorroActual[usuario] = actual;
    mesesMeta[usuario] = meses;
    return true;
}

// Origen: cambiarNombreMeta() — Hector
bool cambiarNombreMeta(int usuario, string nombre)
{
    if (!existeMetaAhorro(usuario) || !esMiPosicion(usuario) || !textoValido(nombre))
    {
        return false;
    }
    nombreMeta[usuario] = nombre;
    return true; // "Nombre actualizado."
}

// Origen: cambiarMontoMeta() — Hector ("no menor al ahorro acumulado")
bool cambiarMontoMeta(int usuario, double monto)
{
    if (!existeMetaAhorro(usuario) || !esMiPosicion(usuario))
    {
        return false;
    }
    double minimo = ahorroActual[usuario] > 0.01 ? ahorroActual[usuario] : 0.01;
    if (!montoEnRango(monto, minimo, MONTO_MAXIMO))
    {
        return false;
    }
    montoMeta[usuario] = monto;
    return true; // "Monto actualizado."
}

// Origen: actualizarAhorroAcumulado() — Hector
bool actualizarAhorroAcumulado(int usuario, double actual)
{
    if (!existeMetaAhorro(usuario) || !esMiPosicion(usuario) || !montoEnRango(actual, 0.0, montoMeta[usuario]))
    {
        return false;
    }
    ahorroActual[usuario] = actual;
    return true; // "Ahorro acumulado actualizado."
}

// Origen: cambiarPlazoMeta() — Hector
bool cambiarPlazoMeta(int usuario, int meses)
{
    if (!existeMetaAhorro(usuario) || !esMiPosicion(usuario) || meses < 1 || meses > 1200)
    {
        return false;
    }
    mesesMeta[usuario] = meses;
    return true; // "Plazo actualizado."
}

// Origen: simularAporteAhorro() — Hector. Devuelve los meses necesarios (0 si no aplica).
long long mesesConAporte(int usuario, double aporte)
{
    if (!existeMetaAhorro(usuario) || !montoEnRango(aporte, 0.01, MONTO_MAXIMO))
    {
        return 0;
    }
    long long faltanteCentavos = llround(montoMeta[usuario] * 100.0) - llround(ahorroActual[usuario] * 100.0);
    if (faltanteCentavos <= 0)
    {
        return 0; // "La meta ya esta alcanzada."
    }
    long long aporteCentavos = llround(aporte * 100.0);
    return (faltanteCentavos + aporteCentavos - 1) / aporteCentavos;
}

// Origen: mostrarProyeccionAhorro() — Hector. Llena la tabla Mes | Aporte | Acumulado
// y devuelve cuántas filas tiene (0 si no hay meta o ya está alcanzada).
int proyeccionAhorro(int usuario, double aporte[], double acumulado[], int max)
{
    if (!existeMetaAhorro(usuario) || calcularFaltanteAhorro(usuario) <= 0)
    {
        return 0;
    }
    long long acum = llround(ahorroActual[usuario] * 100.0);
    long long objetivo = llround(montoMeta[usuario] * 100.0);
    long long mensual = llround(obtenerAhorroMensual(usuario) * 100.0);
    int filas = 0;
    for (int mes = 1; mes <= mesesMeta[usuario] && acum < objetivo && filas < max; mes++)
    {
        long long a = mensual;
        if (a > objetivo - acum) a = objetivo - acum;
        acum += a;
        aporte[filas] = a / 100.0;
        acumulado[filas] = acum / 100.0;
        filas++;
    }
    return filas;
}

// Origen: reiniciarMetaUsuario() — Hector (copia exacta)
void reiniciarMetaUsuario(int usuario)
{
    if (usuario < 0 || usuario >= MAX_METAS_AHORRO) return;
    nombreMeta[usuario].clear();
    montoMeta[usuario] = 0;
    ahorroActual[usuario] = 0;
    mesesMeta[usuario] = 0;
    tieneMeta[usuario] = false;
}

// Origen: eliminarMetaDeUsuario() — Hector (copia exacta)
// Se llama con el indice eliminado y el total ANTES de reducirlo.
void eliminarMetaDeUsuario(int usuario, int totalAntes)
{
    if (totalAntes < 1 || totalAntes > MAX_METAS_AHORRO || usuario < 0 || usuario >= totalAntes) return;
    for (int i = usuario; i < totalAntes - 1; i++)
    {
        nombreMeta[i] = nombreMeta[i + 1];
        montoMeta[i] = montoMeta[i + 1];
        ahorroActual[i] = ahorroActual[i + 1];
        mesesMeta[i] = mesesMeta[i + 1];
        tieneMeta[i] = tieneMeta[i + 1];
    }
    reiniciarMetaUsuario(totalAntes - 1);
}
