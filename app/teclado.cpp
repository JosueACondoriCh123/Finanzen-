// FinanZen — teclado propio de la app · integrado con ayuda de Claude Code (IA)
// raylib no abre el teclado de Android, así que la app dibuja el suyo:
// QWERTY para textos y numérico para montos y fechas. En la PC también se puede usar el teclado físico.
#include "app.h"

int campoActivo = -1;
int tipoActivo = TECLADO_TEXTO;
string pendiente = "";      // letras tocadas que el campo activo todavía no agregó
int borrarPendientes = 0;   // cuántas veces tocaron "borrar"
bool mayuscula = false;
bool tilde = false;
float inicioTeclado = 560;  // el teclado ocupa de aquí hasta abajo

bool tecladoAbierto()
{
    return campoActivo != -1;
}

void cerrarTeclado()
{
    campoActivo = -1;
    pendiente = "";
    borrarPendientes = 0;
    mayuscula = false;
    tilde = false;
}

// Quita la última letra (en UTF-8 una letra con tilde ocupa 2 bytes)
void borrarUltimaLetra(string &s)
{
    if (s.length() == 0)
    {
        return;
    }
    int i = (int)s.length() - 1;
    while (i > 0 && (s[i] & 0xC0) == 0x80)
    {
        i--;
    }
    s = s.substr(0, i);
}

int cantidadLetras(string s)
{
    int n = 0;
    for (int i = 0; i < (int)s.length(); i++)
    {
        if ((s[i] & 0xC0) != 0x80) n++;
    }
    return n;
}

// Agrega una letra respetando las reglas de cada tipo de campo
void agregarLetra(string &valor, string letra, int tipo)
{
    if (tipo == TECLADO_NUMERO)
    {
        if (letra == ".") letra = ",";
        if (letra == ",")
        {
            if (valor.find(',') != string::npos || valor.length() == 0) return;
        }
        else if (letra < "0" || letra > "9" || letra.length() != 1)
        {
            return;
        }
        size_t coma = valor.find(',');
        if (coma != string::npos && valor.length() - coma > 2) return; // máximo 2 decimales
        if (valor.length() >= 12) return;
        valor += letra;
    }
    else if (tipo == TECLADO_ENTERO)
    {
        // Corregido (revisión): los meses usaban el teclado de fecha y "12" se volvía "12/"
        if (letra.length() != 1 || letra < "0" || letra > "9" || valor.length() >= 4) return;
        valor += letra;
    }
    else if (tipo == TECLADO_FECHA)
    {
        if (letra.length() != 1 || letra < "0" || letra > "9" || valor.length() >= 10) return;
        valor += letra;
        if (valor.length() == 2 || valor.length() == 5) valor += "/";
    }
    else
    {
        if (letra == "|" || cantidadLetras(valor) >= 40) return; // '|' separa campos en los .txt
        if (tipo == TECLADO_CLAVE && (unsigned char)letra[0] >= 128) return; // la clave se cifra: sin ñ ni tildes
        valor += letra;
    }
}

// Dibuja un campo. Si es el activo, recibe las letras del teclado.
bool campoTexto(Rectangle r, string etiqueta, string &valor, int tipo, int id)
{
    bool activo = (campoActivo == id);
    texto(etiqueta, r.x + 4, r.y - 24, 16, TEXTO2);
    DrawRectangleRounded(r, 0.3f, 8, TARJETA);
    if (activo)
    {
        DrawRectangleRoundedLinesEx(r, 0.3f, 8, 2, COLOR_AHO);
        for (int i = 0; i < borrarPendientes; i++)
        {
            borrarUltimaLetra(valor);
            if (tipo == TECLADO_FECHA && (valor.length() == 2 || valor.length() == 5))
            {
                borrarUltimaLetra(valor); // saca también la barra
            }
        }
        borrarPendientes = 0;
        // agrega letra por letra lo pendiente
        int i = 0;
        while (i < (int)pendiente.length())
        {
            int largo = 1;
            unsigned char c = pendiente[i];
            if (c >= 0xC0) largo = 2;
            agregarLetra(valor, pendiente.substr(i, largo), tipo);
            i += largo;
        }
        pendiente = "";
    }
    string mostrar = valor;
    if (tipo == TECLADO_CLAVE)
    {
        mostrar = string(cantidadLetras(valor), '*');
    }
    if (tipo == TECLADO_NUMERO && valor != "")
    {
        mostrar = "Bs " + valor;
    }
    texto(mostrar, r.x + 16, r.y + (r.height - 22) / 2, 22, TEXTO);
    if (activo && ((int)(GetTime() * 2)) % 2 == 0)
    {
        float x = r.x + 18 + anchoTexto(mostrar, 22, false);
        DrawRectangle((int)x, (int)(r.y + 12), 2, (int)(r.height - 24), COLOR_AHO);
    }
    if (tocado(r))
    {
        campoActivo = id;
        tipoActivo = tipo;
        pendiente = "";
        borrarPendientes = 0;
        mayuscula = (valor.length() == 0 && tipo == TECLADO_TEXTO);
        return true;
    }
    return false;
}

// Una tecla del teclado dibujado
bool tecla(Rectangle r, string etiqueta, Color fondo, Color letra)
{
    bool encima = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), r);
    DrawRectangleRounded(r, 0.25f, 6, encima ? ColorBrightness(fondo, 0.25f) : fondo);
    float t = etiqueta.length() > 3 ? 17 : 24;
    texto(etiqueta, r.x + (r.width - anchoTexto(etiqueta, t, false)) / 2, r.y + (r.height - t) / 2 - 1, t, letra);
    return tocado(r);
}

