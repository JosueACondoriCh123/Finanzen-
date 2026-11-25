// FinanZen — versión consola: menú de inicio y menú principal · integrado con ayuda de Claude Code (IA)
// Une los 5 módulos del equipo. Estructura tomada del main() de Joaquín 1.
#include "consola.h"
#include <iostream>

void menuPrincipal()
{
    int op = 9;
    while (op != 0 && sesion != -1)
    {
        cout << "\n\n===== FINANZEN - MENU PRINCIPAL =====";
        cout << "\nUsuario: " << nombreSesion();
        cout << "\n1. Usuarios y perfil";
        cout << "\n2. Gestion de ingresos";
        cout << "\n3. Gestion de gastos";
        cout << "\n4. Planificacion del ahorro";
        cout << "\n5. Seguimiento y reportes";
        cout << "\n0. Cerrar sesion\n";
        op = leerEntero("Seleccione una opcion: ", 0, 5);

        switch (op)
        {
            case 1: menuPerfil(); break;
            case 2: menuIngresos(); break;
            case 3: menuGastos(); break;
            case 4: menuPlanificacionAhorro(indiceSesion()); break;
            case 5: menuReportes(); break;
            case 0:
                cerrarSesion();
                cout << "\nSesion cerrada";
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        carpetaDatos = argv[1]; // opcional: carpeta donde están los .txt (terminada en /)
    }
    cargarTodo();

    int x = 9;
    while (x != 0)
    {
        cout << "\n\n===== FINANZEN - SISTEMA DE LOGIN =====";
        cout << "\n1. Registrar usuario";
        cout << "\n2. Iniciar sesion";
        cout << "\n3. Listar usuarios";
        cout << "\n0. Salir\n";
        x = leerEntero("Seleccione una opcion: ", 0, 3);

        switch (x)
        {
            case 1:
                registrarConsola();
                break;
            case 2:
                if (iniciarConsola())
                {
                    menuPrincipal();
                }
                break;
            case 3:
                lista();
                break;
            case 0:
                cout << "\nSaliendo del sistema...\n";
                break;
        }
    }
    return 0;
}
