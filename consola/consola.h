// FinanZen — versión consola · integrado con ayuda de Claude Code (IA)
#ifndef CONSOLA_H
#define CONSOLA_H
#include "../motor/finanzen.h"

// leer.cpp — lectores robustos de Hector (originales: leerEnteroAhorro, leerMontoAhorro, leerTextoAhorro)
int leerEntero(string mensaje, int minimo, int maximo);
double leerMonto(string mensaje, double minimo, double maximo);
string leerTexto(string mensaje);
string leerFecha(string mensaje);
void guardar(); // guardarTodo() con aviso si falla

// menu_usuarios.cpp — Joaquín 1
void registrarConsola();
bool iniciarConsola();
void lista();
void menuPerfil();

// menu_ingresos.cpp — Camila
void menuIngresos();

// menu_gastos.cpp — Joaquín 2
void menuGastos();

// menu_ahorro.cpp — Hector
void menuPlanificacionAhorro(int usuario);
void modificarMetaAhorro(int usuario);

// menu_reportes.cpp — Josue
void menuReportes();

#endif
