// FinanZen — módulo 2 (Camila): gestión de ingresos · integrado con ayuda de Claude Code (IA)
// Lógica tomada de originales/ingresos_camila.cpp. Los cin/cout originales
// quedaron en consola/menu_ingresos.cpp.
#include "finanzen.h"

// Arreglos globales (originales)
string ingDesc[MAX_ING];
double ingMonto[MAX_ING];
int ingCat[MAX_ING];
string ingFecha[MAX_ING];
int ingTotal = 0;

string nombreCatIng[4] = {"Sueldo", "Freelance", "Ventas", "Otros"};

// Agregado para unir módulos: id del usuario dueño de cada ingreso
int ingUsuario[MAX_ING];

// Origen: fechaValidaIng() — Camila (copia exacta; la usan todos los módulos)
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

// Agregado: Las validaciones que Camila hacía con while al pedir los datos
bool datosIngresoValidos(string desc, double monto, int cat, string fecha)
{
    if (!textoValido(desc))
    {
        return false;
    }
    if (monto <= 0 || monto > MONTO_MAXIMO || !montoConDosDecimales(monto)) // "Monto invalido, ingrese un valor mayor a 0"
    {
        return false;
    }
    if (cat < 0 || cat > 3) // "Categoria invalida"
    {
        return false;
    }
    if (!fechaValidaIng(fecha)) // "Fecha invalida, use el formato dd/mm/aaaa"
    {
        return false;
    }
    return true;
}

// Origen: registrarIngreso() — Camila
bool agregarIngreso(string desc, double monto, int cat, string fecha)
{
    if (sesion == -1)
    {
        return false;
    }
    if (ingTotal >= MAX_ING)
    {
        return false; // "No hay espacio para mas ingresos"
    }
    if (!datosIngresoValidos(desc, monto, cat, fecha))
    {
        return false;
    }

    ingDesc[ingTotal] = desc;
    ingMonto[ingTotal] = monto;
    ingCat[ingTotal] = cat;
    ingFecha[ingTotal] = fecha;
    ingUsuario[ingTotal] = sesion;

    ingTotal++;
    return true; // "Ingreso registrado correctamente"
}

// Agregado: Un índice es del usuario conectado
bool ingresoEsMio(int i)
{
    return sesion != -1 && i >= 0 && i < ingTotal && ingUsuario[i] == sesion;
}

// Origen: modificarIngreso() — Camila (i es la posición en los arreglos, desde 0)
bool cambiarIngreso(int i, string desc, double monto, int cat, string fecha)
{
    if (!ingresoEsMio(i))
    {
        return false; // "Ingreso no encontrado"
    }
    if (!datosIngresoValidos(desc, monto, cat, fecha))
    {
        return false;
    }

    ingDesc[i] = desc;
    ingMonto[i] = monto;
    ingCat[i] = cat;
    ingFecha[i] = fecha;
    return true; // "Ingreso modificado correctamente"
}

// Origen: borrarIngreso() — Camila
bool quitarIngreso(int p)
{
    if (!ingresoEsMio(p))
    {
        return false; // "Ingreso no encontrado"
    }

    for (int i = p; i < ingTotal - 1; i++)
    {
        ingDesc[i] = ingDesc[i + 1];
        ingMonto[i] = ingMonto[i + 1];
        ingCat[i] = ingCat[i + 1];
        ingFecha[i] = ingFecha[i + 1];
        ingUsuario[i] = ingUsuario[i + 1];
    }

    ingTotal--;
    return true; // "Ingreso eliminado correctamente"
}

// Origen: resumenIngresos() — Camila (suma total del usuario conectado)
double sumaIngresos()
{
    double suma = 0;
    for (int i = 0; i < ingTotal; i++)
    {
        if (ingUsuario[i] == sesion)
        {
            suma = suma + ingMonto[i];
        }
    }
    return suma;
}

// Origen: resumenIngresos() — Camila (porCat[])
double sumaIngresosCat(int cat)
{
    double suma = 0;
    for (int i = 0; i < ingTotal; i++)
    {
        if (ingUsuario[i] == sesion && ingCat[i] == cat)
        {
            suma = suma + ingMonto[i];
        }
    }
    return suma;
}

// Agregado: cuántos ingresos tiene el usuario conectado
int cantidadIngresos()
{
    int c = 0;
    for (int i = 0; i < ingTotal; i++)
    {
        if (ingUsuario[i] == sesion)
        {
            c++;
        }
    }
    return c;
}

// Agregado: al borrar una cuenta se borran sus ingresos
void borrarIngresosDeUsuario(int id)
{
    int j = 0;
    for (int i = 0; i < ingTotal; i++)
    {
        if (ingUsuario[i] != id)
        {
            ingDesc[j] = ingDesc[i];
            ingMonto[j] = ingMonto[i];
            ingCat[j] = ingCat[i];
            ingFecha[j] = ingFecha[i];
            ingUsuario[j] = ingUsuario[i];
            j++;
        }
    }
    ingTotal = j;
}
