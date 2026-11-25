#include <iostream>
#include <string>
#include <limits>
#define MAX 100
using namespace std;

// Arreglos globales
string usu[MAX];
string con[MAX];
int total = 0;

// Declaracion de funciones
void registrar();
void lista();
void modificar();
void borrar();
void iniciar();

int main()
{
    int x, y = 1;

    while (y != 0)
    {
        int z=9;
        cout << "\n===== SISTEMA DE LOGIN =====";
        cout << "\n1. Registrar usuario";
        cout << "\n2. Listar usuarios";
        cout << "\n3. Modificar usuario";
        cout << "\n4. Borrar usuario";
        cout << "\n5. Iniciar sesion";
        cout << "\n0. Salir";
        cout << "\nSeleccione una opcion: ";
        cin >> x;

        switch (x)
        {
            case 1:
                registrar();
                break;

            case 2:
                lista();
                break;

            case 3:
                while (z!=0){    
                    cout << "\n===== SISTEMA DE MODIFICACION =====";
                    cout << "\n1. Mod usuario";
                    cout << "\n2. Mod meta";
                    cout << "\n0. Cancelar";
                    cin >>z;
                
                    switch (z){
                        case 1:    
                            modificar();
                            break;
                        case 2:
                        
                            break;
                        case 0:
                        
                            break;
                        default:
                            cout<<"opcion invalida";
                            break;
                    }
                }
                break;

            case 4:
                borrar();
                break;

            case 5:
                iniciar();
                break;

            case 0:
                y = 0;
                cout << "\nSaliendo del sistema...";
                break;

            default:
                cout << "\nOpcion invalida";
                break;
        }
    }

    return 0;
}

// REGISTRAR USUARIO
void registrar()
{
    if (total >= MAX)
    {
        cout << "\nNo hay espacio para mas usuarios";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n--- REGISTRAR USUARIO ---";

    cout << "\nNombre completo: ";
    getline(cin, usu[total]);

    cout << "Contrasena: ";
    getline(cin, con[total]);

    total++;

    cout << "\nUsuario registrado correctamente";
}

// LISTAR USUARIOS
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

// MODIFICAR USUARIO
void modificar()
{
    int p;

    if (total == 0)
    {
        cout << "\nNo hay usuarios registrados";
        return;
    }

    cout << "\n--- MODIFICAR USUARIO ---";

    for (int i = 0; i < total; i++)
    {
        cout << "\n" << i + 1 << ". " << usu[i];
    }

    cout << "\nSeleccione el usuario que desea modificar: ";
    cin >> p;

    if (p < 1 || p > total)
    {
        cout << "\nUsuario no encontrado";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nNuevo nombre: ";
    getline(cin, usu[p - 1]);

    cout << "Nueva contrasena: ";
    getline(cin, con[p - 1]);

    cout << "\nUsuario modificado correctamente";
}

// BORRAR USUARIO
void borrar()
{
    int p;

    if (total == 0)
    {
        cout << "\nNo hay usuarios registrados";
        return;
    }

    cout << "\n--- BORRAR USUARIO ---";

    for (int i = 0; i < total; i++)
    {
        cout << "\n" << i + 1 << ". " << usu[i];
    }

    cout << "\nSeleccione el usuario que desea borrar: ";
    cin >> p;

    if (p < 1 || p > total)
    {
        cout << "\nUsuario no encontrado";
        return;
    }

    for (int i = p - 1; i < total - 1; i++)
    {
        usu[i] = usu[i + 1];
        con[i] = con[i + 1];
    }

    total--;

    cout << "\nUsuario eliminado correctamente";
}

// INICIAR SESION
void iniciar()
{
    string nombre, clave;
    bool encontrado = false;

    if (total == 0)
    {
        cout << "\nNo hay usuarios registrados";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n--- INICIAR SESION ---";

    cout << "\nNombre completo: ";
    getline(cin, nombre);

    cout << "Contrasena: ";
    getline(cin, clave);

    for (int i = 0; i < total; i++)
    {
        if (usu[i] == nombre && con[i] == clave)
        {
            encontrado = true;
            break;
        }
    }

    if (encontrado == true)
    {
        cout << "\nInicio de sesion exitoso";
        cout << "\nBienvenido: " << nombre;
    }
    else
    {
        cout << "\nNombre o contrasena incorrectos";
    }
}