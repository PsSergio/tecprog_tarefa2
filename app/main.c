#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/funcoes.h"
#include "../include/derivadas.h"
#include "../include/limites.h"

int main (void){

    printf("\nARQUIVO MAIN - BIBLIOTECA DE CÁLCULO 1\n\n");

    printf("Módulo 1 - Funções\n");
    char *expr = "sen(x)/x";

    Func *f = mapear_func(expr, 'x');

    if(!f){
        fprintf(stderr, "Erro ao fazer o parse da função.\n");
        return EXIT_FAILURE;
    }

    printf("Função mapeada: f(x) = %s\n", f->exp);
    printf("f(1) = %f\n\n", avaliar_func(f, 1.0));

    printf("Módulo 2 - Derivadas\n");

    Func* dfdx = derivar_func(f, 'x');
    printf("f'(x) = %s\n", dfdx->exp);

    Func* tangente_f = reta_tangente_func(f, 1.0);
    printf("Equação da reta tangente em (1.0): t(1) = %s\n", tangente_f->exp);
    printf("Comparando f(1.0) com t(1.0): %f && %f\n\n", avaliar_func(f, 1.0), avaliar_func(tangente_f, 1.0));

    printf("Módulo 3 - Limites\n");
    
    bool continua_em_0 = func_continua_em(f, 0.0);
    printf("%s é contínua em 0.0 ? Resultado: %s\n", f->exp, continua_em_0 ? "Sim" : "Não");

    Func *limite_0 = limite_func(f, 'x', 0.0, BOTH);

    printf("Lim (%s) quando x-> 0.0 = %f\n\n", f->exp, avaliar_func(limite_0, 0.0));

}
