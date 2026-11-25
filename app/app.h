// FinanZen — app gráfica (raylib) · integrado con ayuda de Claude Code (IA)
// Declaraciones compartidas por todas las pantallas de la app.
#ifndef APP_H
#define APP_H
#include "raylib.h"
#include "../motor/finanzen.h"

// Tamaño "virtual" de la pantalla: se dibuja en 450 x 900 y se escala al celular.
#define ANCHO 450
#define ALTO 900

// ---------- Pantallas (una por menú) ----------
enum Pantallas
{
    PANT_BIENVENIDA, PANT_REGISTRO, PANT_LOGIN, PANT_INICIO,
    // Módulo 1 — Joaquín 1
    PANT_PERFIL, PANT_PERFIL_EDITAR, PANT_PERFIL_BORRAR,
    // Módulo 2 — Camila
    PANT_INGRESOS, PANT_ING_FORM, PANT_ING_CONSULTA, PANT_ING_ADMIN,
    // Módulo 3 — Joaquín 2
    PANT_GASTOS, PANT_GAS_FORM, PANT_GAS_LIMITE, PANT_GAS_CONSULTA, PANT_GAS_ADMIN,
    // Módulo 4 — Hector
    PANT_AHORRO, PANT_META_FORM, PANT_META_CALCULO, PANT_META_MODIFICAR,
    // Módulo 5 — Josue
    PANT_REPORTES, PANT_REP_AHORRO, PANT_REP_PROGRESO, PANT_REP_RESUMEN,
    TOTAL_PANTALLAS
};

extern int pantalla;
void irA(int nueva);       // cambia de pantalla y limpia el teclado
int padreDe(int p);        // a dónde vuelve el botón "atrás"

// ---------- Estilo (estilo.cpp) ----------
extern Color FONDO, TARJETA, TARJETA2, TEXTO, TEXTO2, VERDE, ROJO, AMARILLO;
extern Color COLOR_PERFIL, COLOR_ING, COLOR_GAS, COLOR_AHO, COLOR_REP;

void cargarEstilo();
void descargarEstilo();
void texto(string s, float x, float y, float tam, Color c);
void textoNegrita(string s, float x, float y, float tam, Color c);
void textoCentrado(string s, float y, float tam, Color c, bool negrita);
float anchoTexto(string s, float tam, bool negrita);
void dibujarTarjeta(Rectangle r, Color c);
bool tocado(Rectangle r);                       // un toque (no un arrastre) dentro de r
bool botonGrande(Rectangle r, string s, Color c);
bool botonSecundario(Rectangle r, string s);
bool tarjetaMenu(float y, string numero, string titulo, string detalle, Color c);
bool encabezado(string titulo, Color c);        // dibuja la barra superior; true si tocaron "atrás"
void barraProgreso(Rectangle r, double porcentaje, Color c);
void aviso(string mensaje);                     // cartel de 2,5 s
void dibujarAviso();
string primeraLetra(string s);                  // primera letra aunque tenga tilde (UTF-8)
string formatoBs(double v);                     // "Bs 1.240,50"
string hoy();                                   // fecha de hoy dd/mm/aaaa
bool leerMontoTexto(string s, double &v);       // "1240,50" -> 1240.5
string montoATexto(double v);                   // 1240.5 -> "1240,50"
int filaTocada(Rectangle area, int cantidad, float altoFila, float desplazamiento);
float desplazar(Rectangle area, float &desplazamiento, float altoContenido);
void actualizarToques();
bool chips(float y, string nombres[], int cantidad, int &elegido, Color c);
void barraInferior();

// ---------- Teclado propio (teclado.cpp) ----------
#define TECLADO_TEXTO 0
#define TECLADO_NUMERO 1
#define TECLADO_FECHA 2
#define TECLADO_CLAVE 3
#define TECLADO_ENTERO 4 // solo dígitos (meses)
extern int campoActivo;  // -1 = ningún campo activo (teclado oculto)
bool campoTexto(Rectangle r, string etiqueta, string &valor, int tipo, int id);
bool dibujarTeclado();   // true cuando tocan "Listo"
void cerrarTeclado();
bool tecladoAbierto();

// ---------- Pantallas ----------
void dibujarLogo(float cx, float cy, float r);
void dibujarBienvenida();
void dibujarRegistro();
void dibujarLogin();
void dibujarInicio();
void dibujarPerfil();
void dibujarPerfilEditar();
void dibujarPerfilBorrar();
void dibujarIngresos();
void dibujarIngForm();
void dibujarIngConsulta();
void dibujarIngAdmin();
void prepararIngreso(int indice); // -1 = nuevo
void dibujarGastos();
void dibujarGasForm();
void dibujarGasLimite();
void dibujarGasConsulta();
void dibujarGasAdmin();
void prepararGasto(int indice);
void dibujarAhorro();
void dibujarMetaForm();
void dibujarMetaCalculo();
void dibujarMetaModificar();
void prepararMeta();
void prepararAhorro();
void dibujarReportes();
void dibujarRepAhorro();
void dibujarRepProgreso();
void dibujarRepResumen();

void guardarApp(); // guardarTodo() con aviso si falla

#endif
