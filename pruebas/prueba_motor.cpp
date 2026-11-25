// FinanZen — pruebas automáticas del motor · integrado con ayuda de Claude Code (IA)
// Compilar desde finanzen/:
//   g++ -std=c++17 -Wall -Wextra pruebas/prueba_motor.cpp motor/*.cpp -o build/prueba_motor.exe
#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <filesystem>
#include "../motor/finanzen.h"
using namespace std;

bool igual(double a, double b) { return fabs(a - b) < 0.001; }

// ---------- Tarea 2: usuarios ----------
void prueba_usuarios()
{
    assert(registrar("Ana", "1"));
    assert(!registrar("Ana", "2"));
    assert(!registrar("", "x"));
    assert(!iniciar("Ana", "mal"));
    assert(iniciar("Ana", "1"));
    assert(nombreSesion() == "Ana");
    assert(indiceSesion() == 0);
    assert(modificar(0, "Ana B", "2"));
    assert(nombreSesion() == "Ana B");
    assert(borrar(0));
    assert(total == 0);
    assert(sesion == -1);
}

void prueba_texto_invalido()
{
    assert(!textoValido("a|b"));
    assert(!textoValido("   "));
    assert(!textoValido(""));
    assert(textoValido("Comida"));
    assert(!registrar("Pe|pe", "1"));
}

// ---------- Tarea 3: ingresos ----------
void prueba_ingresos()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    assert(agregarIngreso("Sueldo noviembre", 3000, 0, "01/11/2026"));
    assert(agregarIngreso("Venta bici", 500, 2, "05/11/2026"));
    assert(igual(sumaIngresos(), 3500));
    assert(igual(sumaIngresosCat(2), 500));
    assert(!agregarIngreso("Nada", 0, 0, "01/11/2026"));
    assert(!agregarIngreso("Feb", 10, 0, "31/02/2026"));
    assert(!agregarIngreso("Cat", 10, 7, "01/11/2026"));
    assert(cambiarIngreso(0, "Sueldo noviembre", 3200, 0, "01/11/2026"));
    assert(igual(sumaIngresos(), 3700));
    assert(quitarIngreso(1));
    assert(cantidadIngresos() == 1);
    cerrarSesion();
    assert(!agregarIngreso("Sin sesion", 10, 0, "01/11/2026"));
}

void prueba_ajeno()
{
    registrar("Ana", "1");
    registrar("Beto", "2");
    iniciar("Ana", "1");
    agregarIngreso("Sueldo", 3000, 0, "01/11/2026");
    iniciar("Beto", "2");
    assert(!quitarIngreso(0));
    assert(!cambiarIngreso(0, "x", 1, 0, "01/11/2026"));
    assert(cantidadIngresos() == 0);
    iniciar("Ana", "1");
    registrarGasto("Pan", 10, 0, "02/11/2026");
    iniciar("Beto", "2");
    assert(!eliminarGasto(0));
    assert(!modificarGasto(0, "x", 1, 0, "02/11/2026"));
    assert(cantidadGastos() == 0);
}

// ---------- Tarea 4: gastos ----------
void prueba_gastos()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    assert(registrarGasto("Almuerzo", 100, 0, "02/11/2026"));
    assert(registrarGasto("Micro", 40, 1, "03/11/2026"));
    assert(registrarGasto("Cine", 250, 3, "04/11/2026"));
    assert(!registrarGasto("Nada", -5, 0, "04/11/2026"));
    assert(!registrarGasto("Cat", 5, 6, "04/11/2026"));
    assert(igual(totalGastado(), 390));
    assert(igual(gastoMayor(), 250));
    assert(igual(gastoMenor(), 40));
    assert(igual(totalGastadoCat(0), 100));
    assert(establecerLimite(800));
    assert(igual(limiteSesion(), 800));
    assert(!establecerLimite(-1));
    assert(eliminarGasto(1));
    assert(igual(totalGastado(), 350));
    assert(modificarGasto(0, "Almuerzo", 120, 0, "02/11/2026"));
    assert(igual(totalGastado(), 370));
    assert(cantidadGastos() == 2);
    // float de Joaquín 2: montos grandes con centavos deben aceptarse
    assert(registrarGasto("Tele", 1234.56, 5, "05/11/2026"));
    assert(establecerLimite(2500.75));
}

