#ifndef REPRESENTACAO_H
#define REPRESENTACAO_H

#include "funcoes.h"

void draw_graph(const Func *f);
void repr_func_terminal(const Func *f, double x_min, double x_max, int width, int height);
int exportar_repr_func(const Func *f, double x_min, double x_max, int width, int height, const char *filename);

#endif