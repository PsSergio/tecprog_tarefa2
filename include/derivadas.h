#ifndef DERIVADAS_H
#define DERIVADAS_H


#include "funcoes.h"
#include <stdio.h>


Func* derivar_func(Func *f, char var);

Func* reta_tangente_func(Func *f, double x0);

#endif
