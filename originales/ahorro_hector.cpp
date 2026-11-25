// MODULO 4: PLANIFICACION DEL AHORRO
// Este archivo no tiene main. El programa principal llama a
// menuPlanificacionAhorro(indiceUsuario) despues de iniciar sesion.
// El indice debe estar entre 0 y 99 y coincidir con el arreglo de usuarios.

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

// Datos privados del modulo: una meta por posicion de usuario.
static const int MAX_METAS_AHORRO = 100;
static string nombreMeta[MAX_METAS_AHORRO];
static double montoMeta[MAX_METAS_AHORRO] = {};
static double ahorroActual[MAX_METAS_AHORRO] = {};
static int mesesMeta[MAX_METAS_AHORRO] = {};
static bool tieneMeta[MAX_METAS_AHORRO] = {};

static int leerEnteroAhorro(const string& mensaje, int minimo, int maximo)
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

static double leerMontoAhorro(const string& mensaje, double minimo, double maximo)
{
    double valor;
    while (true)
    {
        cout << mensaje;
        if (cin >> valor && isfinite(valor) && valor >= minimo && valor <= maximo &&
            fabs(valor * 100.0 - round(valor * 100.0)) < 0.00001 &&
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

static string leerTextoAhorro(const string& mensaje)
{
    string texto;
    while (true)
    {
        cout << mensaje;
        if (!getline(cin, texto)) exit(0);
        if (texto.find_first_not_of(" \t\r") != string::npos) return texto;
        cout << "El texto no puede estar vacio.\n";
    }
}

static bool existeMetaAhorro(int usuario)
{
    if (tieneMeta[usuario]) return true;
    cout << "Primero debe crear una meta.\n";
    return false;
}

static double calcularFaltanteAhorro(int usuario)
{
    return montoMeta[usuario] - ahorroActual[usuario];
}

static double obtenerAhorroMensual(int usuario)
{
    // Los calculos en centavos evitan errores de redondeo de punto flotante.
    long long faltante = llround(montoMeta[usuario] * 100.0) - llround(ahorroActual[usuario] * 100.0);
    return ((faltante + mesesMeta[usuario] - 1) / mesesMeta[usuario]) / 100.0;
}

// Pide los datos antes de guardar; sirve para crear o reemplazar una meta.
static void guardarNuevaMetaAhorro(int usuario)
{
    string nombre = leerTextoAhorro("Nombre de la meta: ");
    double monto = leerMontoAhorro("Monto objetivo: ", 0.01, 1000000000.0);
    double actual = leerMontoAhorro("Ahorro acumulado hasta ahora: ", 0.0, monto);
    int meses = leerEnteroAhorro("Meses restantes para alcanzar la meta: ", 1, 1200);
    cout << "\nResumen: " << nombre << ", objetivo " << monto
         << ", acumulado " << actual << ", plazo " << meses << " meses.\n";
    if (leerEnteroAhorro("Guardar meta? (1. Si / 0. No): ", 0, 1) == 0)
    {
        cout << "Operacion cancelada.\n";
        return;
    }
    nombreMeta[usuario] = nombre;
    montoMeta[usuario] = monto;
    ahorroActual[usuario] = actual;
    mesesMeta[usuario] = meses;
    tieneMeta[usuario] = true;
    cout << "Meta guardada correctamente.\n";
}

static void crearMetaAhorro(int usuario)
{
    if (tieneMeta[usuario])
    {
        cout << "Ya tiene una meta. Use la opcion de modificar o reemplazar.\n";
        return;
    }
    cout << "\n--- CREAR META DE AHORRO ---\n";
    guardarNuevaMetaAhorro(usuario);
}

static void mostrarMetaAhorro(int usuario)
{
    if (!existeMetaAhorro(usuario)) return;
    ios::fmtflags formato = cout.flags();
    streamsize precision = cout.precision();
    cout << fixed << setprecision(2)
         << "\n--- DETALLE DE LA META ---\n"
         << "Nombre: " << nombreMeta[usuario] << '\n'
         << "Objetivo: " << montoMeta[usuario] << '\n'
         << "Ahorro acumulado: " << ahorroActual[usuario] << '\n'
         << "Faltante: " << calcularFaltanteAhorro(usuario) << '\n'
         << "Progreso: " << ahorroActual[usuario] / montoMeta[usuario] * 100.0 << "%\n"
         << "Meses restantes: " << mesesMeta[usuario] << '\n';
    cout.flags(formato);
    cout.precision(precision);
}

static void menuCrearMetaAhorro(int usuario)
{
    int opcion;
    do
    {
        cout << "\n--- MENU DE CREACION DE META ---\n"
             << "1. Registrar una meta\n2. Consultar meta actual\n0. Volver\n";
        opcion = leerEnteroAhorro("Seleccione una opcion: ", 0, 2);
        if (opcion == 1) crearMetaAhorro(usuario);
        if (opcion == 2) mostrarMetaAhorro(usuario);
    } while (opcion != 0);
}

static void calcularAhorroMensual(int usuario)
{
    if (!existeMetaAhorro(usuario)) return;
    double faltante = calcularFaltanteAhorro(usuario);
    double mensual = obtenerAhorroMensual(usuario);
    cout << "\n--- AHORRO MENSUAL RECOMENDADO ---\n";
    // Se guardan y restauran las opciones de formato para no afectar a otros modulos.
    ios::fmtflags formatoAnterior = cout.flags();
    streamsize precisionAnterior = cout.precision();
    cout << fixed << setprecision(2)
         << "Meta: " << nombreMeta[usuario] << '\n'
         << "Monto objetivo: " << montoMeta[usuario] << '\n'
         << "Ahorro acumulado: " << ahorroActual[usuario] << '\n'
         << "Monto faltante: " << faltante << '\n'
         << "Meses restantes: " << mesesMeta[usuario] << '\n'
         << "Ahorro mensual recomendado: " << mensual << '\n';
    cout.flags(formatoAnterior);
    cout.precision(precisionAnterior);
    if (faltante == 0) cout << "La meta ya esta alcanzada.\n";
}

static void mostrarProyeccionAhorro(int usuario)
{
    if (!existeMetaAhorro(usuario)) return;
    long long acumulado = llround(ahorroActual[usuario] * 100.0);
    long long objetivo = llround(montoMeta[usuario] * 100.0);
    if (calcularFaltanteAhorro(usuario) <= 0)
    {
        cout << "La meta ya esta alcanzada.\n";
        return;
    }
    long long mensual = llround(obtenerAhorroMensual(usuario) * 100.0);
    ios::fmtflags formato = cout.flags();
    streamsize precision = cout.precision();
    cout << fixed << setprecision(2) << "\nMes | Aporte | Acumulado\n";
    for (int mes = 1; mes <= mesesMeta[usuario] && acumulado < objetivo; mes++)
    {
        long long aporte = mensual;
        if (aporte > objetivo - acumulado) aporte = objetivo - acumulado;
        acumulado += aporte;
        cout << mes << " | " << aporte / 100.0 << " | " << acumulado / 100.0 << '\n';
    }
    cout.flags(formato);
    cout.precision(precision);
    cout << "Proyeccion estimada sin intereses ni cambios en los aportes.\n";
}

static void simularAporteAhorro(int usuario)
{
    if (!existeMetaAhorro(usuario)) return;
    double faltante = calcularFaltanteAhorro(usuario);
    if (faltante <= 0) { cout << "La meta ya esta alcanzada.\n"; return; }
    double aporte = leerMontoAhorro("Cuanto podria ahorrar al mes?: ", 0.01, 1000000000.0);
    long long faltanteCentavos = llround(montoMeta[usuario] * 100.0) - llround(ahorroActual[usuario] * 100.0);
    long long aporteCentavos = llround(aporte * 100.0);
    long long mesesNecesarios = (faltanteCentavos + aporteCentavos - 1) / aporteCentavos;
    cout << "Meses estimados para alcanzar la meta: " << mesesNecesarios << '\n';
    if (mesesNecesarios <= mesesMeta[usuario]) cout << "Este aporte permite cumplir el plazo.\n";
    else cout << "Este aporte no alcanza para cumplir el plazo actual.\n";
    cout << "La simulacion no modifica la meta.\n";
}

static void menuCalculoAhorro(int usuario)
{
    if (!existeMetaAhorro(usuario)) return;
    int opcion;
    do
    {
        cout << "\n--- MENU DE CALCULO DE AHORRO ---\n"
             << "1. Calcular ahorro mensual recomendado\n"
             << "2. Ver proyeccion mensual\n"
             << "3. Simular otro aporte mensual\n"
             << "0. Volver\n";
        opcion = leerEnteroAhorro("Seleccione una opcion: ", 0, 3);
        if (opcion == 1) calcularAhorroMensual(usuario);
        if (opcion == 2) mostrarProyeccionAhorro(usuario);
        if (opcion == 3) simularAporteAhorro(usuario);
    } while (opcion != 0);
}

static void cambiarNombreMeta(int usuario)
{
    nombreMeta[usuario] = leerTextoAhorro("Nuevo nombre: ");
    cout << "Nombre actualizado.\n";
}

static void cambiarMontoMeta(int usuario)
{
    montoMeta[usuario] = leerMontoAhorro("Nuevo monto objetivo (no menor al ahorro acumulado): ",
        ahorroActual[usuario] > 0.01 ? ahorroActual[usuario] : 0.01, 1000000000.0);
    cout << "Monto actualizado.\n";
}

static void actualizarAhorroAcumulado(int usuario)
{
    ahorroActual[usuario] = leerMontoAhorro("Ahorro acumulado: ", 0.0, montoMeta[usuario]);
    cout << "Ahorro acumulado actualizado.\n";
}

static void cambiarPlazoMeta(int usuario)
{
    mesesMeta[usuario] = leerEnteroAhorro("Meses restantes: ", 1, 1200);
    cout << "Plazo actualizado.\n";
}

static void reemplazarMetaAhorro(int usuario)
{
    cout << "\n--- REEMPLAZAR META ---\n";
    cout << "La meta actual solo se reemplazara si confirma los nuevos datos.\n";
    guardarNuevaMetaAhorro(usuario);
}

static void modificarMetaAhorro(int usuario)
{
    if (!existeMetaAhorro(usuario)) return;
    int opcion;
    do
    {
        cout << "\n--- MODIFICAR O REEMPLAZAR META ---\n"
             << "Meta actual: " << nombreMeta[usuario] << '\n'
             << "1. Cambiar nombre\n2. Cambiar monto objetivo\n"
             << "3. Actualizar ahorro acumulado\n4. Cambiar meses restantes\n"
             << "5. Reemplazar meta completa\n6. Ver detalle de la meta\n0. Volver\n";
        opcion = leerEnteroAhorro("Seleccione una opcion: ", 0, 6);
        switch (opcion)
        {
            case 1: cambiarNombreMeta(usuario); break;
            case 2: cambiarMontoMeta(usuario); break;
            case 3: actualizarAhorroAcumulado(usuario); break;
            case 4: cambiarPlazoMeta(usuario); break;
            case 5: reemplazarMetaAhorro(usuario); break;
            case 6: mostrarMetaAhorro(usuario); break;
        }
    } while (opcion != 0);
}

// Llamar despues de iniciar sesion, usando la posicion del usuario (empieza en 0).
void menuPlanificacionAhorro(int usuario)
{
    if (usuario < 0 || usuario >= MAX_METAS_AHORRO)
    {
        cout << "Indice de usuario invalido.\n";
        return;
    }
    int opcion;
    do
    {
        cout << "\n===== PLANIFICACION DEL AHORRO =====\n"
             << "1. Crear meta de ahorro\n"
             << "2. Calcular ahorro mensual recomendado\n"
             << "3. Modificar o reemplazar meta\n"
             << "0. Volver\n";
        opcion = leerEnteroAhorro("Seleccione una opcion: ", 0, 3);
        switch (opcion)
        {
            case 1: menuCrearMetaAhorro(usuario); break;
            case 2: menuCalculoAhorro(usuario); break;
            case 3: modificarMetaAhorro(usuario); break;
        }
    } while (opcion != 0);
}

// Llamar al registrar un usuario en una posicion nueva o reutilizada.
void reiniciarMetaUsuario(int usuario)
{
    if (usuario < 0 || usuario >= MAX_METAS_AHORRO) return;
    nombreMeta[usuario].clear();
    montoMeta[usuario] = 0;
    ahorroActual[usuario] = 0;
    mesesMeta[usuario] = 0;
    tieneMeta[usuario] = false;
}

// Si borrar() desplaza los usuarios, llamar con el indice eliminado y el total
// ANTES de reducirlo. Asi las metas se desplazan junto con sus usuarios.
void eliminarMetaDeUsuario(int usuario, int totalAntes)
{
    if (totalAntes < 1 || totalAntes > MAX_METAS_AHORRO || usuario < 0 || usuario >= totalAntes) return;
    for (int i = usuario; i < totalAntes - 1; i++)
    {
        nombreMeta[i] = nombreMeta[i + 1];
        montoMeta[i] = montoMeta[i + 1];
        ahorroActual[i] = ahorroActual[i + 1];
        mesesMeta[i] = mesesMeta[i + 1];
        tieneMeta[i] = tieneMeta[i + 1];
    }
    reiniciarMetaUsuario(totalAntes - 1);
}
