// FinanZen — estilo y controles de la app (botones, tarjetas, textos, toques) · integrado con ayuda de Claude Code (IA)
#include "app.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

// Paleta: fondo oscuro, tarjetas, y un color por módulo
Color FONDO = {15, 20, 32, 255};
Color TARJETA = {28, 36, 54, 255};
Color TARJETA2 = {40, 50, 74, 255};
Color TEXTO = {236, 240, 247, 255};
Color TEXTO2 = {148, 160, 184, 255};
Color VERDE = {72, 207, 140, 255};
Color ROJO = {245, 101, 101, 255};
Color AMARILLO = {246, 196, 83, 255};
Color COLOR_PERFIL = {168, 140, 255, 255};
Color COLOR_ING = {72, 207, 140, 255};
Color COLOR_GAS = {255, 128, 102, 255};
Color COLOR_AHO = {80, 170, 255, 255};
Color COLOR_REP = {246, 196, 83, 255};

Font fuente;
Font fuenteNegrita;
bool hayFuente = false;

void cargarEstilo()
{
    // Letras: ASCII + Latin-1 (tildes, ñ, ¿, ¡)
    int codigos[224];
    for (int i = 0; i < 224; i++)
    {
        codigos[i] = 32 + i;
    }
#if defined(PLATFORM_ANDROID)
    bool existe = true; // en Android la fuente viene dentro del .apk (FileExists no ve los assets)
#else
    bool existe = FileExists("assets/Inter-Regular.ttf") && FileExists("assets/Inter-Bold.ttf");
#endif
#if defined(PLATFORM_ANDROID)
    string carpeta = ""; // dentro del .apk las fuentes están en la raíz de assets/
#else
    string carpeta = "assets/";
#endif
    if (existe)
    {
        fuente = LoadFontEx((carpeta + "Inter-Regular.ttf").c_str(), 64, codigos, 224);
        fuenteNegrita = LoadFontEx((carpeta + "Inter-Bold.ttf").c_str(), 64, codigos, 224);
        SetTextureFilter(fuente.texture, TEXTURE_FILTER_BILINEAR);
        SetTextureFilter(fuenteNegrita.texture, TEXTURE_FILTER_BILINEAR);
        hayFuente = true;
    }
    else
    {
        fuente = GetFontDefault();
        fuenteNegrita = GetFontDefault();
    }
}

void descargarEstilo()
{
    if (hayFuente)
    {
        UnloadFont(fuente);
        UnloadFont(fuenteNegrita);
    }
}

void texto(string s, float x, float y, float tam, Color c)
{
    DrawTextEx(fuente, s.c_str(), Vector2{x, y}, tam, 0, c);
}

void textoNegrita(string s, float x, float y, float tam, Color c)
{
    DrawTextEx(fuenteNegrita, s.c_str(), Vector2{x, y}, tam, 0, c);
}

float anchoTexto(string s, float tam, bool negrita)
{
    return MeasureTextEx(negrita ? fuenteNegrita : fuente, s.c_str(), tam, 0).x;
}

void textoCentrado(string s, float y, float tam, Color c, bool negrita)
{
    float x = (ANCHO - anchoTexto(s, tam, negrita)) / 2;
    if (negrita) textoNegrita(s, x, y, tam, c);
    else texto(s, x, y, tam, c);
}

void dibujarTarjeta(Rectangle r, Color c)
{
    DrawRectangleRounded(r, 0.18f, 8, c);
}

// ---------- Toques ----------
// Un toque es soltar el dedo en el mismo lugar donde se apoyó (si se movió, es un arrastre).
Vector2 presion = {-100, -100};
Vector2 anterior = {0, 0};
bool fueArrastre = false;

void actualizarToques()
{
    Vector2 m = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        presion = m;
        anterior = m;
        fueArrastre = false;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        if (fabs(m.x - presion.x) > 12 || fabs(m.y - presion.y) > 12)
        {
            fueArrastre = true;
        }
    }
}

bool tocado(Rectangle r)
{
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || fueArrastre)
    {
        return false;
    }
    return CheckCollisionPointRec(GetMousePosition(), r) && CheckCollisionPointRec(presion, r);
}

// Arrastrar con el dedo (o la rueda del mouse) dentro de area mueve el contenido.
// Devuelve el desplazamiento actual.
float desplazar(Rectangle area, float &desplazamiento, float altoContenido)
{
    Vector2 m = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(presion, area))
    {
        desplazamiento -= m.y - anterior.y;
    }
    if (CheckCollisionPointRec(m, area))
    {
        desplazamiento -= GetMouseWheelMove() * 40;
    }
    anterior = m;
    float maximo = altoContenido - area.height;
    if (maximo < 0) maximo = 0;
    if (desplazamiento > maximo) desplazamiento = maximo;
    if (desplazamiento < 0) desplazamiento = 0;
    return desplazamiento;
}

