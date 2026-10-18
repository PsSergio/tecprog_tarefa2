#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "../include/funcoes.h"
#include "../include/derivadas.h"

static char* alocar_string(char *expr){

    int i = 0;
    for(i = 0; expr[i] != '\0'; i++);
    char *new_expr = malloc(i* sizeof *new_expr);

    for(int j = 0; expr[j] != '\0'; j++){
        new_expr[j] = expr[j];
    }

    return new_expr;

}

void testar_derivar_func() {
    printf("[Módulo 2] Executando testes de derivar_func...\n");

    /* Teste 1.1: Polinômio padrão (x^3 -> 3*x^2). Para x = 2: 3*(4) = 12.0 */
    Func *f1 = mapear_func(alocar_string("x^3"), 'x');
    assert(f1 != NULL);
    Func *df1 = derivar_func(f1, 'x');
    assert(df1 != NULL);
    assert(fabs(avaliar_func(df1, 2.0) - 12.0) < 1e-9);
    liberar_func(f1);
    liberar_func(df1);

    /* Teste 1.2: Função elementar (sen(x) -> cos(x)). Para x = 0: cos(0) = 1.0 */
    Func *f2 = mapear_func(alocar_string("sen(x)"), 'x');
    assert(f2 != NULL);
    Func *df2 = derivar_func(f2, 'x');
    assert(df2 != NULL);
    assert(fabs(avaliar_func(df2, 0.0) - 1.0) < 1e-9);
    liberar_func(f2);
    liberar_func(df2);

    /* Teste 1.3: Derivação parcial em relação a outra variável (x^2 rel a y -> 0) */
    Func *f3 = mapear_func(alocar_string("x^2"), 'x');
    assert(f3 != NULL);
    Func *df3 = derivar_func(f3, 'y');
    assert(df3 != NULL);
    assert(fabs(avaliar_func(df3, 5.0) - 0.0) < 1e-9);
    liberar_func(f3);
    liberar_func(df3);

    /* Teste 1.4: Testando com ponteiro nulo */
    Func *df4_null1 = derivar_func(NULL, 'x');
    assert(df4_null1 == NULL);

    Func *f4 = mapear_func(alocar_string(""), 'x');
    Func *df4_null2 = derivar_func(f4, NULL);
    assert(df4_null2 == NULL);
    liberar_func(f4);

    printf(" -> derivar_func: 4 testes aprovados!\n");
}

/* ========================================================================= */
/* 2. TESTES PARA reta_tangente_func                                        */
/* ========================================================================= */
void testar_reta_tangente_func() {
    printf("[Módulo 2] Executando testes de reta_tangente_func...\n");

    /* Teste 2.1: Caso nominal do TP2 (x^2 em x0 = 2 -> y = 4*x - 4) */
    Func *f1 = mapear_func(alocar_string("x^2"), 'x');
    assert(f1 != NULL);
    Func *tangente1 = reta_tangente_func(f1, 2.0);
    assert(tangente1 != NULL);
    // Avaliação no ponto de tangência x = 2: 4(2) - 4 = 4.0
    assert(fabs(avaliar_func(tangente1, 2.0) - 4.0) < 1e-9);
    // Avaliação no intercepto x = 0: 4(0) - 4 = -4.0
    assert(fabs(avaliar_func(tangente1, 0.0) - (-4.0)) < 1e-9);
    liberar_func(f1);
    liberar_func(tangente1);

    /* Teste 2.2: Reta tangente horizontal (x^2 em x0 = 0 -> y = 0) */
    Func *f2 = mapear_func(alocar_string("x^2"), 'x');
    assert(f2 != NULL);
    Func *tangente2 = reta_tangente_func(f2, 0.0);
    assert(tangente2 != NULL);
    assert(fabs(avaliar_func(tangente2, 0.0) - 0.0) < 1e-9);
    assert(fabs(avaliar_func(tangente2, 5.0) - 0.0) < 1e-9);
    liberar_func(f2);
    liberar_func(tangente2);

    /* Teste 2.3: Ponto fora do domínio / assíntota (1/x em x0 = 0) */
    Func *f3 = mapear_func(alocar_string("1 / x"), 'x');
    assert(f3 != NULL);
    Func *tangente3 = reta_tangente_func(f3, 0.0);
    if (tangente3 != NULL) {
        double y_val = avaliar_func(tangente3, 1.0);
        assert(isnan(y_val) || isinf(y_val));
        liberar_func(tangente3);
    } else {
        assert(tangente3 == NULL);
    }
    liberar_func(f3);

    /* Teste 2.4: Testando com função nula */
    Func *tangente4 = reta_tangente_func(NULL, 1.0);
    assert(tangente4 == NULL);

    printf(" -> reta_tangente_func: 4 testes aprovados!\n");
}

int main() {

    testar_derivar_func();
    testar_reta_tangente_func();

    return 0;
}