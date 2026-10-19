#ifndef REPRESENTACAO_H
#define REPRESENTACAO_H

#include "funcoes.h"

void draw_graph(Func *f);
int exportar_repr_func(const Func *f, double x_min, double x_max, int width, int height, const char *filename);

#endif
