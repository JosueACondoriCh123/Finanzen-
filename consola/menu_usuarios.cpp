// FinanZen — módulo 1 (Joaquín 1): menús de usuarios · integrado con ayuda de Claude Code (IA)
// Textos tomados de originales/usuarios_joaquin1.cpp; la lógica está en motor/usuarios.cpp.
#include "consola.h"
#include <iostream>

// Menú 1: Registro de usuarios — Origen: registrar()
void registrarConsola()
{
    if (total >= MAX)
    {
        cout << "\nNo hay espacio para mas usuarios";
        return;
    }

    cout << "\n--- REGISTRAR USUARIO ---\n";
    string nombre = leerTexto("Nombre completo: ");
    string clave = leerTexto("Contrasena (letras sin tilde, numeros y simbolos; se guarda cifrada): ");
    while (!claveValida(clave))
    {
        clave = leerTexto("Contrasena invalida. Use letras sin tilde, numeros y simbolos: ");
    }

    if (registrar(nombre, clave))
    {
        guardar();
        cout << "\nUsuario registrado correctamente";
    }
    else
    {
        cout << "\nYa existe un usuario con ese nombre";
    }
}

// Menú 2: Inicio de sesión — Origen: iniciar()
bool iniciarConsola()
{
    if (total == 0)
    {
        cout << "\nNo hay usuarios registrados";
        return false;
    }

    cout << "\n--- INICIAR SESION ---\n";
    string nombre = leerTexto("Nombre completo: ");
    string clave = leerTexto("Contrasena: ");

    if (iniciar(nombre, clave))
    {
        cout << "\nInicio de sesion exitoso";
        cout << "\nBienvenido: " << nombre;
        return true;
    }
    cout << "\nNombre o contrasena incorrectos";
    return false;
}

// Origen: lista() — Joaquín 1 (copia: solo muestra nombres)
void lista()
{
    cout << "\n--- LISTA DE USUARIOS ---";

    if (total == 0)
    {
        cout << "\nNo hay usuarios registrados";
        return;
    }

    for (int i = 0; i < total; i++)
    {
        cout << "\nUsuario " << i + 1;
        cout << "\nNombre: " << usu[i];
        cout << "\n----------------------";
    }
}

// Menú 3: Administración del perfil — Origen: el "SISTEMA DE MODIFICACION" de main()
void menuPerfil()
{
    int z = 9;
    while (z != 0 && sesion != -1)
    {
        cout << "\n===== SISTEMA DE MODIFICACION =====";
        cout << "\nUsuario: " << nombreSesion();
        cout << "\n1. Mod usuario";
        cout << "\n2. Mod meta";
        cout << "\n3. Borrar mi cuenta";
        cout << "\n0. Volver\n";
        z = leerEntero("Seleccione una opcion: ", 0, 3);

        switch (z)
        {
            case 1:
            {
                // Origen: modificar()
                cout << "\n--- MODIFICAR USUARIO ---\n";
                string nombre = leerTexto("Nuevo nombre: ");
                string clave = leerTexto("Nueva contrasena (letras sin tilde, numeros y simbolos): ");
                while (!claveValida(clave))
                {
                    clave = leerTexto("Contrasena invalida. Use letras sin tilde, numeros y simbolos: ");
                }
                if (modificar(indiceSesion(), nombre, clave))
                {
                    guardar();
                    cout << "\nUsuario modificado correctamente";
                }
                else
                {
                    cout << "\nEse nombre ya lo usa otro usuario";
                }
                break;
            }
            case 2:
                // Corregido: en el original este case estaba vacío; ahora abre el menú de Hector
                modificarMetaAhorro(indiceSesion());
                break;
            case 3:
                // Origen: borrar()
                cout << "\n--- BORRAR USUARIO ---";
                if (leerEntero("\nSe borraran tambien sus ingresos, gastos y metas. Confirmar? (1. Si / 0. No): ", 0, 1) == 1)
                {
                    borrar(indiceSesion());
                    guardar();
                    cout << "\nUsuario eliminado correctamente";
                }
                break;
            case 0:
                break;
        }
    }
}
