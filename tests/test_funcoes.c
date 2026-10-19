#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../include/funcoes.h"
#include "../include/relacoes.h"

static char* alocar_string(char *expr){

    int i = 0;
    for(i = 0; expr[i] != '\0'; i++);
    char *new_expr = malloc(i* sizeof *new_expr);

    for(int j = 0; expr[j] != '\0'; j++){
        new_expr[j] = expr[j];
    }

    return new_expr;

}

void testar_avaliar_func() {
    char *expr = alocar_string("x^2");
    Func *f = mapear_func(expr, 'x');

    /* o parser deve criar uma função válida */
    assert(f != NULL);

    /* 3^2 deve ser 9 */
    double resultado = avaliar_func(f, 3);
    assert(fabs(resultado - 9.0) < 1e-9);

    liberar_func(f);
    free(expr);

    printf("avaliar func [OK]\n");
}

void testar_mapear_func1() {
    char *expr = alocar_string("sen(x) + 3*x^2");
    Func *f = mapear_func(expr, 'x');           // programa executa e apenas imprime
                                                // 2 "\n","sen","\n" e simplesmente
    printf("Função mapeada: %s\n", f->exp);     // nao avanca
    
    /* o parser deve criar uma função válida */
    assert(f != NULL);

    liberar_func(f);
    free(expr);

    printf("mapear func1 [OK]\n");
}

void testar_mapear_func2() {
/* A entrada inválida deve ser rejeitada */
    char *expr_invalida = alocar_string("x + * 2");
    Func *invalida = mapear_func(expr_invalida, 'x');

    assert(invalida == NULL);   // assertion failed: deveria dar NULL,
    free(expr_invalida);        // mas nao está dando por alguma razão.

    printf("mapear func2 [OK]\n");
}

/* main para teste */

int main() {
    testar_mapear_func1();
    testar_mapear_func2();
    testar_avaliar_func();
    return 0;
}