#ifndef DERIVADAS_H
#define DERIVADAS_H

#include "funcoes.h"

Func* derivar_func(const Func *f, const char *var);
Func* reta_tangente(const Func *f, const char *x0);

#endif