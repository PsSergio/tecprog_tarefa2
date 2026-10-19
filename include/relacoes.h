#ifndef RELACOES_H
#define RELACOES_H

enum RelationType {
    SOMA,
    SUB,
    MULT,
    DIV,
    SEN,
    COS,
    TAN,
    POT,
    DESCONHECIDO
};

enum RelationType identificar_rel(char *rel);
void print_rel(enum RelationType rel);
double aplicar_relacao(double x1, double x2, enum RelationType rel);

#endif
