#include <stdbool.h>
#include <stdio.h>
#include "../include/derivadas.h"
#include "../include/limites.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define expr_len 50000
#define EPSILON_X 1e-7   // Distância x0 para aproximação dos limites
#define TOLERANCIA 1e-4  // Tolerância para comparar se dois valores double são iguais

Func* limite_func(Func *f, char var, double target, enum LimitSide side) {
    if (f == NULL) return NULL;
    Func *s1_f = simplificar_func(f);
    
    double a = avaliar_func(s1_f, target);
    if(!isnan(a) && !isinf(a)){
        char *expr = malloc(expr_len * sizeof *expr);
        sprintf(expr, "%f", a);
        expr = realloc(expr, strlen(expr)+1);

        Func *r = mapear_func(expr, s1_f->var);
        free(expr);
        return r;
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
        free(expr);
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
    else if(a < 0) sprintf(expr, "-inf");
    else sprintf(expr, "%f", a);
    Func *r = mapear_func(expr, f->var);
    free(expr);

    return r;

}


bool func_continua_em(Func *f, double x0) {
    if (f == NULL) return false;

    // Func é contínua se, e somente se limf = f(x0) quando x -> x0

    //Avalia o valor exato da função no ponto x0
    double val_fx0 = avaliar_func(f, x0);

    if (isnan(val_fx0) || isinf(val_fx0)) {
        return false;
    }

    Func *lim_func = limite_func(f, f->var, x0, BOTH);
    double val_lim = avaliar_func(lim_func, x0);

    liberar_func(lim_func);

    //Verifica se o limite é igual ao valor f(x0) com tolerância
    if (fabs(val_lim - val_fx0) > TOLERANCIA) {
        return false; 
    }

    return true; // A função é contínua em x0
}
