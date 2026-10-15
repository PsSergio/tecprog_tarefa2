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
    DESCONHECIDO
};

enum RelationType identificar_rel(char *rel);

#endif