#ifndef FUNCOES_H
#define FUNCOES_H

#include "relacoes.h"

typedef struct Func {

    struct Func **children;
    int num_children;
    enum RelationType rel;
    int num_rel;
    char *exp;
    char var;

} Func; 

Func* mapear_func(char *expr, char var);
void liberar_func(Func *f);
Func* simplificar_func(const Func *f);
double avaliar_func(const Func *f, double x);

Func* criar_func(char var, const char* exp);
int adicionar_filho(Func *f, Func *child);

#endif