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

    printf("[Módulo 1] Executando testes de avaliar_func...\n");

    /* Teste 4.1: Avaliação regular de polinômio */
    Func *f1 = mapear_func(alocar_string("3*x^2 - 5*x + 2"), 'x');
    assert(f1 != NULL);
    assert(fabs(avaliar_func(f1, 2.0) - 4.0) < 1e-9);
    liberar_func(f1);
    printf("->Teste 1 OK\n");

    /* Teste 4.2: Assíntota vertical (divisão por zero c/0) -> INFINITY */
    Func *f2 = mapear_func(alocar_string("1 / x"), 'x');
    assert(f2 != NULL);
    double res2 = avaliar_func(f2, 0.0);
    assert(isinf(res2));
    liberar_func(f2);
    printf("->Teste 2 OK\n");

    /* Teste 4.3: Forma indeterminada 0/0 -> NAN */
    Func *f3 = mapear_func(alocar_string("sen(x) / x"), 'x');
    assert(f3 != NULL);
    double res3 = avaliar_func(f3, 0.0);
    assert(isnan(res3));
    liberar_func(f3);
    printf("->Teste 3 OK\n");

    Func *f = mapear_func(alocar_string("x^2"), 'x');
    assert(f != NULL);
    double resultado = avaliar_func(f, 3);
    assert(fabs(resultado - 9.0) < 1e-9);
    liberar_func(f);
    printf("->Teste 4 OK\n");

    printf("***** simplificar_func: 4 testes aprovados! *****\n");

}

void testar_mapear_func() {
    printf("[Módulo 1] Executando testes de mapear_func...\n");

    /* Teste 1.1 */
    char *expr1 = alocar_string("3*x^2 - 5*x + 2");
    Func *f1 = mapear_func(expr1, 'x');
    assert(f1 != NULL);
    liberar_func(f1);
    printf("->Teste 1 OK\n");

    /* Teste 1.2 */
    char *expr2 = alocar_string("sen(x) + 3*x^2");
    Func *f2 = mapear_func(expr2, 'x');
    assert(f2 != NULL);
    liberar_func(f2);
    printf("->Teste 2 OK\n");

    /* Teste 1.3 */
    // char *expr3 = alocar_string(NULL);
    Func *f3_null = mapear_func(NULL, 'x');

    assert(f3_null == NULL);
    printf("->Teste 3.1 OK\n");

    char *expr3v = alocar_string("");
    Func *f3_vazia = mapear_func(expr3v, 'x');
    assert(f3_vazia == NULL);
    printf("->Teste 3.2 OK\n");

    printf("***** mapear_func: 4 testes aprovados! *****\n");

}

void testar_simplificar_func() {
    printf("[Módulo 1] Executando testes de simplificar_func...\n");

    /* Teste 3.1: */
    Func *f1 = mapear_func(alocar_string("x * 0 + 1 * x"), 'x');
    assert(f1 != NULL);
    Func *s1 = simplificar_func(f1);
    assert(s1 != NULL);
    assert(fabs(avaliar_func(s1, 5.0) - 5.0) < 1e-9);
    liberar_func(f1);
    liberar_func(s1);
    printf("->Teste 1 OK\n");

    /* Teste 3.2:  */
    Func *f2 = mapear_func(alocar_string("2 + 3 * 4"), 'x');
    assert(f2 != NULL);
    Func *s2 = simplificar_func(f2);
    assert(s2 != NULL);
    assert(fabs(avaliar_func(s2, 0.0) - 14.0) < 1e-9);
    liberar_func(f2);
    liberar_func(s2);
    printf("->Teste 2 OK\n");

    /* Teste 3.3: Regra de potência (u^0 -> 1) */
    Func *f3 = mapear_func(alocar_string("(x + 5)^0"), 'x');
    assert(f3 != NULL);
    Func *s3 = simplificar_func(f3);
    assert(s3 != NULL);
    assert(fabs(avaliar_func(s3, 10.0) - 1.0) < 1e-9);
    liberar_func(f3);
    liberar_func(s3);
    printf("->Teste 3 OK\n");

    /* Teste 3.4: Cancelamento de termos idênticos (x - x -> 0) */
    Func *f4 = mapear_func(alocar_string("x - x"), 'x');
    assert(f4 != NULL);
    Func *s4 = simplificar_func(f4);
    assert(s4 != NULL);
    assert(fabs(avaliar_func(s4, 7.0) - 0.0) < 1e-9);
    liberar_func(f4);
    liberar_func(s4);
    printf("->Teste 4 OK\n");

    printf("***** simplificar_func: 4 testes aprovados! *****\n");
}

/* main para teste */

int main() {
    testar_mapear_func();
    testar_avaliar_func();
    testar_simplificar_func();
    return 0;
}