// Fila tocada en una lista desplazable, o -1
int filaTocada(Rectangle area, int cantidad, float altoFila, float desplazamiento)
{
    if (!tocado(area))
    {
        return -1;
    }
    int fila = (int)((GetMousePosition().y - area.y + desplazamiento) / altoFila);
    if (fila < 0 || fila >= cantidad)
    {
        return -1;
    }
    return fila;
}

// ---------- Botones y tarjetas ----------

bool botonGrande(Rectangle r, string s, Color c)
{
    bool encima = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), r) &&
                  CheckCollisionPointRec(presion, r);
    Color fondo = c;
    if (encima) fondo = ColorBrightness(c, -0.2f);
    DrawRectangleRounded(r, 0.5f, 10, fondo);
    float t = 21;
    textoNegrita(s, r.x + (r.width - anchoTexto(s, t, true)) / 2, r.y + (r.height - t) / 2, t, FONDO);
    return tocado(r);
}

bool botonSecundario(Rectangle r, string s)
{
    DrawRectangleRoundedLinesEx(r, 0.5f, 10, 2, TARJETA2);
    float t = 19;
    texto(s, r.x + (r.width - anchoTexto(s, t, false)) / 2, r.y + (r.height - t) / 2, t, TEXTO);
    return tocado(r);
}

// Tarjeta grande de menú: es uno de los 15 menús del proyecto
bool tarjetaMenu(float y, string numero, string titulo, string detalle, Color c)
{
    Rectangle r = {24, y, ANCHO - 48, 104};
    bool encima = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), r) &&
                  CheckCollisionPointRec(presion, r);
    dibujarTarjeta(r, encima ? TARJETA2 : TARJETA);
    DrawCircle(66, (int)(y + 52), 24, ColorAlpha(c, 0.18f));
    textoNegrita(numero, 66 - anchoTexto(numero, 22, true) / 2, y + 41, 22, c);
    textoNegrita(titulo, 106, y + 26, 21, TEXTO);
    texto(detalle, 106, y + 56, 16, TEXTO2);
    // flecha
    DrawLineEx(Vector2{ANCHO - 52, y + 44}, Vector2{ANCHO - 44, y + 52}, 3, TEXTO2);
    DrawLineEx(Vector2{ANCHO - 44, y + 52}, Vector2{ANCHO - 52, y + 60}, 3, TEXTO2);
    return tocado(r);
}

// Barra superior con botón atrás. Si lo tocan (o el botón atrás de Android), vuelve.
bool encabezado(string titulo, Color c)
{
    DrawRectangle(0, 0, ANCHO, 4, c);
    Rectangle atras = {8, 16, 56, 56};
    DrawLineEx(Vector2{40, 32}, Vector2{28, 44}, 4, TEXTO);
    DrawLineEx(Vector2{28, 44}, Vector2{40, 56}, 4, TEXTO);
    textoNegrita(titulo, 72, 30, 26, TEXTO);
    if (tocado(atras) || IsKeyPressed(KEY_BACK) || IsKeyPressed(KEY_ESCAPE))
    {
        irA(padreDe(pantalla));
        return true;
    }
    return false;
}

void barraProgreso(Rectangle r, double porcentaje, Color c)
{
    if (porcentaje < 0) porcentaje = 0;
    if (porcentaje > 100) porcentaje = 100;
    DrawRectangleRounded(r, 1.0f, 10, TARJETA2);
    if (porcentaje > 0)
    {
        Rectangle lleno = {r.x, r.y, (float)(r.width * porcentaje / 100.0), r.height};
        if (lleno.width < r.height) lleno.width = r.height;
        DrawRectangleRounded(lleno, 1.0f, 10, c);
    }
}

// Pastillas para elegir una opción (categorías). Devuelve true si cambió.
bool chips(float y, string nombres[], int cantidad, int &elegido, Color c)
{
    float x = 24;
    bool cambio = false;
    for (int i = 0; i < cantidad; i++)
    {
        float w = anchoTexto(nombres[i], 17, false) + 30;
        if (x + w > ANCHO - 24)
        {
            x = 24;
            y += 46;
        }
        Rectangle r = {x, y, w, 38};
        if (i == elegido)
        {
            DrawRectangleRounded(r, 1.0f, 10, c);
            texto(nombres[i], x + 15, y + 10, 17, FONDO);
        }
        else
        {
            DrawRectangleRounded(r, 1.0f, 10, TARJETA);
            texto(nombres[i], x + 15, y + 10, 17, TEXTO);
        }
        if (tocado(r))
        {
            elegido = i;
            cambio = true;
        }
        x += w + 8;
    }
    return cambio;
}

