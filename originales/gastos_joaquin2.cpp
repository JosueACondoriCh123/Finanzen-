#include <iostream>
using namespace std;

// PROTOTIPOS
void registrarGastos(float gastos[], int &n, float &limite);
void consultarGastos(float gastos[], int n);
void administrarGastos(float gastos[], int &n);

int main() {

    float gastos[100];
    float limite = 0;
    int n = 0;
    int opcion;

    do {

        cout << "\n===== GESTION DE GASTOS =====" << endl;
        cout << "1. Registrar gastos" << endl;
        cout << "2. Consultar gastos" << endl;
        cout << "3. Administrar gastos" << endl;
        cout << "0. Salir" << endl;

        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {

            case 1:
                registrarGastos(gastos, n, limite);
                break;

            case 2:
                consultarGastos(gastos, n);
                break;

            case 3:
                administrarGastos(gastos, n);
                break;

            case 0:
                cout << "Saliendo al menú principal" << endl;
                break;

            default:
                cout << "Opcion incorrecta." << endl;
        }

    } while (opcion != 4);

    return 0;
}


// FUNCION 1: REGISTRAR GASTOS
void registrarGastos(float gastos[], int &n, float &limite) {

    int opcion;
    int cantidad;

    cout << "\n--- REGISTRAR GASTOS ---" << endl;
    cout << "1. Registrar un gasto" << endl;
    cout << "2. Registrar varios gastos" << endl;
    cout << "3. Establecer limite de gasto" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    if (opcion == 1) {

        if (n < 100) {
            cout << "Ingrese el monto: ";
            cin >> gastos[n];
            n++;
        }
        else {
            cout << "No hay espacio disponible." << endl;
        }

    }

    else if (opcion == 2) {

        cout << "Cuantos gastos desea registrar: ";
        cin >> cantidad;

        for (int i = 0; i < cantidad && n < 100; i++) {

            cout << "Ingrese el monto: ";
            cin >> gastos[n];
            n++;
        }

    }

    else if (opcion == 3) {

        cout << "Ingrese su limite de gasto: ";
        cin >> limite;

    }

    else {
        cout << "Opcion incorrecta." << endl;
    }

}


// FUNCION 2: CONSULTAR GASTOS
void consultarGastos(float gastos[], int n) {

    int opcion;
    float total = 0;
    float mayor, menor;

    cout << "\n--- CONSULTAR GASTOS ---" << endl;
    cout << "1. Mostrar todos los gastos" << endl;
    cout << "2. Mostrar total gastado" << endl;
    cout << "3. Mostrar gasto mayor" << endl;
    cout << "4. Mostrar gasto menor" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    if (n == 0) {
        cout << "No hay gastos registrados." << endl;
        return;
    }

    mayor = gastos[0];
    menor = gastos[0];

    for (int i = 0; i < n; i++) {

        total += gastos[i];

        if (gastos[i] > mayor) {
            mayor = gastos[i];
        }

        if (gastos[i] < menor) {
            menor = gastos[i];
        }

    }

    if (opcion == 1) {

        for (int i = 0; i < n; i++) {
            cout << "Gasto " << i + 1 << ": "
                 << gastos[i] << " Bs" << endl;
        }

    }

    else if (opcion == 2) {
        cout << "Total gastado: " << total << " Bs" << endl;
    }

    else if (opcion == 3) {
        cout << "Gasto mayor: " << mayor << " Bs" << endl;
    }

    else if (opcion == 4) {
        cout << "Gasto menor: " << menor << " Bs" << endl;
    }

    else {
        cout << "Opcion incorrecta." << endl;
    }

}


// FUNCION 3: ADMINISTRAR GASTOS
void administrarGastos(float gastos[], int &n) {

    int opcion;
    int posicion;

    if (n == 0) {
        cout << "No hay gastos registrados." << endl;
        return;
    }

    cout << "\n--- ADMINISTRAR GASTOS ---" << endl;
    cout << "1. Modificar gasto" << endl;
    cout << "2. Eliminar gasto" << endl;
    cout << "3. Buscar gasto" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    cout << "Ingrese el numero del gasto: ";
    cin >> posicion;

    posicion--;

    if (posicion < 0 || posicion >= n) {
        cout << "Gasto no valido." << endl;
        return;
    }

    if (opcion == 1) {

        cout << "Ingrese el nuevo monto: ";
        cin >> gastos[posicion];

        cout << "Gasto modificado correctamente." << endl;

    }

    else if (opcion == 2) {

        for (int i = posicion; i < n - 1; i++) {
            gastos[i] = gastos[i + 1];
        }

        n--;

        cout << "Gasto eliminado correctamente." << endl;

    }

    else if (opcion == 3) {

        cout << "Gasto encontrado: "
             << gastos[posicion] << " Bs" << endl;

    }

    else {
        cout << "Opcion incorrecta." << endl;
    }

}