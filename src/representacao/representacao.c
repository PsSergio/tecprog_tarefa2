#include "../../include/representacao.h"
#include "../../include/funcoes.h"
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include <math.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600
#define SCALE 30.0

double avaliar_func(Func *f, double x);

// Converte coordenadas matemáticas para pixels
static Vector2 to_screen(double x, double y) {
    Vector2 screen;
    screen.x = WIDTH / 2 + x * SCALE;
    screen.y = HEIGHT / 2 - y * SCALE; // Inverte o eixo y
    return screen;
}

void draw_graph(Func *f) {

    // eixos cartesianos
    DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, GRAY); // eixo x
    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GRAY); // eixo y

    // Graduação dos eixos
    for (int x = 0; x < WIDTH; x += (int)SCALE) {
        DrawLine(x, HEIGHT / 2 - 4,
                 x, HEIGHT / 2 + 4, GRAY);
    }
    for (int y = 0; y < HEIGHT; y += (int)SCALE) {
        DrawLine(WIDTH / 2 - 4, y,
                 WIDTH / 2 + 4, y, GRAY);
    }

    // Desenha a curva amostrando vários valores de x
    bool previous_valid = false;
    Vector2 previous = {0, 0};

    for (double x = -WIDTH / (2.0 * SCALE); x <= WIDTH / (2.0 * SCALE); x += 0.01) {
        double y = avaliar_func(f, x);

        // Evita desenhar valores inválidos ou fora da tela.
        bool valid = isfinite(y) 
                    && fabs(y) < HEIGHT / (2.0 * SCALE);
        
        if (valid) {
            Vector2 current = to_screen(x, y);

            if (previous_valid) {
                DrawLineV(previous, current, RED);
            }

            previous = current;
        }

        previous_valid = valid;
    }
}

// funcao auxiliar
char* alocar_string(char *expr){

    int i = 0;
    for(i = 0; expr[i] != '\0'; i++);
    char *new_expr = malloc(i* sizeof *new_expr);

    for(int j = 0; expr[j] != '\0'; j++){
        new_expr[j] = expr[j];
    }

    return new_expr;

}

int main() {

    //funcao teste
    char *expr = alocar_string("x^2");
    Func *f = mapear_func(expr, 'x');

    InitWindow(800, 600, "Gráfico da função");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_graph(f); // f é a função que você quer desenhar

        EndDrawing();
    }

    CloseWindow();
}