// FinanZen — prueba del teclado: el plazo en meses acepta 10 o más (revisión final)
// Compilar: g++ -std=c++17 pruebas/prueba_teclado.cpp app/teclado.cpp app/estilo.cpp motor/*.cpp -Iterceros/raylib/src -Lbuild -lraylib -lopengl32 -lgdi32 -lwinmm -o build/prueba_teclado.exe
#include <cassert>
#include <iostream>
#include "../app/app.h"
void agregarLetra(string &valor, string letra, int tipo);
// La navegación no se usa en esta prueba
int pantalla = 0;
void irA(int nueva) { pantalla = nueva; }
int padreDe(int p) { return p; }
int main()
{
    string v = "";
    agregarLetra(v, "1", TECLADO_ENTERO);
    agregarLetra(v, "2", TECLADO_ENTERO);
    agregarLetra(v, ",", TECLADO_ENTERO);
    assert(v == "12");
    string f = "";
    agregarLetra(f, "1", TECLADO_FECHA);
    agregarLetra(f, "2", TECLADO_FECHA);
    assert(f == "12/"); // la fecha sí agrega la barra
    string c = "";
    agregarLetra(c, "a", TECLADO_CLAVE);
    agregarLetra(c, "ñ", TECLADO_CLAVE);
    agregarLetra(c, "é", TECLADO_CLAVE);
    agregarLetra(c, "1", TECLADO_CLAVE);
    assert(c == "a1"); // la contraseña no acepta ñ ni tildes (se cifra)
    std::cout << "OK" << std::endl;
    return 0;
}
