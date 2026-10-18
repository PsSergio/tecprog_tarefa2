#ifndef LIMITES_H
#define LIMITES_H

#include "funcoes.h"

enum LimitSide {
    LEFT,
    RIGHT,
    BOTH
};

#define EPSILON_X 1e-7   // Distância minúscula de x0 para aproximação dos limites
#define TOLERANCIA 1e-4  // Tolerância para comparar se dois valores flutuantes são iguais

static bool indeterminacao_zero_sobre_zero(const Func *f, double target, char var) {
    //Se não for uma divisão, L'Hôpital direto não se aplica nesta camada
    if (f->rel != DIV) return false;

    Func *numerador = f->children[0];
    Func *denominador = f->children[1];

    double val_num = avaliar_func(numerador, target);
    double val_den = avaliar_func(denominador, target);

    //Condição de 0/0 (com pequena tolerância)
    if (fabs(val_num) < 1e-6 && fabs(val_den) < 1e-6) {
        return true;
    }
    return false;
}


Func* limite_func(const Func *f, const char *var, double target, LimitSide side) {
    if (f == NULL) return NULL;

    char v_char = var[0];

    // Verifica se tem uma divisão e se ocorre indeterminação (0/0)
    if (f->rel == DIV && indeterminacao_zero_sobre_zero(f, target, v_char)) {
        
        //Aplica L'Hôpital: Deriva o numerador e o denominador separadamente
        Func *u = f->children[0];
        Func *v = f->children[1];

        Func *u_linha = derivar_func(u, var);
        Func *v_linha = derivar_func(v, var);

        //Cria uma nova árvore de divisão com as derivadas: (u' / v')
        Func *nova_divisao = criar_func(v_char, alocar_string("/"));
        nova_divisao->rel = DIV;
        nova_divisao->num_rel = 1;
        adicionar_filho(nova_divisao, u_linha);
        adicionar_filho(nova_divisao, v_linha);

        //Chama recursivamente o limite para a nova expressão derivada (L'Hôpital recursivo)
        return limite_func(nova_divisao, var, target, side);
    }

    //Se não há indeterminação imediata por L'Hôpital, avalia o comportamento lateral ou direto
    double ponto_avaliacao = target;

    if (side == LIMIT_LEFT) {
        ponto_avaliacao = target - EPSILON_LIM;
    } else if (side == LIMIT_RIGHT) {
        ponto_avaliacao = target + EPSILON_LIM;
    } else if (side == LIMIT_BOTH) {
        //Para LIMIT_BOTH, podemos conferir a média dos limites laterais ou valor direto se contínuo
        double val_esq = avaliar_func(f, target - EPSILON_LIM);
        double val_dir = avaliar_func(f, target + EPSILON_LIM);
        
        if (fabs(val_esq - val_dir) > 1e-4) {
            //Limites laterais diferentes: o limite bilateral não existe de forma unificada
            //Retorna um nó indicando divergência ou NULL conforme o padrão do seu motor
            return NULL; 
        }
        ponto_avaliacao = target;
    }

    //Avalia o valor numérico final no ponto ajustado
    double resultado_numerico = avaliar_func(f, ponto_avaliacao);

    //Retorna uma representação em forma de árvore Func com o valor resolvido
    char buffer_res[64];
    snprintf(buffer_res, sizeof(buffer_res), "%f", resultado_numerico);
    return mapear_func(buffer_res, v_char);
}


bool func_continua_em(const Func *f, double x0) {
    if (f == NULL) return false;

    //Avalia o valor exato da função no ponto x0
    double val_fx0 = avaliar_func(f, x0);

    //Se f(x0) for indeterminado ou infinito, a função não é contínua em x0
    if (isnan(val_fx0) || isinf(val_fx0)) {
        return false;
    }

    //Calcula os limites de aproximação pela esquerda (x0 - epsilon) e pela direita (x0 + epsilon)
    double x_esquerda = x0 - EPSILON_X;
    double x_direita  = x0 + EPSILON_X;

    double lim_esquerda = avaliar_func(f, x_esquerda);
    double lim_direita  = avaliar_func(f, x_direita);

    //Se algum dos limites laterais for indeterminado ou infinito
    if (isnan(lim_esquerda) || isinf(lim_esquerda) ||
        isnan(lim_direita)  || isinf(lim_direita)) {
        return false;
    }

    //Verifica se os limites laterais coincidem entre si
    if (fabs(lim_esquerda - lim_direita) > TOLERANCIA) {
        return false; // Descontinuidade de salto ou essencial
    }

    //Média dos limites laterais para estimar o valor limite
    double limite_estimado = (lim_esquerda + lim_direita) / 2.0;

    //Verifica se o limite é igual ao valor f(x0)
    if (fabs(limite_estimado - val_fx0) > TOLERANCIA) {
        return false; // Descontinuidade removível
    }

    return true; // A função é contínua em x0
}

#endif