#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "../include/funcoes.h"
#include "../include/derivadas.h"
#include "../include/limites.h"
#include "../include/utils.h"
#include "test.h"

void testar_limite_func(void) {
    printf("[Módulo 3] Executando testes de limite_func...\n");

    /* Teste 1.1: Polinômio por substituição direta (3*x^2 - 5*x + 2 quando x -> 2: 4.0) */
    Func *f1 = mapear_func("3*x^2 - 5*x + 2", 'x');
    assert(f1 != NULL);
    Func *lim1 = limite_func(f1, 'x', 2.0, BOTH);
    assert(lim1 != NULL);
    assert(fabs(avaliar_func(lim1, 0.0) - 4.0) < 1e-9);
    liberar_func(f1);
    liberar_func(lim1);

    /* Teste 1.2: L'Hôpital obrigatório do TP2 (sen(x) / x quando x -> 0: 1.0) */
    Func *f2 = mapear_func("sen(x) / x", 'x');
    assert(f2 != NULL);
    Func *lim2 = limite_func(f2, 'x', 0.0, BOTH);
    assert(lim2 != NULL);
    assert(fabs(avaliar_func(lim2, 0.0) - 1.0) < 1e-9);
    liberar_func(f2);
    liberar_func(lim2);

    /* Teste 1.3: Quociente com indeterminação 0/0 ((x^2 - 4)/(x - 2) quando x -> 2: 4.0) */
    Func *f3 = mapear_func("(x^2 - 4) / (x - 2)", 'x');
    assert(f3 != NULL);
    Func *lim3 = limite_func(f3, 'x', 2.0, BOTH);
    assert(lim3 != NULL);
    assert(fabs(avaliar_func(lim3, 0.0) - 4.0) < 1e-9);
    liberar_func(f3);
    liberar_func(lim3);

    /* Teste 1.4: Testando com ponteiro nulo */
    Func *lim4_null1 = limite_func(NULL, 'x', 0.0, BOTH);
    assert(lim4_null1 == NULL);

    printf(" -> limite_func: 4 testes aprovados!\n");
}

void testar_func_continua_em(void) {
    printf("[Módulo 3] Executando testes de func_continua_em...\n");

    /* Teste 2.1: Função contínua em todo o domínio */
    Func *f1 = mapear_func("3*x^2 - 5*x + 2", 'x');
    assert(f1 != NULL);
    assert(func_continua_em(f1, 2.0) == true);
    liberar_func(f1);

    /* Teste 2.2: Descontinuidade essencial / assíntota vertical (1/x em x0 = 0) */
    Func *f2 = mapear_func("1 / x", 'x');
    assert(f2 != NULL);
    assert(func_continua_em(f2, 0.0) == false);
    liberar_func(f2);

    /* Teste 2.3: Descontinuidade removível / 0/0 (sen(x)/x em x0 = 0 não tem f(0) definida) */
    Func *f3 = mapear_func("sen(x) / x", 'x');
    assert(f3 != NULL);
    assert(func_continua_em(f3, 0.0) == false);
    liberar_func(f3);

    /* Teste 2.4: Testando com entrada nula */
    assert(func_continua_em(NULL, 1.0) == false);

    printf(" -> func_continua_em: 4 testes aprovados!\n");
}

void rodar_testes_limites(void){
    testar_limite_func();
    testar_func_continua_em();
}
