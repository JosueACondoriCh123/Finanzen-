// FinanZen — funciones comunes del motor y archivos .txt · integrado con ayuda de Claude Code (IA)
// Archivos (documento del proyecto, sección 8), un registro por línea, campos separados por '|':
//   usuarios.txt  id|nombre|clave cifrada|limite
//   ingresos.txt  usuario|descripcion|monto|categoria|fecha
//   gastos.txt    usuario|descripcion|monto|categoria|fecha
//   metas.txt     usuario|nombre|monto|acumulado|meses
//   ahorros.txt   usuario|monto|fecha
#include "finanzen.h"
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

string carpetaDatos = ""; // vacío = misma carpeta desde donde se ejecuta el programa

// Agregado: Un texto es válido si no está vacío, no es solo espacios y no tiene '|'
// ('|' separa los campos en los archivos .txt)
bool textoValido(string t)
{
    if (t.find('|') != string::npos)
    {
        return false;
    }
    return t.find_first_not_of(" \t\r\n") != string::npos;
}

// Agregado: Borra todo lo que pertenece al usuario en la posición pos (antes de correr usu[])
void borrarDatosDeUsuario(int pos)
{
    int id = usuId[pos];
    borrarIngresosDeUsuario(id);
    borrarGastosDeUsuario(id);
    borrarAhorrosDeUsuario(id);
    eliminarLimiteDeUsuario(pos, total);
    eliminarMetaDeUsuario(pos, total);
}

// Agregado: vacía todos los arreglos (al cargar y en las pruebas)
void reiniciarTodo()
{
    total = 0;
    ingTotal = 0;
    nGastos = 0;
    ahoTotal = 0;
    for (int i = 0; i < MAX; i++)
    {
        reiniciarLimiteUsuario(i);
        reiniciarMetaUsuario(i);
    }
    proximoId = 1;
    sesion = -1;
}

// Agregado: ---------- Guardar ----------
// Se escribe primero en archivos .tmp; solo si los 5 salieron bien se reemplazan los .txt.
// Así un error a mitad de camino (archivo bloqueado, memoria llena) no deja los datos vacíos.

string rutaDatos(string nombre)
{
    return carpetaDatos + nombre;
}

// Agregado: guarda los 5 archivos .txt del documento
bool guardarTodo()
{
    string nombres[5] = {"usuarios.txt", "ingresos.txt", "gastos.txt", "metas.txt", "ahorros.txt"};
    ofstream fu(rutaDatos("usuarios.txt.tmp").c_str());
    ofstream fi(rutaDatos("ingresos.txt.tmp").c_str());
    ofstream fg(rutaDatos("gastos.txt.tmp").c_str());
    ofstream fm(rutaDatos("metas.txt.tmp").c_str());
    ofstream fa(rutaDatos("ahorros.txt.tmp").c_str());
    if (!fu || !fi || !fg || !fm || !fa)
    {
        return false;
    }
    fu << fixed << setprecision(2);
    fi << fixed << setprecision(2);
    fg << fixed << setprecision(2);
    fm << fixed << setprecision(2);
    fa << fixed << setprecision(2);

    for (int i = 0; i < total; i++)
    {
        fu << usuId[i] << "|" << usu[i] << "|" << cifrar(con[i], LLAVE_CIFRADO) << "|" // la contraseña se guarda cifrada
           << "" << limiteGasto[i] << "\n";
        if (tieneMeta[i])
        {
            // Corregido (revisión): la meta se guarda con el id del dueño, no con su posición
            fm << usuId[i] << "|" << nombreMeta[i] << "|" << montoMeta[i] << "|"
               << ahorroActual[i] << "|" << mesesMeta[i] << "\n";
        }
    }
    for (int i = 0; i < ingTotal; i++)
    {
        fi << ingUsuario[i] << "|" << ingDesc[i] << "|" << ingMonto[i] << "|"
           << ingCat[i] << "|" << ingFecha[i] << "\n";
    }
    for (int i = 0; i < nGastos; i++)
    {
        fg << gasUsuario[i] << "|" << gasDesc[i] << "|" << gastos[i] << "|"
           << gasCat[i] << "|" << gasFecha[i] << "\n";
    }
    for (int i = 0; i < ahoTotal; i++)
    {
        fa << ahoUsuario[i] << "|" << ahoMonto[i] << "|" << ahoFecha[i] << "\n";
    }

    fu.close();
    fi.close();
    fg.close();
    fm.close();
    fa.close();
    if (fu.fail() || fi.fail() || fg.fail() || fm.fail() || fa.fail())
    {
        return false; // los .txt anteriores quedan intactos
    }
    for (int i = 0; i < 5; i++)
    {
        remove(rutaDatos(nombres[i]).c_str()); // en Windows rename no pisa un archivo existente
        if (rename(rutaDatos(nombres[i] + ".tmp").c_str(), rutaDatos(nombres[i]).c_str()) != 0)
        {
            return false;
        }
    }
    return true;
}

// ---------- Cargar ----------

