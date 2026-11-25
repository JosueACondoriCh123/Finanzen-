// FinanZen — lectura de datos por consola · integrado con ayuda de Claude Code (IA)
// Origen: leerEnteroAhorro(), leerMontoAhorro() y leerTextoAhorro() — Hector.
// Se usan en todos los menús para que una letra donde va un número no cuelgue el programa.
#include "consola.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>

// Origen: leerEnteroAhorro() — Hector
int leerEntero(string mensaje, int minimo, int maximo)
{
    int valor;
    while (true)
    {
        cout << mensaje;
        if (cin >> valor && valor >= minimo && valor <= maximo &&
            (cin.peek() == '\n' || cin.peek() == '\r' || cin.peek() == EOF))
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente nuevamente.\n";
    }
}

// Origen: leerMontoAhorro() — Hector
double leerMonto(string mensaje, double minimo, double maximo)
{
    double valor;
    while (true)
    {
        cout << mensaje;
        if (cin >> valor && std::isfinite(valor) && valor >= minimo && valor <= maximo &&
            montoConDosDecimales(valor) &&
            (cin.peek() == '\n' || cin.peek() == '\r' || cin.peek() == EOF))
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Monto invalido. Use hasta dos decimales e intente nuevamente.\n";
    }
}

// Origen: leerTextoAhorro() — Hector (además rechaza '|', que separa campos en los .txt)
string leerTexto(string mensaje)
{
    string texto;
    while (true)
    {
        cout << mensaje;
        if (!getline(cin, texto)) exit(0);
        if (texto.length() > 0 && texto[texto.length() - 1] == '\r')
        {
            texto = texto.substr(0, texto.length() - 1);
        }
        if (textoValido(texto)) return texto;
        cout << "El texto no puede estar vacio ni tener '|'.\n";
    }
}

// Origen: el while de fechas de registrarIngreso() — Camila
string leerFecha(string mensaje)
{
    string f = leerTexto(mensaje);
    while (!fechaValidaIng(f))
    {
        f = leerTexto("Fecha invalida, use el formato dd/mm/aaaa con una fecha real: ");
    }
    return f;
}

void guardar()
{
    if (!guardarTodo())
    {
        cout << "\nAviso: no se pudieron guardar los archivos .txt";
    }
}
