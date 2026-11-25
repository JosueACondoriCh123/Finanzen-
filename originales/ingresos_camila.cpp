CON el main provisional para que lo compiles:
#include <iostream>
#include <string>
#define MAX_ING 100
using namespace std;

string ingDesc[MAX_ING];
double ingMonto[MAX_ING];
int ingCat[MAX_ING];
string ingFecha[MAX_ING];
int ingTotal = 0;

string nombreCatIng[4] = {"Sueldo", "Freelance", "Ventas", "Otros"};

void menuIngresos();
void registrarIngreso();
void menuConsultaIngresos();
void verTodosIngresos();
void buscarIngresosPorCategoria();
void modificarIngreso();
void borrarIngreso();
void resumenIngresos();
int elegirCategoriaIng();
void mostrarIngreso(int i);
bool fechaValidaIng(string f);

int main()
{
    menuIngresos();
    return 0;
}

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
        cout << "\n5. Resumen de ingresos";
        cout << "\n0. Volver al menu principal";
        cout << "\nSeleccione una opcion: ";
        cin >> op;

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
            case 5:
                resumenIngresos();
                break;
            case 0:
                cout << "\nVolviendo al menu principal...";
                break;
            default:
                cout << "\nOpcion invalida";
                break;
        }
    }
}

bool fechaValidaIng(string f)
{
    if (f.length() != 10)
    {
        return false;
    }

    if (f[2] != '/' || f[5] != '/')
    {
        return false;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i != 2 && i != 5)
        {
            if (f[i] < '0' || f[i] > '9')
            {
                return false;
            }
        }
    }

    int dia = (f[0] - '0') * 10 + (f[1] - '0');
    int mes = (f[3] - '0') * 10 + (f[4] - '0');
    int anio = (f[6] - '0') * 1000 + (f[7] - '0') * 100 + (f[8] - '0') * 10 + (f[9] - '0');

    if (anio < 1900 || anio > 2100)
    {
        return false;
    }

    if (mes < 1 || mes > 12)
    {
        return false;
    }

    int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)
    {
        diasMes[1] = 29;
    }

    if (dia < 1 || dia > diasMes[mes - 1])
    {
        return false;
    }

    return true;
}

int elegirCategoriaIng()
{
    int c = 0;

    while (c < 1 || c > 4)
    {
        cout << "\nCategoria:";
        for (int i = 0; i < 4; i++)
        {
            cout << "\n  " << i + 1 << ". " << nombreCatIng[i];
        }
        cout << "\nSeleccione una categoria: ";
        cin >> c;

        if (c < 1 || c > 4)
        {
            cout << "\nCategoria invalida";
        }
    }

    return c - 1;
}

void mostrarIngreso(int i)
{
    cout << "\nIngreso " << i + 1;
    cout << "\nDescripcion: " << ingDesc[i];
    cout << "\nMonto: " << ingMonto[i];
    cout << "\nCategoria: " << nombreCatIng[ingCat[i]];
    cout << "\nFecha: " << ingFecha[i];
    cout << "\n----------------------";
}

void registrarIngreso()
{
    if (ingTotal >= MAX_ING)
    {
        cout << "\nNo hay espacio para mas ingresos";
        return;
    }

    cout << "\n--- REGISTRAR INGRESO ---";

    cin.ignore();
    cout << "\nDescripcion: ";
    getline(cin, ingDesc[ingTotal]);

    cout << "Monto: ";
    cin >> ingMonto[ingTotal];
    while (ingMonto[ingTotal] <= 0)
    {
        cout << "Monto invalido, ingrese un valor mayor a 0: ";
        cin >> ingMonto[ingTotal];
    }

    ingCat[ingTotal] = elegirCategoriaIng();

    cin.ignore();
    cout << "Fecha (dd/mm/aaaa): ";
    getline(cin, ingFecha[ingTotal]);
    while (!fechaValidaIng(ingFecha[ingTotal]))
    {
        cout << "Fecha invalida, use el formato dd/mm/aaaa con una fecha real: ";
        getline(cin, ingFecha[ingTotal]);
    }

    ingTotal++;

    cout << "\nIngreso registrado correctamente";
}