// ---------- Tarea 5: ahorro ----------
void prueba_ahorro()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    assert(!tieneMeta[0]);
    assert(guardarNuevaMeta(0, "Viaje", 1000, 250, 3));
    assert(tieneMeta[0]);
    assert(igual(calcularFaltanteAhorro(0), 750));
    assert(igual(obtenerAhorroMensual(0), 250));
    assert(mesesConAporte(0, 200) == 4);
    double aporte[20], acumulado[20];
    assert(proyeccionAhorro(0, aporte, acumulado, 20) == 3);
    assert(igual(acumulado[2], 1000));
    assert(!cambiarMontoMeta(0, 100));
    assert(cambiarMontoMeta(0, 1200));
    assert(cambiarPlazoMeta(0, 5));
    assert(!cambiarPlazoMeta(0, 0));
    assert(cambiarNombreMeta(0, "Viaje a Cocha"));
    assert(!actualizarAhorroAcumulado(0, 5000));
    assert(actualizarAhorroAcumulado(0, 300));
    assert(!guardarNuevaMeta(0, "x", 10.555, 0, 1));
    assert(!guardarNuevaMeta(0, "x", 100, 200, 1));
    assert(!cambiarNombreMeta(1, "sin meta"));
}

// ---------- Tarea 6: reportes y borrado en cascada ----------
void prueba_reportes()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    agregarIngreso("Sueldo", 3000, 0, "01/11/2026");
    registrarGasto("Alquiler", 1000, 2, "02/11/2026");
    assert(igual(progreso_ahorro(), -1));
    assert(!registro_ahorro(100, "03/11/2026")); // sin meta
    guardarNuevaMeta(indiceSesion(), "Viaje", 1000, 250, 3);
    assert(registro_ahorro(500, "03/11/2026"));
    assert(!registro_ahorro(10, "99/99/2026"));
    double ing, gas, aho, saldo;
    resumen_financiero(ing, gas, aho, saldo);
    assert(igual(ing, 3000));
    assert(igual(gas, 1000));
    assert(igual(aho, 500));
    assert(igual(saldo, 1500));
    assert(igual(saldoDisponible(), 1500));
    assert(igual(progreso_ahorro(), 75));
    assert(igual(ahorroActual[indiceSesion()], 750));
}

void prueba_ahorro_sin_saldo()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    agregarIngreso("Sueldo", 100, 0, "01/11/2026");
    guardarNuevaMeta(0, "Moto", 1000, 0, 10);
    assert(!registro_ahorro(200, "02/11/2026")); // más que el saldo
    agregarIngreso("Extra", 1000, 3, "01/11/2026");
    guardarNuevaMeta(0, "Celu", 100, 50, 1);
    assert(!registro_ahorro(60, "02/11/2026")); // más de lo que falta
    assert(registro_ahorro(50, "02/11/2026"));
    assert(igual(progreso_ahorro(), 100));
}

void prueba_borrado_en_medio()
{
    string nombres[3] = {"A", "B", "C"};
    for (int k = 0; k < 3; k++)
    {
        registrar(nombres[k], "1");
        iniciar(nombres[k], "1");
        agregarIngreso("Sueldo", 1000 * (k + 1), 0, "01/11/2026");
        registrarGasto("Gasto", 10 * (k + 1), 0, "02/11/2026");
        establecerLimite(100 * (k + 1));
        guardarNuevaMeta(k, "Meta " + nombres[k], 500, 0, 5);
        registro_ahorro(5 * (k + 1), "03/11/2026");
    }
    iniciar("B", "1"); // cada usuario borra solo su propia cuenta
    assert(borrar(1));
    assert(total == 2);
    assert(usu[1] == "C");
    assert(nombreMeta[1] == "Meta C");
    assert(!tieneMeta[2]);
    assert(nGastos == 2 && ingTotal == 2 && ahoTotal == 2);
    iniciar("C", "1");
    assert(igual(totalGastado(), 30));
    assert(igual(limiteSesion(), 300));
    assert(igual(sumaIngresos(), 3000));
    assert(igual(totalAhorradoRegistrado(), 15));
    iniciar("A", "1");
    assert(igual(totalGastado(), 10));
    assert(igual(limiteSesion(), 100));
    assert(nombreMeta[indiceSesion()] == "Meta A");
}

// ---------- Tarea 7: archivos ----------
void carpetaVacia(string c)
{
    std::filesystem::remove_all(c);
    std::filesystem::create_directories(c);
    carpetaDatos = c;
}

void prueba_archivos_tildes()
{
    carpetaVacia("build/prueba_datos/");
    registrar("Íñigo Peña", "Clave#2026");
    registrar("Beto", "2");
    iniciar("Íñigo Peña", "Clave#2026");
    agregarIngreso("Sueldo de diciembre", 3000.50, 0, "01/12/2026");
    registrarGasto("Almuerzo con café", 45.75, 0, "02/12/2026");
    establecerLimite(900);
    guardarNuevaMeta(0, "Viaje a Cochabamba", 1000, 250, 3);
    registro_ahorro(100, "03/12/2026");
    assert(guardarTodo());

    reiniciarTodo();
    assert(total == 0 && ingTotal == 0);
    assert(cargarTodo());
    assert(total == 2);
    assert(proximoId == 3);
    assert(iniciar("Íñigo Peña", "Clave#2026"));
    assert(igual(sumaIngresos(), 3000.50));
    assert(igual(totalGastado(), 45.75));
    assert(gasDesc[0] == "Almuerzo con café");
    assert(igual(limiteSesion(), 900));
    assert(nombreMeta[0] == "Viaje a Cochabamba");
    assert(igual(progreso_ahorro(), 35));
    assert(igual(saldoDisponible(), 3000.50 - 45.75 - 100));
    assert(ingFecha[0] == "01/12/2026");
}

