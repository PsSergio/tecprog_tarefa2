#ifndef LIMITES_H
#define LIMITES_H

#include "funcoes.h"

enum LimitSide {
    LEFT,
    RIGHT,
    BOTH
};

Func* limite_func(Func *f, char var, double target, enum LimitSide side);
bool func_continua_em(Func *f, double x0);

#endif
