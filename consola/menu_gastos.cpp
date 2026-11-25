// FinanZen — módulo 3 (Joaquín 2): menús de gastos · integrado con ayuda de Claude Code (IA)
// Textos tomados de originales/gastos_joaquin2.cpp; la lógica está en motor/gastos.cpp.
#include "consola.h"
#include <iostream>

int elegirCategoriaGasto()
{
    cout << "Categoria:";
    for (int i = 0; i < NUM_CAT_GAS; i++)
    {
        cout << "\n  " << i + 1 << ". " << nombreCatGas[i];
    }
    cout << "\n";
    return leerEntero("Seleccione una categoria: ", 1, NUM_CAT_GAS) - 1;
}

// Pide y registra un gasto (lo que antes era "cout << Ingrese el monto; cin >> gastos[n]")
void pedirUnGasto()
{
    if (nGastos >= MAX)
    {
        cout << "No hay espacio disponible." << endl;
        return;
    }
    string desc = leerTexto("Descripcion: ");
    float monto = (float)leerMonto("Ingrese el monto: ", 0.01, 1000000000.0);
    int cat = elegirCategoriaGasto();
    string fecha = leerFecha("Fecha (dd/mm/aaaa): ");
    if (registrarGasto(desc, monto, cat, fecha))
    {
        guardar();
        cout << "Gasto registrado." << endl;
        if (limiteSesion() > 0 && totalGastado() > limiteSesion())
        {
            cout << "Atencion: supero su limite de gasto de " << limiteSesion() << " Bs" << endl;
        }
    }
    else
    {
        cout << "No se pudo registrar: revise la descripcion." << endl;
    }
}

// Lista numerada de los gastos del usuario; mios[] guarda su posición real
int listarMisGastos(int mios[])
{
    int k = 0;
    for (int i = 0; i < nGastos; i++)
    {
        if (gasUsuario[i] == sesion)
        {
            mios[k] = i;
            k++;
            cout << "Gasto " << k << ": " << gastos[i] << " Bs - " << gasDesc[i]
                 << " (" << nombreCatGas[gasCat[i]] << ", " << gasFecha[i] << ")" << endl;
        }
    }
    return k;
}

// Menú 1: Registro de gastos — Origen: registrarGastos()
void registrarGastos()
{
    int opcion;
    int cantidad;

    cout << "\n--- REGISTRAR GASTOS ---" << endl;
    cout << "1. Registrar un gasto" << endl;
    cout << "2. Registrar varios gastos" << endl;
    cout << "3. Establecer limite de gasto" << endl;
    opcion = leerEntero("Seleccione: ", 1, 3);

    if (opcion == 1)
    {
        pedirUnGasto();
    }
    else if (opcion == 2)
    {
        cantidad = leerEntero("Cuantos gastos desea registrar: ", 1, MAX);

        for (int i = 0; i < cantidad && nGastos < MAX; i++)
        {
            cout << "\nGasto " << i + 1 << " de " << cantidad << endl;
            pedirUnGasto();
        }
    }
    else if (opcion == 3)
    {
        float limite = (float)leerMonto("Ingrese su limite de gasto: ", 0, 1000000000.0);
        if (establecerLimite(limite))
        {
            guardar();
            cout << "Limite guardado." << endl;
        }
    }
}

// Menú 2: Consulta y clasificación — Origen: consultarGastos()
void consultarGastos()
{
    int opcion;

    cout << "\n--- CONSULTAR GASTOS ---" << endl;
    cout << "1. Mostrar todos los gastos" << endl;
    cout << "2. Mostrar total gastado" << endl;
    cout << "3. Mostrar gasto mayor" << endl;
    cout << "4. Mostrar gasto menor" << endl;
    cout << "5. Mostrar gastos por categoria" << endl;
    opcion = leerEntero("Seleccione: ", 1, 5);

    if (cantidadGastos() == 0)
    {
        cout << "No hay gastos registrados." << endl;
        return;
    }

    if (opcion == 1)
    {
        int mios[MAX];
        listarMisGastos(mios);
    }
    else if (opcion == 2)
    {
        cout << "Total gastado: " << totalGastado() << " Bs" << endl;
        if (limiteSesion() > 0)
        {
            cout << "Limite de gasto: " << limiteSesion() << " Bs" << endl;
        }
    }
    else if (opcion == 3)
    {
        cout << "Gasto mayor: " << gastoMayor() << " Bs" << endl;
    }
    else if (opcion == 4)
    {
        cout << "Gasto menor: " << gastoMenor() << " Bs" << endl;
    }
    else if (opcion == 5)
    {
        for (int c = 0; c < NUM_CAT_GAS; c++)
        {
            cout << nombreCatGas[c] << ": " << totalGastadoCat(c) << " Bs" << endl;
        }
    }
}

// Menú 3: Modificación o eliminación — Origen: administrarGastos()
void administrarGastos()
{
    int opcion;
    int posicion;
    int mios[MAX];

    if (cantidadGastos() == 0)
    {
        cout << "No hay gastos registrados." << endl;
        return;
    }

    cout << "\n--- ADMINISTRAR GASTOS ---" << endl;
    cout << "1. Modificar gasto" << endl;
    cout << "2. Eliminar gasto" << endl;
    cout << "3. Buscar gasto" << endl;
    opcion = leerEntero("Seleccione: ", 1, 3);

    int k = listarMisGastos(mios);
    posicion = leerEntero("Ingrese el numero del gasto: ", 1, k);
    posicion = mios[posicion - 1];

    if (opcion == 1)
    {
        string desc = leerTexto("Nueva descripcion: ");
        float monto = (float)leerMonto("Ingrese el nuevo monto: ", 0.01, 1000000000.0);
        int cat = elegirCategoriaGasto();
        string fecha = leerFecha("Nueva fecha (dd/mm/aaaa): ");
        if (modificarGasto(posicion, desc, monto, cat, fecha))
        {
            guardar();
            cout << "Gasto modificado correctamente." << endl;
        }
    }
    else if (opcion == 2)
    {
        if (eliminarGasto(posicion))
        {
            guardar();
            cout << "Gasto eliminado correctamente." << endl;
        }
    }
    else if (opcion == 3)
    {
        cout << "Gasto encontrado: " << gastos[posicion] << " Bs - " << gasDesc[posicion] << endl;
    }
}

// Origen: main() de gastos — Joaquín 2
void menuGastos()
{
    int opcion;

    do
    {
        cout << "\n===== GESTION DE GASTOS =====" << endl;
        cout << "1. Registrar gastos" << endl;
        cout << "2. Consultar gastos" << endl;
        cout << "3. Administrar gastos" << endl;
        cout << "0. Salir" << endl;
        opcion = leerEntero("Seleccione una opcion: ", 0, 3);

        switch (opcion)
        {
            case 1:
                registrarGastos();
                break;
            case 2:
                consultarGastos();
                break;
            case 3:
                administrarGastos();
                break;
            case 0:
                cout << "Saliendo al menu principal" << endl;
                break;
        }

    } while (opcion != 0); // Corregido: el original comparaba con 4 y nunca salía
}