// Barra inferior: Inicio y los módulos 2 a 5 (el perfil se abre desde Inicio)
void barraInferior()
{
    float y = ALTO - 76;
    DrawRectangle(0, (int)y, ANCHO, 76, TARJETA);
    string nombres[5] = {"Inicio", "Ingresos", "Gastos", "Ahorro", "Reportes"};
    int destinos[5] = {PANT_INICIO, PANT_INGRESOS, PANT_GASTOS, PANT_AHORRO, PANT_REPORTES};
    Color colores[5] = {TEXTO, COLOR_ING, COLOR_GAS, COLOR_AHO, COLOR_REP};
    int actual = pantalla;
    while (actual != PANT_INICIO && padreDe(actual) != PANT_INICIO && padreDe(actual) != actual)
    {
        actual = padreDe(actual);
    }
    for (int i = 0; i < 5; i++)
    {
        float x = i * (ANCHO / 5.0f);
        Rectangle r = {x, y, ANCHO / 5.0f, 76};
        bool activo = (actual == destinos[i]);
        Color c = activo ? colores[i] : TEXTO2;
        DrawCircle((int)(x + r.width / 2), (int)(y + 26), activo ? 10 : 7, c);
        float t = 14;
        texto(nombres[i], x + (r.width - anchoTexto(nombres[i], t, false)) / 2, y + 44, t, c);
        if (tocado(r) && !activo)
        {
            irA(destinos[i]);
        }
    }
}

// ---------- Avisos ----------
string mensajeAviso = "";
double finAviso = 0;

void aviso(string mensaje)
{
    mensajeAviso = mensaje;
    finAviso = GetTime() + 2.5;
}

void dibujarAviso()
{
    if (GetTime() > finAviso || mensajeAviso == "")
    {
        return;
    }
    float w = anchoTexto(mensajeAviso, 17, false) + 40;
    if (w > ANCHO - 30) w = ANCHO - 30;
    Rectangle r = {(ANCHO - w) / 2, 90, w, 48};
    DrawRectangleRounded(r, 0.5f, 10, Color{245, 245, 250, 245});
    texto(mensajeAviso, r.x + 20, r.y + 14, 17, FONDO);
}

// ---------- Números y fechas ----------

// "Álvaro" -> "Á": una letra con tilde ocupa 2 bytes en UTF-8
string primeraLetra(string s)
{
    if (s.length() == 0) return "";
    int largo = 1;
    unsigned char c = s[0];
    if (c >= 0xF0) largo = 4;
    else if (c >= 0xE0) largo = 3;
    else if (c >= 0xC0) largo = 2;
    return s.substr(0, largo);
}

// 1240.5 -> "Bs 1.240,50" (como se escribe en Bolivia)
string formatoBs(double v)
{
    bool negativo = v < 0;
    long long centavos = llround(fabs(v) * 100);
    long long entero = centavos / 100;
    int dec = (int)(centavos % 100);
    string s = to_string(entero);
    string conPuntos = "";
    int cuenta = 0;
    for (int i = (int)s.length() - 1; i >= 0; i--)
    {
        conPuntos = s[i] + conPuntos;
        cuenta++;
        if (cuenta % 3 == 0 && i > 0)
        {
            conPuntos = "." + conPuntos;
        }
    }
    string d = (dec < 10 ? "0" : "") + to_string(dec);
    return string(negativo ? "-" : "") + "Bs " + conPuntos + "," + d;
}

string montoATexto(double v)
{
    long long centavos = llround(v * 100);
    int dec = (int)(centavos % 100);
    return to_string(centavos / 100) + "," + (dec < 10 ? "0" : "") + to_string(dec);
}

// Acepta "1240,50" o "1240.50"
bool leerMontoTexto(string s, double &v)
{
    for (int i = 0; i < (int)s.length(); i++)
    {
        if (s[i] == ',') s[i] = '.';
    }
    if (s.length() == 0 || s == ".")
    {
        return false;
    }
    char *fin;
    v = strtod(s.c_str(), &fin);
    return *fin == '\0';
}

string hoy()
{
    time_t t = time(0);
    tm *f = localtime(&t);
    string dd = (f->tm_mday < 10 ? "0" : "") + to_string(f->tm_mday);
    string mm = (f->tm_mon + 1 < 10 ? "0" : "") + to_string(f->tm_mon + 1);
    return dd + "/" + mm + "/" + to_string(f->tm_year + 1900);
}

void guardarApp()
{
    if (!guardarTodo())
    {
        aviso("No se pudo guardar en el celular");
    }
}
