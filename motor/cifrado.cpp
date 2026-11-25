// FinanZen — cifrado de contraseñas · integrado con ayuda de Claude Code (IA)
// Origen: el ejemplo de cifrado del equipo (sumar una llave a cada letra para cifrar,
// restarla para descifrar). Se usa al guardar y leer usuarios.txt, así las
// contraseñas no se pueden leer abriendo el archivo.
//
// Por qué la llave es 100: las contraseñas solo tienen letras sin tilde, números y
// símbolos (códigos 32 a 126). Sumando 100, todas quedan entre 132 y 226, fuera de
// las letras normales. Así el cifrado nunca produce un '|' (que separa los campos
// del archivo) ni un salto de línea.
#include "finanzen.h"

// Origen: BLOQUE 2: CIFRADO del ejemplo del equipo
string cifrar(string mensaje, int llaveCifrado)
{
    string cifrado = mensaje;
    for (int i = 0; i < (int)cifrado.length(); i++)
    {
        cifrado[i] = cifrado[i] + llaveCifrado;
    }
    return cifrado;
}

// Origen: BLOQUE 3: DESCIFRADO del ejemplo del equipo
string descifrar(string cifrado, int llaveDescifrado)
{
    string descifrado = cifrado;
    for (int i = 0; i < (int)descifrado.length(); i++)
    {
        descifrado[i] = descifrado[i] - llaveDescifrado;
    }
    return descifrado;
}

// Agregado: una contraseña válida tiene solo letras sin tilde, números, espacios y símbolos
// (sin '|'), y no puede estar vacía.
bool claveValida(string clave)
{
    if (!textoValido(clave))
    {
        return false;
    }
    for (int i = 0; i < (int)clave.length(); i++)
    {
        unsigned char c = clave[i];
        if (c < 32 || c > 126)
        {
            return false; // tildes, ñ y otros caracteres especiales
        }
    }
    return true;
}
