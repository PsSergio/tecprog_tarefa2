#ifndef DERIVADAS_H
#define DERIVADAS_H


#include "funcoes.h"
#include <stdio.h>


Func* derivar_func(const Func *f);

Func* reta_tangente(const Func *f, double x0);

#endif