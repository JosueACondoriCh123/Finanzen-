// FinanZen — módulo 5 (Josue): menús de seguimiento y reportes · integrado con ayuda de Claude Code (IA)
// Textos tomados de originales/reportes_josue.cpp; la lógica está en motor/reportes.cpp.
#include "consola.h"
#include <iostream>
#include <iomanip>

// Corregido (revisión): se guarda y restaura el formato de cout, como hace Hector,
// para no dejar los otros menús con 2 cifras de precisión.
ios::fmtflags formatoGuardado;
streamsize precisionGuardada;
void formatoDinero() { formatoGuardado = cout.flags(); precisionGuardada = cout.precision(); cout << fixed << setprecision(2); }
void restaurarFormato() { cout.flags(formatoGuardado); cout.precision(precisionGuardada); }

// Origen: menu5() — Josue
void menu5()
{
    cout << "\n\t Menu \n";
    cout << "1.- Registro del ahorro\n";
    cout << "2.- Progreso del ahorro\n";
    cout << "3.- Resumen financiero\n";
    cout << "0.- Salir\n";
}

// Menú 1: Registro del ahorro realizado — Origen: registro_ahorro()
void registroAhorroConsola()
{
    int pos = indiceSesion();
    if (!tieneMeta[pos])
    {
        cout << "Primero cree una meta en Planificacion del ahorro." << endl;
        return;
    }
    formatoDinero();
    cout << "Meta: " << nombreMeta[pos] << " (faltan " << calcularFaltanteAhorro(pos) << " Bs)" << endl;
    cout << "Saldo disponible: " << saldoDisponible() << " Bs" << endl;
    restaurarFormato();
    double monto = leerMonto("Cuanto ahorro?: ", 0.01, 1000000000.0);
    string fecha = leerFecha("Fecha (dd/mm/aaaa): ");
    if (registro_ahorro(monto, fecha))
    {
        guardar();
        formatoDinero();
        cout << "Ahorro registrado. El ahorro total es: " << totalAhorradoRegistrado() << endl;
        restaurarFormato();
    }
    else
    {
        cout << "No se puede registrar: supera lo que falta para la meta o su saldo disponible." << endl;
    }
}

// Menú 2: Progreso del ahorro — Origen: progreso_ahorro()
void progresoConsola()
{
    double progreso = progreso_ahorro();
    if (progreso < 0)
    {
        cout << "Todavia no tiene una meta de ahorro." << endl;
        return;
    }
    formatoDinero();
    cout << "Progreso: " << progreso << "%" << endl;
    cout << "Se ha ahorrado en total un " << progreso << "% de la meta propuesta" << endl;
    if (progreso >= 100)
        cout << "La meta de ahorro esta cumplida" << endl;
    else
        cout << "Aun no se cumple la meta de ahorro" << endl;
    restaurarFormato();
}

// Menú 3: Resumen financiero — Origen: resumen_financiero()
void resumenConsola()
{
    double ingresos, gastosTot, ahorrado, saldo;
    resumen_financiero(ingresos, gastosTot, ahorrado, saldo);
    formatoDinero();
    cout << "\n--- RESUMEN FINANCIERO ---" << endl;
    cout << "Ingresos: " << ingresos << endl;
    cout << "Gastos: " << gastosTot << endl;
    cout << "Ahorro registrado: " << ahorrado << endl;
    cout << "Saldo disponible: " << saldo << endl;
    int pos = indiceSesion();
    if (tieneMeta[pos])
    {
        cout << "El saldo para alcanzar la meta es: " << calcularFaltanteAhorro(pos) << endl;
    }
    restaurarFormato();
}

// Origen: main() de reportes — Josue
void menuReportes()
{
    int opcion5;
    do
    {
        menu5();
        opcion5 = leerEntero("Que seguimiento quiere hacer? ", 0, 3);
        switch (opcion5)
        {
        case 1:
            registroAhorroConsola();
            break;
        case 2:
            progresoConsola();
            break;
        case 3:
            resumenConsola();
            break;
        case 0:
            cout << "Saliendo al menu principal";
            break;
        }
    } while (opcion5 != 0);
}
