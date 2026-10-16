
#include "../include/relacoes.h"
#include "../include/funcoes.h"
#include <math.h>
#include <stdlib.h>
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
    case '^':
        return POT;
    default:
        break;
    }
    

    if(comparar_string(rel, "sen") == 0) return SEN;
    if(comparar_string(rel, "cos") == 0) return COS;
    if(comparar_string(rel, "tan") == 0) return TAN;

    return DESCONHECIDO;
};



void print_rel(enum RelationType rel){

    switch (rel)
    {
    case SOMA:
        printf("+");
        break;
    case SUB:
        printf("-");
        break;
    case MULT:
        printf("*");
        break;
    case DIV:
        printf("/");
        break;
    case POT:
        printf("^");
        break;
    case SEN:
        printf("sen");
        break;
    case COS:
        printf("cos");
        break;
    case TAN:
        printf("tan");
        break;
    default:
        printf("desconhecido");
        break;
    }
}

/* 
 @brief Aplica a relação de RelationType em dois valores x1 e x2
 @param x1 representa o primeiro elemento da equação
 @param x2 representa o segundo elemento da equação
 @param rel representa a relação da equação a ser calculada
 @return o valor que representa x1Rx2 ou R(x1)
 @note Caso a relação aborde apenas um elemento, então x2 será desconsiderado
*/
double aplicar_relacao(double x1, double x2, enum RelationType rel){
    if(x1 == NAN || x2 == NAN) return NAN;
    switch (rel)
    {
    case SOMA:
        return x1+x2;
        
    case SUB:
        return x1-x2;
        
    case MULT:
        return x1*x2;
        
    case DIV:
        if(x1 == 0 && x2 == 0) return NAN;
        if(x2 == 0 && x1 > 0) return INFINITY;
        else if(x2 == 0 && x2 < 0) return -INFINITY;
        
        return x1/x2;
    case POT:
        if(x1 == 0 && x2 == 0) return NAN;
        return pow(x1, x2);
    case SEN:
        return sin(x1);
    case COS:
        return cos(x1);
    case TAN:
        return tan(x1);
    default:
        return NAN;
    }
}