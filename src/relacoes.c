
#include "../include/relacoes.h"
#include <stdio.h>

static int comparar_string(char *s1, char *s2){
    for(int i = 0; s1[i] != '\0'; i++){
        if(s1[i] != s2[i]) return 1;
    }

    return 0;
}

enum RelationType identificar_rel(char *rel){
    
    switch (rel[0])
    {
    case '+':
        return SOMA;
    case '-':
        return SUB;
    case '*':
        return MULT;
    case '/':
        return DIV;
    default:
        break;
    }
    

    if(comparar_string(rel, "sen") == 0) return SEN;
    if(comparar_string(rel, "cos") == 0) return COS;
    if(comparar_string(rel, "tan") == 0) return TAN;

    return DESCONHECIDO;
};

