#ifndef DERIVADAS_H
#define DERIVADAS_H


#include "funcoes.h"
#include <stdio.h>


Func* derivar_func(const Func *f, const char *var);

Func* reta_tangente_func(const Func *f, double x0);

#endif