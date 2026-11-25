// FinanZen — módulo 4 (Hector): menús de planificación del ahorro · integrado con ayuda de Claude Code (IA)
// Textos tomados de originales/ahorro_hector.cpp; la lógica está en motor/ahorro.cpp.
#include "consola.h"
#include <iostream>
#include <iomanip>

static bool existeMetaConsola(int usuario)
{
    if (tieneMeta[usuario]) return true;
    cout << "Primero debe crear una meta.\n";
    return false;
}

// Origen: guardarNuevaMetaAhorro() — pide los datos antes de guardar
static void guardarNuevaMetaAhorro(int usuario)
{
    string nombre = leerTexto("Nombre de la meta: ");
    double monto = leerMonto("Monto objetivo: ", 0.01, 1000000000.0);
    double actual = leerMonto("Ahorro acumulado hasta ahora: ", 0.0, monto);
    int meses = leerEntero("Meses restantes para alcanzar la meta: ", 1, 1200);
    cout << "\nResumen: " << nombre << ", objetivo " << monto
         << ", acumulado " << actual << ", plazo " << meses << " meses.\n";
    if (leerEntero("Guardar meta? (1. Si / 0. No): ", 0, 1) == 0)
    {
        cout << "Operacion cancelada.\n";
        return;
    }
    if (guardarNuevaMeta(usuario, nombre, monto, actual, meses))
    {
        guardar();
        cout << "Meta guardada correctamente.\n";
    }
}

// Origen: mostrarMetaAhorro()
static void mostrarMetaAhorro(int usuario)
{
    if (!existeMetaConsola(usuario)) return;
    ios::fmtflags formato = cout.flags();
    streamsize precision = cout.precision();
    cout << fixed << setprecision(2)
         << "\n--- DETALLE DE LA META ---\n"
         << "Nombre: " << nombreMeta[usuario] << '\n'
         << "Objetivo: " << montoMeta[usuario] << '\n'
         << "Ahorro acumulado: " << ahorroActual[usuario] << '\n'
         << "Faltante: " << calcularFaltanteAhorro(usuario) << '\n'
         << "Progreso: " << ahorroActual[usuario] / montoMeta[usuario] * 100.0 << "%\n"
         << "Meses restantes: " << mesesMeta[usuario] << '\n';
    cout.flags(formato);
    cout.precision(precision);
}

// Menú 1: Creación de metas — Origen: menuCrearMetaAhorro()
static void menuCrearMetaAhorro(int usuario)
{
    int opcion;
    do
    {
        cout << "\n--- MENU DE CREACION DE META ---\n"
             << "1. Registrar una meta\n2. Consultar meta actual\n0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ", 0, 2);
        if (opcion == 1)
        {
            // Origen: crearMetaAhorro()
            if (tieneMeta[usuario])
            {
                cout << "Ya tiene una meta. Use la opcion de modificar o reemplazar.\n";
            }
            else
            {
                cout << "\n--- CREAR META DE AHORRO ---\n";
                guardarNuevaMetaAhorro(usuario);
            }
        }
        if (opcion == 2) mostrarMetaAhorro(usuario);
    } while (opcion != 0);
}

// Origen: calcularAhorroMensual()
static void calcularAhorroMensual(int usuario)
{
    double faltante = calcularFaltanteAhorro(usuario);
    double mensual = obtenerAhorroMensual(usuario);
    cout << "\n--- AHORRO MENSUAL RECOMENDADO ---\n";
    ios::fmtflags formatoAnterior = cout.flags();
    streamsize precisionAnterior = cout.precision();
    cout << fixed << setprecision(2)
         << "Meta: " << nombreMeta[usuario] << '\n'
         << "Monto objetivo: " << montoMeta[usuario] << '\n'
         << "Ahorro acumulado: " << ahorroActual[usuario] << '\n'
         << "Monto faltante: " << faltante << '\n'
         << "Meses restantes: " << mesesMeta[usuario] << '\n'
         << "Ahorro mensual recomendado: " << mensual << '\n';
    cout.flags(formatoAnterior);
    cout.precision(precisionAnterior);
    if (faltante == 0) cout << "La meta ya esta alcanzada.\n";
}

// Origen: mostrarProyeccionAhorro()
static void mostrarProyeccionAhorro(int usuario)
{
    double aporte[1200], acumulado[1200];
    int filas = proyeccionAhorro(usuario, aporte, acumulado, 1200);
    if (filas == 0)
    {
        cout << "La meta ya esta alcanzada.\n";
        return;
    }
    ios::fmtflags formato = cout.flags();
    streamsize precision = cout.precision();
    cout << fixed << setprecision(2) << "\nMes | Aporte | Acumulado\n";
    for (int i = 0; i < filas; i++)
    {
        cout << i + 1 << " | " << aporte[i] << " | " << acumulado[i] << '\n';
    }
    cout.flags(formato);
    cout.precision(precision);
    cout << "Proyeccion estimada sin intereses ni cambios en los aportes.\n";
}

