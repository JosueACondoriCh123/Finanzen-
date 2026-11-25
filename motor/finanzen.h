// FinanZen — motor común · integrado con ayuda de Claude Code (IA)
// Aquí se declaran los arreglos y funciones de los 5 módulos del equipo.
// Cada módulo los define en su propio archivo .cpp dentro de motor/.
#ifndef FINANZEN_H
#define FINANZEN_H

#include <string>
using namespace std;

#define MAX 100
#define MONTO_MAXIMO 1000000000.0 // tope común de montos (el de Hector), igual en consola y app

// ===== Módulo 1: Usuarios (Joaquín 1) — motor/usuarios.cpp =====
extern string usu[MAX];   // nombres (original)
extern string con[MAX];   // contraseñas (original)
extern int usuId[MAX];    // id fijo de cada usuario (agregado para unir módulos)
extern int total;         // cantidad de usuarios (original)
extern int proximoId;     // próximo id a entregar
extern int sesion;        // id del usuario conectado, -1 si no hay nadie

int buscarUsuario(string nombre);
bool registrar(string nombre, string clave);
bool iniciar(string nombre, string clave);
void cerrarSesion();
bool modificar(int pos, string nombre, string clave);
bool borrar(int pos);
int indiceSesion();
string nombreSesion();

// ===== Módulo 2: Ingresos (Camila) — motor/ingresos.cpp =====
#define MAX_ING 100
extern string ingDesc[MAX_ING];
extern double ingMonto[MAX_ING];
extern int ingCat[MAX_ING];
extern string ingFecha[MAX_ING];
extern int ingUsuario[MAX_ING]; // agregado: id del dueño
extern int ingTotal;
extern string nombreCatIng[4];

bool fechaValidaIng(string f);
bool agregarIngreso(string desc, double monto, int cat, string fecha);
bool cambiarIngreso(int i, string desc, double monto, int cat, string fecha);
bool quitarIngreso(int i);
double sumaIngresos();
double sumaIngresosCat(int cat);
int cantidadIngresos();
void borrarIngresosDeUsuario(int id);

// ===== Módulo 3: Gastos (Joaquín 2) — motor/gastos.cpp =====
#define NUM_CAT_GAS 6
extern float gastos[MAX];       // montos (original)
extern int nGastos;             // cantidad (original: parámetro n)
extern string gasDesc[MAX];
extern int gasCat[MAX];
extern string gasFecha[MAX];
extern int gasUsuario[MAX];
extern float limiteGasto[MAX];  // límite por posición de usuario (original: limite)
extern string nombreCatGas[NUM_CAT_GAS];

bool registrarGasto(string desc, float monto, int cat, string fecha);
bool establecerLimite(float limite);
float limiteSesion();
double totalGastado();
float gastoMayor();
float gastoMenor();
double totalGastadoCat(int cat);
int cantidadGastos();
bool modificarGasto(int i, string desc, float monto, int cat, string fecha);
bool eliminarGasto(int i);
void borrarGastosDeUsuario(int id);
void reiniciarLimiteUsuario(int pos);
void eliminarLimiteDeUsuario(int pos, int totalAntes);

// ===== Módulo 4: Planificación del ahorro (Hector) — motor/ahorro.cpp =====
static const int MAX_METAS_AHORRO = 100;
extern string nombreMeta[MAX_METAS_AHORRO];
extern double montoMeta[MAX_METAS_AHORRO];
extern double ahorroActual[MAX_METAS_AHORRO];
extern int mesesMeta[MAX_METAS_AHORRO];
extern bool tieneMeta[MAX_METAS_AHORRO];

bool montoConDosDecimales(double v);
double calcularFaltanteAhorro(int usuario);
double obtenerAhorroMensual(int usuario);
bool guardarNuevaMeta(int usuario, string nombre, double monto, double actual, int meses);
bool modificarMetaCompleta(int usuario, string nombre, double monto, double actual, int meses);
bool metaValida(string nombre, double monto, double actual, int meses);
double cuotaMensual(double monto, double actual, int meses);
bool cambiarNombreMeta(int usuario, string nombre);
bool cambiarMontoMeta(int usuario, double monto);
bool actualizarAhorroAcumulado(int usuario, double actual);
bool cambiarPlazoMeta(int usuario, int meses);
long long mesesConAporte(int usuario, double aporte);
int proyeccionAhorro(int usuario, double aporte[], double acumulado[], int max);
void reiniciarMetaUsuario(int usuario);
void eliminarMetaDeUsuario(int usuario, int totalAntes);

// ===== Módulo 5: Seguimiento y reportes (Josue) — motor/reportes.cpp =====
extern double ahoMonto[MAX];
extern string ahoFecha[MAX];
extern int ahoUsuario[MAX];
extern int ahoTotal;

bool registro_ahorro(double monto, string fecha);
double progreso_ahorro();
void resumen_financiero(double &ingresos, double &gastosTotales, double &ahorrado, double &saldo);
double totalAhorradoRegistrado();
double saldoDisponible();
void borrarAhorrosDeUsuario(int id);

// ===== Cifrado de contraseñas — motor/cifrado.cpp (estilo del ejemplo del equipo) =====
#define LLAVE_CIFRADO 100
string cifrar(string mensaje, int llaveCifrado);
string descifrar(string cifrado, int llaveDescifrado);
bool claveValida(string clave);

// ===== Común — motor/archivos.cpp =====
extern string carpetaDatos;
bool textoValido(string t);
void borrarDatosDeUsuario(int pos);
void reiniciarTodo();
bool guardarTodo();
bool cargarTodo();

#endif