void prueba_carga_sin_archivos()
{
    carpetaVacia("build/prueba_vacia/");
    assert(cargarTodo());
    assert(total == 0);

    carpetaVacia("build/prueba_rota/");
    ofstream u("build/prueba_rota/usuarios.txt");
    u << "1|Ana|1|0" << endl;
    u << "basura sin separadores" << endl;
    u.close();
    ofstream g("build/prueba_rota/gastos.txt");
    g << "1|Pan|10.00|0|02/11/2026" << endl;
    g << "1|Cortado|5.0";  // línea cortada a la mitad
    g.close();
    assert(cargarTodo());
    assert(total == 1);
    assert(nGastos == 1);
}

// ---------- Revisión final ----------
void prueba_total_gastos_centavos()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    for (int i = 0; i < 100; i++) registrarGasto("Cuota", 12345.67f, 0, "01/11/2026");
    assert(fabs(totalGastado() - 1234567.00) < 0.005);
    assert(fabs(totalGastadoCat(0) - 1234567.00) < 0.005);
}

void prueba_huerfanos_no_se_heredan()
{
    carpetaVacia("build/prueba_huerfanos/");
    ofstream u("build/prueba_huerfanos/usuarios.txt");
    u << "1|a|1|0.00" << endl;   // falta la línea de "b" (id 2)
    u.close();
    ofstream i("build/prueba_huerfanos/ingresos.txt");
    i << "2|Plata de b|999.00|0|01/11/2026" << endl;
    i.close();
    assert(cargarTodo());
    assert(registrar("c", "1"));
    assert(iniciar("c", "1"));
    assert(igual(sumaIngresos(), 0));
}

void prueba_redondeo_ahorro()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    agregarIngreso("Sueldo", 100, 0, "01/11/2026");
    guardarNuevaMeta(0, "Chica", 0.30, 0.10, 1);
    assert(registro_ahorro(0.20, "02/11/2026"));
    assert(cambiarMontoMeta(0, 0.30));
}

// ---------- Detalles menores (segunda pasada) ----------
void prueba_meta_sigue_a_su_dueno()
{
    carpetaVacia("build/prueba_metas/");
    registrar("a", "1"); registrar("b", "1"); registrar("c", "1");
    iniciar("c", "1");
    guardarNuevaMeta(2, "Meta de c", 500, 0, 5);
    assert(guardarTodo());
    // se rompe la línea de "b": "c" pasa a la posición 1 al cargar
    ifstream ent("build/prueba_metas/usuarios.txt");
    string l1, l2, l3;
    getline(ent, l1); getline(ent, l2); getline(ent, l3);
    ent.close();
    ofstream sal("build/prueba_metas/usuarios.txt");
    sal << l1 << endl << "linea rota" << endl << l3 << endl;
    sal.close();
    assert(cargarTodo());
    assert(total == 2 && usu[1] == "c");
    assert(tieneMeta[1] && nombreMeta[1] == "Meta de c");
}

void prueba_carga_sin_repetidos()
{
    carpetaVacia("build/prueba_repetidos/");
    ofstream u("build/prueba_repetidos/usuarios.txt");
    u << "1|Ana|1|0.00" << endl << "1|Beto|1|0.00" << endl << "2|Ana|2|0.00" << endl << "3|Caro|1|0.00" << endl;
    u.close();
    assert(cargarTodo());
    assert(total == 2 && usu[0] == "Ana" && usu[1] == "Caro");
}

void prueba_guardado_sin_temporales()
{
    carpetaVacia("build/prueba_tmp/");
    registrar("Ana", "1");
    assert(guardarTodo());
    assert(!std::filesystem::exists("build/prueba_tmp/usuarios.txt.tmp"));
    assert(std::filesystem::exists("build/prueba_tmp/usuarios.txt"));
}

