#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct Func {

    struct Func **children;
    int num_children;
    char *rel;
    char *exp;
    char var;

} Func; 

Func* mapear_func(const char *expr);
void liberar_func(Func *f);
Func* simplificar_func(const Func *f);
double avaliar_func(const Func *f, double x);

#endif