void menuConsultaIngresos()
{
    int z = 9;

    while (z != 0)
    {
        cout << "\n===== CONSULTA DE INGRESOS =====";
        cout << "\n1. Ver todos los ingresos";
        cout << "\n2. Buscar por categoria";
        cout << "\n0. Volver";
        cout << "\nSeleccione una opcion: ";
        cin >> z;

        switch (z)
        {
            case 1:
                verTodosIngresos();
                break;
            case 2:
                buscarIngresosPorCategoria();
                break;
            case 0:
                break;
            default:
                cout << "\nOpcion invalida";
                break;
        }
    }
}

void verTodosIngresos()
{
    cout << "\n--- LISTA DE INGRESOS ---";

    if (ingTotal == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    for (int i = 0; i < ingTotal; i++)
    {
        mostrarIngreso(i);
    }
}

void buscarIngresosPorCategoria()
{
    if (ingTotal == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    int c = elegirCategoriaIng();
    int encontrados = 0;

    cout << "\n--- INGRESOS DE " << nombreCatIng[c] << " ---";

    for (int i = 0; i < ingTotal; i++)
    {
        if (ingCat[i] == c)
        {
            mostrarIngreso(i);
            encontrados++;
        }
    }

    if (encontrados == 0)
    {
        cout << "\nNo hay ingresos en esta categoria";
    }
}

void modificarIngreso()
{
    int p;

    if (ingTotal == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    cout << "\n--- MODIFICAR INGRESO ---";

    for (int i = 0; i < ingTotal; i++)
    {
        cout << "\n" << i + 1 << ". " << ingDesc[i] << " - " << ingMonto[i];
    }

    cout << "\nSeleccione el ingreso que desea modificar: ";
    cin >> p;

    if (p < 1 || p > ingTotal)
    {
        cout << "\nIngreso no encontrado";
        return;
    }

    cin.ignore();
    cout << "\nNueva descripcion: ";
    getline(cin, ingDesc[p - 1]);

    cout << "Nuevo monto: ";
    cin >> ingMonto[p - 1];
    while (ingMonto[p - 1] <= 0)
    {
        cout << "Monto invalido, ingrese un valor mayor a 0: ";
        cin >> ingMonto[p - 1];
    }

    ingCat[p - 1] = elegirCategoriaIng();

    cin.ignore();
    cout << "Nueva fecha (dd/mm/aaaa): ";
    getline(cin, ingFecha[p - 1]);
    while (!fechaValidaIng(ingFecha[p - 1]))
    {
        cout << "Fecha invalida, use el formato dd/mm/aaaa con una fecha real: ";
        getline(cin, ingFecha[p - 1]);
    }

    cout << "\nIngreso modificado correctamente";
}

void borrarIngreso()
{
    int p;

    if (ingTotal == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    cout << "\n--- ELIMINAR INGRESO ---";

    for (int i = 0; i < ingTotal; i++)
    {
        cout << "\n" << i + 1 << ". " << ingDesc[i] << " - " << ingMonto[i];
    }

    cout << "\nSeleccione el ingreso que desea eliminar: ";
    cin >> p;

    if (p < 1 || p > ingTotal)
    {
        cout << "\nIngreso no encontrado";
        return;
    }

    for (int i = p - 1; i < ingTotal - 1; i++)
    {
        ingDesc[i] = ingDesc[i + 1];
        ingMonto[i] = ingMonto[i + 1];
        ingCat[i] = ingCat[i + 1];
        ingFecha[i] = ingFecha[i + 1];
    }

    ingTotal--;

    cout << "\nIngreso eliminado correctamente";
}

void resumenIngresos()
{
    double suma = 0;
    double porCat[4] = {0, 0, 0, 0};

    cout << "\n--- RESUMEN DE INGRESOS ---";

    if (ingTotal == 0)
    {
        cout << "\nNo hay ingresos registrados";
        return;
    }

    for (int i = 0; i < ingTotal; i++)
    {
        suma = suma + ingMonto[i];
        porCat[ingCat[i]] = porCat[ingCat[i]] + ingMonto[i];
    }

    cout << "\nCantidad de ingresos: " << ingTotal;
    cout << "\nTotal acumulado: " << suma;
    cout << "\n\nPor categoria:";
    for (int i = 0; i < 4; i++)
    {
        cout << "\n  " << nombreCatIng[i] << ": " << porCat[i];
    }
    cout << "\nPromedio por ingreso: " << suma / ingTotal;
}