// Agregado: Parte una línea por '|'. Devuelve cuántos campos encontró (max + 1 si sobran).
int partirLinea(string linea, string campos[], int max)
{
    if (linea.length() > 0 && linea[linea.length() - 1] == '\r')
    {
        linea = linea.substr(0, linea.length() - 1);
    }
    int n = 0;
    int inicio = 0;
    for (int i = 0; i <= (int)linea.length(); i++)
    {
        if (i == (int)linea.length() || linea[i] == '|')
        {
            if (n >= max)
            {
                return max + 1;
            }
            campos[n] = linea.substr(inicio, i - inicio);
            n++;
            inicio = i + 1;
        }
    }
    return n;
}

// Agregado: Convierte texto a número sin colgarse si viene basura
bool aDouble(string s, double &v)
{
    if (s.length() == 0)
    {
        return false;
    }
    char *fin;
    v = strtod(s.c_str(), &fin);
    return *fin == '\0';
}

// Agregado: texto a entero sin colgarse
bool aEntero(string s, int &v)
{
    double d;
    if (!aDouble(s, d) || d != (int)d)
    {
        return false;
    }
    v = (int)d;
    return true;
}

// Agregado: un registro solo se carga si su dueño existe (así un usuario nuevo nunca hereda datos ajenos)
bool existeUsuarioId(int id)
{
    for (int i = 0; i < total; i++)
    {
        if (usuId[i] == id)
        {
            return true;
        }
    }
    return false;
}

// Agregado: lee los 5 archivos .txt del documento
bool cargarTodo()
{
    reiniciarTodo();
    string linea;
    string c[6];

    ifstream fu((carpetaDatos + "usuarios.txt").c_str());
    while (getline(fu, linea) && total < MAX)
    {
        int id;
        double limite;
        // Corregido (revisión): se saltean ids o nombres repetidos, para que el inicio de sesión no sea ambiguo
        if (partirLinea(linea, c, 6) == 4 && aEntero(c[0], id) && id > 0 && !existeUsuarioId(id) &&
            textoValido(c[1]) && buscarUsuario(c[1]) == -1 && textoValido(c[2]) &&
            aDouble(c[3], limite) && limite >= 0 && limite <= MONTO_MAXIMO)
        {
            usuId[total] = id;
            usu[total] = c[1];
            con[total] = descifrar(c[2], LLAVE_CIFRADO); // se descifra al leer
            limiteGasto[total] = (float)limite;
            if (id >= proximoId)
            {
                proximoId = id + 1;
            }
            total++;
        }
    }

    ifstream fi((carpetaDatos + "ingresos.txt").c_str());
    while (getline(fi, linea) && ingTotal < MAX_ING)
    {
        int u, cat;
        double monto;
        if (partirLinea(linea, c, 6) == 5 && aEntero(c[0], u) && existeUsuarioId(u) && textoValido(c[1]) &&
            aDouble(c[2], monto) && monto > 0 && aEntero(c[3], cat) && cat >= 0 && cat <= 3 &&
            fechaValidaIng(c[4]))
        {
            ingUsuario[ingTotal] = u;
            ingDesc[ingTotal] = c[1];
            ingMonto[ingTotal] = monto;
            ingCat[ingTotal] = cat;
            ingFecha[ingTotal] = c[4];
            ingTotal++;
        }
    }

    ifstream fg((carpetaDatos + "gastos.txt").c_str());
    while (getline(fg, linea) && nGastos < MAX)
    {
        int u, cat;
        double monto;
        if (partirLinea(linea, c, 6) == 5 && aEntero(c[0], u) && existeUsuarioId(u) && textoValido(c[1]) &&
            aDouble(c[2], monto) && monto > 0 && aEntero(c[3], cat) && cat >= 0 &&
            cat < NUM_CAT_GAS && fechaValidaIng(c[4]))
        {
            gasUsuario[nGastos] = u;
            gasDesc[nGastos] = c[1];
            gastos[nGastos] = (float)monto;
            gasCat[nGastos] = cat;
            gasFecha[nGastos] = c[4];
            nGastos++;
        }
    }

    ifstream fm((carpetaDatos + "metas.txt").c_str());
    while (getline(fm, linea))
    {
        int id, meses;
        double monto, actual;
        if (partirLinea(linea, c, 6) == 5 && aEntero(c[0], id) && aDouble(c[2], monto) &&
            aDouble(c[3], actual) && aEntero(c[4], meses) && metaValida(c[1], monto, actual, meses))
        {
            // la meta va a la posición actual de su dueño (las metas de Hector van por posición)
            for (int pos = 0; pos < total; pos++)
            {
                if (usuId[pos] == id)
                {
                    nombreMeta[pos] = c[1];
                    montoMeta[pos] = monto;
                    ahorroActual[pos] = actual;
                    mesesMeta[pos] = meses;
                    tieneMeta[pos] = true;
                }
            }
        }
    }

    ifstream fa((carpetaDatos + "ahorros.txt").c_str());
    while (getline(fa, linea) && ahoTotal < MAX)
    {
        int u;
        double monto;
        if (partirLinea(linea, c, 6) == 3 && aEntero(c[0], u) && existeUsuarioId(u) && aDouble(c[1], monto) &&
            monto > 0 && fechaValidaIng(c[2]))
        {
            ahoUsuario[ahoTotal] = u;
            ahoMonto[ahoTotal] = monto;
            ahoFecha[ahoTotal] = c[2];
            ahoTotal++;
        }
    }

    return true; // si faltan archivos, se empieza vacío
}
