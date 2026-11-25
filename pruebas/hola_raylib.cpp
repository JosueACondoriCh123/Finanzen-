// FinanZen - prueba de que raylib compila y dibuja (Tarea 1)
#include "raylib.h"
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    string captura = "";
    if (argc == 3 && string(argv[1]) == "--captura") captura = argv[2];

    InitWindow(450, 900, "FinanZen - hola raylib");
    SetTargetFPS(60);
    Font fuente = LoadFontEx("assets/Inter-Bold.ttf", 48, 0, 250);
    int cuadro = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{15, 20, 32, 255});
        DrawTextEx(fuente, "FinanZen", Vector2{120, 400}, 48, 0, RAYWHITE);
        DrawTextEx(fuente, "Año, ñandú, ahorro", Vector2{80, 470}, 28, 0, Color{120, 220, 160, 255});
        EndDrawing();
        cuadro++;
        if (captura != "" && cuadro == 2)
        {
            Image img = LoadImageFromScreen();
            ExportImage(img, captura.c_str());
            UnloadImage(img);
            break;
        }
    }
    UnloadFont(fuente);
    CloseWindow();
    return 0;
}
