// FinanZen — módulo 2 (Camila): menús de ingresos · integrado con ayuda de Claude Code (IA)
// Textos tomados de originales/ingresos_camila.cpp; la lógica está en motor/ingresos.cpp.
#include "consola.h"
#include <iostream>

// Origen: elegirCategoriaIng() — Camila
int elegirCategoriaIng()
{
    cout << "\nCategoria:";
    for (int i = 0; i < 4; i++)
    {
        cout << "\n  " << i + 1 << ". " << nombreCatIng[i];
    }
    cout << "\n";
    return leerEntero("Seleccione una categoria: ", 1, 4) - 1;
}

// Origen: mostrarIngreso() — Camila
void mostrarIngreso(int numero, int i)
{
    cout << "\nIngreso " << numero;
    cout << "\nDescripcion: " << ingDesc[i];
    cout << "\nMonto: " << ingMonto[i];
    cout << "\nCategoria: " << nombreCatIng[ingCat[i]];
    cout << "\nFecha: " << ingFecha[i];
    cout << "\n----------------------";
}

// Lista numerada de los ingresos del usuario; mios[] guarda su posición real en los arreglos
int listarMisIngresos(int mios[])
{
    int k = 0;
    for (int i = 0; i < ingTotal; i++)
    {
        if (ingUsuario[i] == sesion)
        {
            mios[k] = i;
            k++;
            cout << "\n" << k << ". " << ingDesc[i] << " - " << ingMonto[i];
        }
    }
    return k;
}

// Menú 1: Registro de ingresos — Origen: registrarIngreso()
void registrarIngreso()
{
    if (ingTotal >= MAX_ING)
    {
        cout << "\nNo hay espacio para mas ingresos";
        return;
    }
    cout << "\n--- REGISTRAR INGRESO ---\n";
    string desc = leerTexto("Descripcion: ");
    double monto = leerMonto("Monto: ", 0.01, 1000000000.0);
    int cat = elegirCategoriaIng();
    string fecha = leerFecha("Fecha (dd/mm/aaaa): ");

    if (agregarIngreso(desc, monto, cat, fecha))
    {
        guardar();
        cout << "\nIngreso registrado correctamente";
    }
    else
    {
        cout << "\nNo se pudo registrar: revise la descripcion";
    }
}

// Origen: verTodosIngresos() — Camila
void verTodosIngresos()
{
    cout << "\n--- LISTA DE INGRESOS ---";
    int k = 0;
    for (int i = 0; i < ingTotal; i++)
    {
        if (ingUsuario[i] == sesion)
        {
            k++;
            mostrarIngreso(k, i);
        }
    }
    if (k == 0)
    {
        cout << "\nNo hay ingresos registrados";
    }
}

// Origen: buscarIngresosPorCategoria() — Camila
void buscarIngresosPorCategoria()
{
    if (cantidadIngresos() == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    int c = elegirCategoriaIng();
    int encontrados = 0;

    cout << "\n--- INGRESOS DE " << nombreCatIng[c] << " ---";

    for (int i = 0; i < ingTotal; i++)
    {
        if (ingUsuario[i] == sesion && ingCat[i] == c)
        {
            encontrados++;
            mostrarIngreso(encontrados, i);
        }
    }

    if (encontrados == 0)
    {
        cout << "\nNo hay ingresos en esta categoria";
    }
}

// Origen: resumenIngresos() — Camila
void resumenIngresos()
{
    cout << "\n--- RESUMEN DE INGRESOS ---";

    int cantidad = cantidadIngresos();
    if (cantidad == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    double suma = sumaIngresos();
    cout << "\nCantidad de ingresos: " << cantidad;
    cout << "\nTotal acumulado: " << suma;
    cout << "\n\nPor categoria:";
    for (int i = 0; i < 4; i++)
    {
        cout << "\n  " << nombreCatIng[i] << ": " << sumaIngresosCat(i);
    }
    cout << "\nPromedio por ingreso: " << suma / cantidad;
}

// Menú 2: Consulta de ingresos — Origen: menuConsultaIngresos()
void menuConsultaIngresos()
{
    int z = 9;

    while (z != 0)
    {
        cout << "\n===== CONSULTA DE INGRESOS =====";
        cout << "\n1. Ver todos los ingresos";
        cout << "\n2. Buscar por categoria";
        cout << "\n3. Resumen de ingresos";
        cout << "\n0. Volver\n";
        z = leerEntero("Seleccione una opcion: ", 0, 3);

        switch (z)
        {
            case 1:
                verTodosIngresos();
                break;
            case 2:
                buscarIngresosPorCategoria();
                break;
            case 3:
                resumenIngresos();
                break;
            case 0:
                break;
        }
    }
}

// Menú 3: Modificación de ingresos — Origen: modificarIngreso() y borrarIngreso()
void modificarIngreso()
{
    int mios[MAX_ING];

    cout << "\n--- MODIFICAR INGRESO ---";
    int k = listarMisIngresos(mios);
    if (k == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    int p = leerEntero("\nSeleccione el ingreso que desea modificar: ", 1, k);

    string desc = leerTexto("Nueva descripcion: ");
    double monto = leerMonto("Nuevo monto: ", 0.01, 1000000000.0);
    int cat = elegirCategoriaIng();
    string fecha = leerFecha("Nueva fecha (dd/mm/aaaa): ");

    if (cambiarIngreso(mios[p - 1], desc, monto, cat, fecha))
    {
        guardar();
        cout << "\nIngreso modificado correctamente";
    }
}

void borrarIngreso()
{
    int mios[MAX_ING];

    cout << "\n--- ELIMINAR INGRESO ---";
    int k = listarMisIngresos(mios);
    if (k == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    int p = leerEntero("\nSeleccione el ingreso que desea eliminar: ", 1, k);

    if (quitarIngreso(mios[p - 1]))
    {
        guardar();
        cout << "\nIngreso eliminado correctamente";
    }
}

// Origen: menuIngresos() — Camila
void menuIngresos()
{
    int op = 9;

    while (op != 0)
    {
        cout << "\n===== GESTION DE INGRESOS =====";
        cout << "\n1. Registro de ingresos";
        cout << "\n2. Consulta de ingresos";
        cout << "\n3. Modificacion de ingresos";
        cout << "\n4. Eliminar ingreso";
        cout << "\n0. Volver al menu principal\n";
        op = leerEntero("Seleccione una opcion: ", 0, 4);

        switch (op)
        {
            case 1:
                registrarIngreso();
                break;
            case 2:
                menuConsultaIngresos();
                break;
            case 3:
                modificarIngreso();
                break;
            case 4:
                borrarIngreso();
                break;
            case 0:
                cout << "\nVolviendo al menu principal...";
                break;
        }
    }
}
