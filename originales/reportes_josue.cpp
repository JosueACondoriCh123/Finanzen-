#include <iostream>
using namespace std;

void menu5();
void registro_ahorro(float gasto[], int n, float sueldo, float meta);
void progreso_ahorro(float gasto[], int n, float sueldo, float meta);
void resumen_financiero(float gasto[], int n, float sueldo, float meta);
int main()
{
    int opcion5, n;
    float meta, sueldo, gasto[];
    do {
        menu5();
        cin >> opcion5;
        switch (opcion5)
        {
        case 1:
            registro_ahorro(gasto, n, sueldo, meta);
            break;
        case 2:
            progreso_ahorro(gasto, n, sueldo, meta);
            break;
        case 3:
            resumen_financiero(gasto, n, sueldo, meta);
                break;
        case 0: cout << "Saliendo al menú principal";
            break;
        default: cout << "Opción inválida";
            break;
        }
    } while (opcion5 != 0);

    return 0;
}

void menu5()
{
    cout<<"\t Menu \n";
    cout<<"1.- Registro del ahorro\n";
    cout<<"2.- Progreso del ahorro\n";
    cout<<"3.- Resumen financiero\n";
    cout<<"0.- Salir\n";
    cout<<"¿Qué seguimiento quiere hacer?"<<endl;
}
void registro_ahorro(float gasto[], int n, float sueldo, float meta)
{
    float gastototal=0, ahorro;
    for (int i=0; i<n; i++)
        gastototal+=gasto[i];
    ahorro=sueldo-gastototal;
    cout<<"El ahorro total es: "<<ahorro<<endl;
}
void progreso_ahorro(float gasto[], int n, float sueldo, float meta)
{
    float progreso, ahorro, gastototal=0;
    for (int i=0; i<n; i++)
        gastototal+=gasto[i];
    ahorro=sueldo-gastototal;
    progreso=(ahorro/meta)*100;
    cout<<"Se ha ahorrado en total un "<<progreso<<"% de la meta propuesta"<<endl;
    if (progreso>=100)
        cout<<"Es posible cumplir la meta de ahorro"<<endl;
    else
        cout<<"No es posible cumplir la meta de ahorro"<<endl; 
}
void resumen_financiero(float gasto[], int n, float sueldo, float meta)
{
    float saldos, saldom, gastototal;
    for (int i=0; i<n; i++)
        gastototal+=gasto[i];
    saldom=meta-gastototal;
    saldos=sueldo-gastototal;
    cout<<"El saldo de la cuenta es: "<<saldos<<endl;
    cout<<"El saldo para alcanzar la meta es: "<<saldom<<endl;
}