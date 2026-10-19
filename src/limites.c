#include <stdbool.h>
#include <stdio.h>
#include "../include/derivadas.h"
#include "../include/limites.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define expr_len 1024
#define EPSILON_X 1e-7   // Distância x0 para aproximação dos limites
#define TOLERANCIA 1e-4  // Tolerância para comparar se dois valores double são iguais

static char* alocar_string(char *expr){

    int i = 0;
    for(i = 0; expr[i] != '\0'; i++);
    char *new_expr = malloc((i+1)* sizeof *new_expr);

    for(int j = 0; expr[j] != '\0'; j++){
        new_expr[j] = expr[j];
    }

    new_expr[i] = '\0';

    return new_expr;

}

Func* limite_func(const Func *f, const char var, double target, enum LimitSide side) {
    if (f == NULL) return NULL;
    Func *s1_f = simplificar_func(f);
    
    double a = avaliar_func(s1_f, target);
    if(!isnan(a) && !isinf(a)){
        char *expr = malloc(expr_len * sizeof *expr);
        sprintf(expr, "%f", a);
        expr = realloc(expr, strlen(expr)+1);

        return mapear_func(expr, s1_f->var);
    }

    // nao é um número - inf ou nan
    
    // aplicar LHopital
    if(f->num_children == 2 && s1_f->rel == DIV && isnan(a)){
        Func *u_linha = derivar_func(s1_f->children[0], s1_f->children[0]->var);
        Func *v_linha = derivar_func(s1_f->children[1], s1_f->children[1]->var);

        char *expr = malloc(expr_len * sizeof *expr);
        // printf("%s\n\n", expr);

        sprintf(expr, "(%s)/(%s)", u_linha->exp, v_linha->exp);
        expr = realloc(expr, strlen(expr)+1);

        Func *n_f = mapear_func(expr, s1_f->var);
        Func *s_f = simplificar_func(n_f);
        
        liberar_func(u_linha);
        liberar_func(v_linha);
        liberar_func(n_f);

        Func *r = limite_func(s_f, var, target, side);
        liberar_func(s_f);
        return r;

    }   

    if(isinf(a) && !isinf(target) && side == BOTH){
        Func *l_e = limite_func(f, var, target-EPSILON_X, LEFT);
        Func *l_d = limite_func(f, var, target+EPSILON_X, RIGHT);

        if(strcmp(l_e->exp, l_d->exp) == 0) {
            liberar_func(l_e);
            return l_d;}

        liberar_func(l_e);
        liberar_func(l_d);
        return NULL;
    }

    if(isnan(a)) return NULL;

    if(side == LEFT) a = avaliar_func(s1_f, target-EPSILON_X);
    if(side == RIGHT) a = avaliar_func(s1_f, target+EPSILON_X);

    char *expr = malloc(expr_len * sizeof *expr);
    
    if(a > 0) sprintf(expr, "+inf");
    else if(a < 0) sprintf(expr, "-inf", a);
    else sprintf(expr, "%f", a);
    return mapear_func(expr, f->var);

}


bool func_continua_em(const Func *f, double x0) {
    if (f == NULL) return false;

    // Func é contínua se, e somente se limf = f(x0) quando x -> x0

    //Avalia o valor exato da função no ponto x0
    double val_fx0 = avaliar_func(f, x0);

    //Se f(x0) for indeterminado ou infinito, a função não é contínua em x0
    if (isnan(val_fx0) || isinf(val_fx0)) {
        return false;
    }

    //Calcula os limites de aproximação pela esquerda (x0 - epsilon) e pela direita (x0 + epsilon)
    Func *lim_func = limite_func(f, f->var, x0, BOTH);
    double val_lim = avaliar_func(f, x0);

    //Verifica se o limite é igual ao valor f(x0) com tolerância
    if (fabs(val_lim - val_fx0) > TOLERANCIA) {
        return false; 
    }

    return true; // A função é contínua em x0
}

// int main(){

//     // Func *f = mapear_func(alocar_string("(x^5+(x^3))/((x^5)+(x^2))"), 'x');
//     Func *f = mapear_func(alocar_string("(x^5+x^3)/(x^5+x^2)"), 'x');
//     Func *l = limite_func(f, f->var, +INFINITY, BOTH);

//     if(l == NULL){
//         printf("Nulo");
//         liberar_func(f);
//         return 0;
//     }
//     printf("%s", l->exp);

//     liberar_func(f);
//     liberar_func(l);

// }