string conTilde(string vocal)
{
    if (vocal == "a") return "á";
    if (vocal == "e") return "é";
    if (vocal == "i") return "í";
    if (vocal == "o") return "ó";
    if (vocal == "u") return "ú";
    if (vocal == "A") return "Á";
    if (vocal == "E") return "É";
    if (vocal == "I") return "Í";
    if (vocal == "O") return "Ó";
    if (vocal == "U") return "Ú";
    return vocal;
}

string aMayuscula(string letra)
{
    if (letra == "ñ") return "Ñ";
    if (letra.length() == 1 && letra[0] >= 'a' && letra[0] <= 'z')
    {
        letra[0] = (char)(letra[0] - 'a' + 'A');
    }
    return letra;
}

void tocarLetra(string letra)
{
    if (mayuscula)
    {
        letra = aMayuscula(letra);
        mayuscula = false;
    }
    if (tilde)
    {
        letra = conTilde(letra);
        tilde = false;
    }
    pendiente += letra;
}

bool dibujarTecladoTexto()
{
    float y = ALTO - 4 * 62 - 24; // pegado abajo, como el teclado de un celular
    float alto = 56, sep = 6;
    float w = (ANCHO - 12 - sep * 9) / 10.0f;
    string filas[2] = {"qwertyuiop", "asdfghjkl"};
    for (int f = 0; f < 2; f++)
    {
        for (int i = 0; i < 10; i++)
        {
            string letra;
            if (f == 1 && i == 9) letra = "ñ";
            else letra = filas[f].substr(i, 1);
            string ver = mayuscula ? aMayuscula(letra) : letra;
            Rectangle r = {6 + i * (w + sep), y, w, alto};
            if (tecla(r, ver, TARJETA2, TEXTO)) tocarLetra(letra);
        }
        y += alto + sep;
    }
    // fila 3: mayúscula, z..m, borrar
    Rectangle rMay = {6, y, w * 1.5f + sep / 2, alto};
    if (tecla(rMay, "Mayús", mayuscula ? COLOR_AHO : TARJETA, mayuscula ? FONDO : TEXTO)) mayuscula = !mayuscula;
    string fila3 = "zxcvbnm";
    for (int i = 0; i < 7; i++)
    {
        string letra = fila3.substr(i, 1);
        Rectangle r = {6 + (w * 1.5f + sep * 1.5f) + i * (w + sep), y, w, alto};
        if (tecla(r, mayuscula ? aMayuscula(letra) : letra, TARJETA2, TEXTO)) tocarLetra(letra);
    }
    Rectangle rBorrar = {ANCHO - 6 - (w * 1.5f + sep / 2), y, w * 1.5f + sep / 2, alto};
    if (tecla(rBorrar, "Borrar", TARJETA, TEXTO)) borrarPendientes++;
    y += alto + sep;
    // fila 4: tilde, números rápidos, espacio, listo
    Rectangle rTilde = {6, y, w * 1.5f, alto};
    if (tecla(rTilde, "´", tilde ? COLOR_AHO : TARJETA, tilde ? FONDO : TEXTO)) tilde = !tilde;
    Rectangle rEsp = {6 + w * 1.5f + sep, y, ANCHO - 12 - w * 1.5f - sep * 2 - 110, alto};
    if (tecla(rEsp, "espacio", TARJETA2, TEXTO2)) pendiente += " ";
    Rectangle rListo = {ANCHO - 6 - 110, y, 110, alto};
    return tecla(rListo, "Listo", COLOR_AHO, FONDO);
}

bool dibujarTecladoNumero(bool conComa)
{
    float y = inicioTeclado + 12;
    float alto = 60, sep = 8;
    float w = (ANCHO - 24 - sep * 2) / 3.0f;
    string teclas[12] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", ",", "0", "Borrar"};
    for (int i = 0; i < 12; i++)
    {
        Rectangle r = {12 + (i % 3) * (w + sep), y + (i / 3) * (alto + sep), w, alto};
        if (i == 9 && !conComa) continue;
        if (tecla(r, teclas[i], i == 11 ? TARJETA : TARJETA2, TEXTO))
        {
            if (i == 11) borrarPendientes++;
            else pendiente += teclas[i];
        }
    }
    Rectangle rListo = {12, y + 4 * (alto + sep), ANCHO - 24, 52};
    return tecla(rListo, "Listo", COLOR_AHO, FONDO);
}

// Dibuja el teclado del campo activo. Devuelve true al tocar "Listo".
bool dibujarTeclado()
{
    if (campoActivo == -1)
    {
        return false;
    }
    // Teclado físico (para probar en la PC)
    int c = GetCharPressed();
    while (c > 0)
    {
        if (c <= 255) // la fuente tiene Latin-1; "€" y emojis se ignoran
        {
            int largo = 0;
            const char *utf8 = CodepointToUTF8(c, &largo);
            pendiente += string(utf8, largo);
        }
        c = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) borrarPendientes++;

    DrawRectangle(0, (int)inicioTeclado, ANCHO, ALTO - (int)inicioTeclado, Color{10, 14, 24, 255});
    bool listo;
    if (tipoActivo == TECLADO_TEXTO || tipoActivo == TECLADO_CLAVE)
    {
        listo = dibujarTecladoTexto();
    }
    else
    {
        listo = dibujarTecladoNumero(tipoActivo == TECLADO_NUMERO);
    }
    if (listo || IsKeyPressed(KEY_ENTER))
    {
        cerrarTeclado();
        return true;
    }
    return false;
}
