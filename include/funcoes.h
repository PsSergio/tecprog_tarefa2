#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct {

    char expr[1000]; // incompleto

} Func; 

Func* mapear_func(const char *expr);
void liberar_func(Func *f);
Func* simplificar_func(const Func *f);
double avaliar_func(const Func *f, double x);

#endif