void prueba_modificar_meta_completa()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    guardarNuevaMeta(0, "Viaje", 1000, 500, 5);
    // acumulado 800 con objetivo 600: no vale, y no debe cambiar nada
    assert(!modificarMetaCompleta(0, "Otro", 600, 800, 3));
    assert(nombreMeta[0] == "Viaje" && igual(montoMeta[0], 1000) && mesesMeta[0] == 5);
    assert(modificarMetaCompleta(0, "Viaje largo", 600, 550, 3));
    assert(nombreMeta[0] == "Viaje largo" && igual(montoMeta[0], 600) && igual(ahorroActual[0], 550));
}

void prueba_cuota_y_guardas()
{
    assert(igual(cuotaMensual(1000, 0, 3), 333.34));
    assert(igual(cuotaMensual(100, 100, 3), 0));
    assert(igual(calcularFaltanteAhorro(-1), 0));
    assert(igual(obtenerAhorroMensual(5), 0)); // sin meta: no divide por 0
}

void prueba_tope_de_monto()
{
    registrar("Ana", "1");
    iniciar("Ana", "1");
    assert(!agregarIngreso("Mucho", 2000000000.0, 0, "01/11/2026"));
    assert(!registrarGasto("Mucho", 2000000000.0f, 0, "01/11/2026"));
    assert(!establecerLimite(2000000000.0f));
    assert(agregarIngreso("Justo", MONTO_MAXIMO, 0, "01/11/2026"));
}

void prueba_solo_mi_posicion()
{
    registrar("Ana", "1");
    registrar("Beto", "2");
    iniciar("Ana", "1");
    assert(!modificar(1, "Hack", "x"));
    assert(!borrar(1));
    assert(!guardarNuevaMeta(1, "Ajena", 100, 0, 1));
    assert(guardarNuevaMeta(0, "Mia", 100, 0, 1));
    assert(!cambiarNombreMeta(1, "x"));
}

// ---------- Cifrado de contraseñas ----------
void prueba_cifrado()
{
    string original = "Hola wxyz 123 #!~";
    string cifrado = cifrar(original, LLAVE_CIFRADO);
    assert(cifrado != original);
    assert(cifrado.length() == original.length());
    assert(cifrado.find('|') == string::npos);   // no rompe los campos del .txt
    assert(cifrado.find('\n') == string::npos);
    assert(descifrar(cifrado, LLAVE_CIFRADO) == original);
    assert(descifrar(cifrado, LLAVE_CIFRADO + 1) != original); // con otra llave no se lee
}

void prueba_clave_valida()
{
    assert(claveValida("Clave#2026"));
    assert(!claveValida("clañe"));      // solo letras sin tilde, números y símbolos
    assert(!claveValida("a|b"));
    assert(!claveValida("   "));
    assert(!registrar("Ana", "contraseña"));
    assert(registrar("Ana", "contrasena"));
}

void prueba_archivo_sin_claves_visibles()
{
    carpetaVacia("build/prueba_cifrado/");
    registrar("Ana", "MiClaveSecreta");
    assert(guardarTodo());
    ifstream f("build/prueba_cifrado/usuarios.txt");
    string contenido, linea;
    while (getline(f, linea)) contenido += linea;
    f.close();
    assert(contenido.find("Ana") != string::npos);
    assert(contenido.find("MiClaveSecreta") == string::npos); // la clave no se ve en el archivo
    reiniciarTodo();
    assert(cargarTodo());
    assert(iniciar("Ana", "MiClaveSecreta"));
}

int main()
{
    reiniciarTodo(); prueba_usuarios();
    reiniciarTodo(); prueba_texto_invalido();
    reiniciarTodo(); prueba_ingresos();
    reiniciarTodo(); prueba_ajeno();
    reiniciarTodo(); prueba_gastos();
    reiniciarTodo(); prueba_ahorro();
    reiniciarTodo(); prueba_reportes();
    reiniciarTodo(); prueba_ahorro_sin_saldo();
    reiniciarTodo(); prueba_borrado_en_medio();
    reiniciarTodo(); prueba_archivos_tildes();
    reiniciarTodo(); prueba_carga_sin_archivos();
    reiniciarTodo(); prueba_total_gastos_centavos();
    reiniciarTodo(); prueba_huerfanos_no_se_heredan();
    reiniciarTodo(); prueba_redondeo_ahorro();
    reiniciarTodo(); prueba_meta_sigue_a_su_dueno();
    reiniciarTodo(); prueba_carga_sin_repetidos();
    reiniciarTodo(); prueba_guardado_sin_temporales();
    reiniciarTodo(); prueba_modificar_meta_completa();
    reiniciarTodo(); prueba_cuota_y_guardas();
    reiniciarTodo(); prueba_tope_de_monto();
    reiniciarTodo(); prueba_solo_mi_posicion();
    reiniciarTodo(); prueba_cifrado();
    reiniciarTodo(); prueba_clave_valida();
    reiniciarTodo(); prueba_archivo_sin_claves_visibles();
    cout << "OK" << endl;
    return 0;
}
