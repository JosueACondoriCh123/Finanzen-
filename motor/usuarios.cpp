// FinanZen — módulo 1 (Joaquín 1): usuarios y autenticación · integrado con ayuda de Claude Code (IA)
// Lógica tomada de originales/usuarios_joaquin1.cpp. Los cin/cout originales
// quedaron en consola/menu_usuarios.cpp; aquí solo queda la lógica.
#include "finanzen.h"

// Arreglos globales (originales)
string usu[MAX];
string con[MAX];
int total = 0;

// Agregados para unir los módulos
int usuId[MAX];
int proximoId = 1;
int sesion = -1;

// Agregado: Busca un nombre en usu[]; devuelve la posición o -1 (apoyo para no repetir nombres)
int buscarUsuario(string nombre)
{
    for (int i = 0; i < total; i++)
    {
        if (usu[i] == nombre)
        {
            return i;
        }
    }
    return -1;
}

// Origen: registrar() — Joaquín 1
bool registrar(string nombre, string clave)
{
    if (total >= MAX)
    {
        return false; // "No hay espacio para mas usuarios"
    }
    if (!textoValido(nombre) || !claveValida(clave))
    {
        return false;
    }
    if (buscarUsuario(nombre) != -1)
    {
        return false; // Corregido: el original permitía dos usuarios con el mismo nombre
    }

    usu[total] = nombre;
    con[total] = clave;
    usuId[total] = proximoId;
    proximoId++;

    reiniciarMetaUsuario(total);   // módulo de Hector: la posición empieza sin meta
    reiniciarLimiteUsuario(total); // módulo de Joaquín 2: sin límite de gasto

    total++;
    return true; // "Usuario registrado correctamente"
}

// Origen: iniciar() — Joaquín 1
bool iniciar(string nombre, string clave)
{
    bool encontrado = false;

    for (int i = 0; i < total; i++)
    {
        if (usu[i] == nombre && con[i] == clave)
        {
            encontrado = true;
            sesion = usuId[i];
            break;
        }
    }

    return encontrado;
}

// Agregado: cierra la sesión actual
void cerrarSesion()
{
    sesion = -1;
}

// Agregado: Posición en usu[] del usuario conectado (-1 si no hay sesión)
int indiceSesion()
{
    for (int i = 0; i < total; i++)
    {
        if (usuId[i] == sesion)
        {
            return i;
        }
    }
    return -1;
}

// Agregado: nombre del usuario conectado
string nombreSesion()
{
    int p = indiceSesion();
    if (p == -1)
    {
        return "";
    }
    return usu[p];
}

// Origen: modificar() — Joaquín 1 (pos empieza en 0; el original usaba p - 1)
bool modificar(int pos, string nombre, string clave)
{
    if (pos < 0 || pos >= total || pos != indiceSesion()) // agregado: solo la propia cuenta
    {
        return false; // "Usuario no encontrado"
    }
    if (!textoValido(nombre) || !claveValida(clave))
    {
        return false;
    }
    int otro = buscarUsuario(nombre);
    if (otro != -1 && otro != pos)
    {
        return false;
    }

    usu[pos] = nombre;
    con[pos] = clave;
    return true; // "Usuario modificado correctamente"
}

// Origen: borrar() — Joaquín 1
bool borrar(int pos)
{
    if (pos < 0 || pos >= total || pos != indiceSesion()) // agregado: solo la propia cuenta
    {
        return false; // "Usuario no encontrado"
    }

    int id = usuId[pos];
    borrarDatosDeUsuario(pos); // agregado: borra sus ingresos, gastos, meta y ahorros

    for (int i = pos; i < total - 1; i++)
    {
        usu[i] = usu[i + 1];
        con[i] = con[i + 1];
        usuId[i] = usuId[i + 1];
    }

    total--;

    if (sesion == id)
    {
        sesion = -1;
    }
    return true; // "Usuario eliminado correctamente"
}