// Origen: simularAporteAhorro()
static void simularAporteAhorro(int usuario)
{
    if (calcularFaltanteAhorro(usuario) <= 0) { cout << "La meta ya esta alcanzada.\n"; return; }
    double aporte = leerMonto("Cuanto podria ahorrar al mes?: ", 0.01, 1000000000.0);
    long long mesesNecesarios = mesesConAporte(usuario, aporte);
    cout << "Meses estimados para alcanzar la meta: " << mesesNecesarios << '\n';
    if (mesesNecesarios <= mesesMeta[usuario]) cout << "Este aporte permite cumplir el plazo.\n";
    else cout << "Este aporte no alcanza para cumplir el plazo actual.\n";
    cout << "La simulacion no modifica la meta.\n";
}

// Menú 2: Cálculo del ahorro mensual — Origen: menuCalculoAhorro()
static void menuCalculoAhorro(int usuario)
{
    if (!existeMetaConsola(usuario)) return;
    int opcion;
    do
    {
        cout << "\n--- MENU DE CALCULO DE AHORRO ---\n"
             << "1. Calcular ahorro mensual recomendado\n"
             << "2. Ver proyeccion mensual\n"
             << "3. Simular otro aporte mensual\n"
             << "0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ", 0, 3);
        if (opcion == 1) calcularAhorroMensual(usuario);
        if (opcion == 2) mostrarProyeccionAhorro(usuario);
        if (opcion == 3) simularAporteAhorro(usuario);
    } while (opcion != 0);
}

// Menú 3: Modificación o reemplazo — Origen: modificarMetaAhorro()
void modificarMetaAhorro(int usuario)
{
    if (!existeMetaConsola(usuario)) return;
    int opcion;
    do
    {
        cout << "\n--- MODIFICAR O REEMPLAZAR META ---\n"
             << "Meta actual: " << nombreMeta[usuario] << '\n'
             << "1. Cambiar nombre\n2. Cambiar monto objetivo\n"
             << "3. Actualizar ahorro acumulado\n4. Cambiar meses restantes\n"
             << "5. Reemplazar meta completa\n6. Ver detalle de la meta\n0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ", 0, 6);
        bool ok = false;
        switch (opcion)
        {
            case 1:
                ok = cambiarNombreMeta(usuario, leerTexto("Nuevo nombre: "));
                if (ok) cout << "Nombre actualizado.\n";
                break;
            case 2:
                ok = cambiarMontoMeta(usuario, leerMonto("Nuevo monto objetivo (no menor al ahorro acumulado): ",
                    ahorroActual[usuario] > 0.01 ? ahorroActual[usuario] : 0.01, 1000000000.0));
                if (ok) cout << "Monto actualizado.\n";
                break;
            case 3:
                ok = actualizarAhorroAcumulado(usuario, leerMonto("Ahorro acumulado: ", 0.0, montoMeta[usuario]));
                if (ok) cout << "Ahorro acumulado actualizado.\n";
                break;
            case 4:
                ok = cambiarPlazoMeta(usuario, leerEntero("Meses restantes: ", 1, 1200));
                if (ok) cout << "Plazo actualizado.\n";
                break;
            case 5:
                cout << "\n--- REEMPLAZAR META ---\n";
                cout << "La meta actual solo se reemplazara si confirma los nuevos datos.\n";
                guardarNuevaMetaAhorro(usuario);
                break;
            case 6:
                mostrarMetaAhorro(usuario);
                break;
        }
        if (ok) guardar();
    } while (opcion != 0);
}

// Origen: menuPlanificacionAhorro() — Hector
void menuPlanificacionAhorro(int usuario)
{
    if (usuario < 0 || usuario >= MAX_METAS_AHORRO)
    {
        cout << "Indice de usuario invalido.\n";
        return;
    }
    int opcion;
    do
    {
        cout << "\n===== PLANIFICACION DEL AHORRO =====\n"
             << "1. Crear meta de ahorro\n"
             << "2. Calcular ahorro mensual recomendado\n"
             << "3. Modificar o reemplazar meta\n"
             << "0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ", 0, 3);
        switch (opcion)
        {
            case 1: menuCrearMetaAhorro(usuario); break;
            case 2: menuCalculoAhorro(usuario); break;
            case 3: modificarMetaAhorro(usuario); break;
        }
    } while (opcion != 0);
}
