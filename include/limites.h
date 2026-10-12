#ifndef LIMITES_H
#define LIMITES_H

#include "funcoes.h"

enum LimitSide {
    LEFT,
    RIGHT,
    BOTH
};

Func* limite_func(const Func *f, const char *var, double target, LimitSide side);
bool func_continua_em(const Func *f, double x0);

#endif