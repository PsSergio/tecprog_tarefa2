#include "../include/funcoes.h"
#include "../include/relacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define expr_len 256
static char* alocar_string(char *expr){

    int i = 0;
    for(i = 0; expr[i] != '\0'; i++);
    char *new_expr = malloc((i+1)* sizeof *new_expr);

    for(int j = 0; expr[j] != '\0'; j++){
        new_expr[j] = expr[j];
    }

    new_expr[i] = '\0';

    return new_expr;

}

Func* derivar_func(const Func *f, const char var){
    if (f == NULL) return NULL;

    if(f->num_children == 0){

        if(f->exp[0] == var){
            char *expr = alocar_string("1");
            Func *d = criar_func(f->var, expr);
            return d;
        }

        char *expr = alocar_string("0");
        Func *d = criar_func(f->var, expr);
        return d;
        
    }

    char *expr = malloc(expr_len *sizeof *expr);
    Func *d = criar_func(f->var, expr);

    switch (f->rel) {
        case SOMA: {

	        Func *u = f->children[0];
		    Func *v = f->children[1];
		
		    // Obtém as derivadas recursivamente: u' e v'
		    Func *u_linha = derivar_func(u, var);
		    Func *v_linha = derivar_func(v, var);
		
		    //Cria o novo nó raiz da SOMA (+)
		    Func *nova_raiz = criar_func(f->var, alocar_string("+"));
		    nova_raiz->rel = SOMA;
		    nova_raiz->num_rel = 1;
		
		    //Adiciona diretamente as derivadas já calculadas como filhos
		    adicionar_filho(nova_raiz, u_linha); //Primeiro filho agora é u'
		    adicionar_filho(nova_raiz, v_linha); //O segundo filho agora é v'
	
	    	return nova_raiz;
            break;
        }
        case SUB: {
        //Mesmo processo que a soma, mas para subtração
        	Func *u = f->children[0];
		    Func *v = f->children[1];
		
		    Func *u_linha = derivar_func(u, var);
		    Func *v_linha = derivar_func(v, var);
		
		    Func *nova_raiz = criar_func(f->var, alocar_string("-"));
		    nova_raiz->rel = SUB;
		    nova_raiz->num_rel = 1;
		
		    adicionar_filho(nova_raiz, u_linha);
		    adicionar_filho(nova_raiz, v_linha);
	
	    	return nova_raiz;
            break;
        }
        case MULT: //fórmula do produto: (u*v)' = u'*v + u*v' 
            {
	        Func *u = f->children[0];
	        Func *v = f->children[1];
	
	        Func *u_linha = derivar_func(u, var);
	        Func *v_linha = derivar_func(v, var);

            sprintf(d->exp, "(%s*%s)+(%s*%s)", u_linha->exp, v->exp, u->exp, v_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;
        }
        case DIV: //fórmula do quociente: (u/v)' = (u'*v - u*v') / (v*v)
            //Identificar u (numerador) e v (denominador)
        {
		    Func *u = f->children[0];
		    Func *v = f->children[1];
		
		    Func *u_linha = derivar_func(u, var);
		    Func *v_linha = derivar_func(v, var);
		
		    //Cópias seguras e independentes de u e v para reutilização nos termos cruzados
		    Func *u_copia  = mapear_func(alocar_string(u->exp), f->var);
		    Func *v_copia1 = mapear_func(alocar_string(v->exp), f->var);
		    Func *v_copia2 = mapear_func(alocar_string(v->exp), f->var);
		    Func *v_copia3 = mapear_func(alocar_string(v->exp), f->var);
		
		    //Construir o primeiro termo do numerador: (u' * v)
		    Func *termo_esq = criar_func(f->var, alocar_string("u'*v"));
		    termo_esq->rel = MULT;
		    termo_esq->num_rel = 1;
		    adicionar_filho(termo_esq, u_linha);
		    adicionar_filho(termo_esq, v_copia1);
		
		    //Construir o segundo termo do numerador: (u * v')
		    Func *termo_dir = criar_func(f->var, alocar_string("u*v'"));
		    termo_dir->rel = MULT;
		    termo_dir->num_rel = 1;
		    adicionar_filho(termo_dir, u_copia);
		    adicionar_filho(termo_dir, v_linha);
		
		    //Construir o Numerador completo como uma SUBTRAÇÃO (-)
		    Func *numerador = criar_func(f->var, alocar_string("-"));
		    numerador->rel = SUB;
		    numerador->num_rel = 1;
		    adicionar_filho(numerador, termo_esq);
		    adicionar_filho(numerador, termo_dir);
		
		    //Construir o Denominador (v^2 representado como v * v)
		    Func *denominador = criar_func(f->var, alocar_string("v*v"));
		    denominador->rel = MULT;
		    denominador->num_rel = 1;
		    adicionar_filho(denominador, v_copia2);
		    adicionar_filho(denominador, v_copia3);
		
		    //Criar a raiz final da Divisão (/) unindo o Numerador e o Denominador
		    Func *nova_raiz = criar_func(f->var, alocar_string("/"));
		    nova_raiz->rel = DIV;
		    nova_raiz->num_rel = 1;
		    adicionar_filho(nova_raiz, numerador);
		    adicionar_filho(nova_raiz, denominador);
		
		    return nova_raiz;
            break;
        }
        case SEN:
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		
		    //Cria a sub-árvore para o cos(u) utilizando o mapeamento
		    // (Aproveitando a string "cos(" + u->exp + ")" ou recriando a expressão cos(u))
		    char *expr_cos = malloc(snprintf(NULL, 0, "cos(%s)", u->exp) + 1);
		    sprintf(expr_cos, "cos(%s)", u->exp);
		    Func *termo_cos = mapear_func(expr_cos, f->var);
		    free(expr_cos);
		
		    //Cria o novo nó raiz que será uma MULTIPLICAÇÃO (*) cos(u) * u'
		    Func *nova_raiz = criar_func(f->var, alocar_string("*"));
		    nova_raiz->rel = MULT;
		    nova_raiz->num_rel = 1;
		
		    // Adiciona os dois filhos na multiplicação: (cos(u)) e (u')
		    adicionar_filho(nova_raiz, termo_cos);
		    adicionar_filho(nova_raiz, u_linha);
		
		    return nova_raiz;
        	break;
        }
        case COS: //mesmo processo que o seno, mas com a diferença do sinal negativo
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		
		    char *expr_sen = malloc(snprintf(NULL, 0, "-1*sen(%s)", u->exp) + 1);
		    sprintf(expr_sen, "-1*sen(%s)", u->exp);
		    Func *termo_sen = mapear_func(expr_sen, f->var);
		    free(expr_sen);
		
		    Func *nova_raiz = criar_func(f->var, alocar_string("*"));
		    nova_raiz->rel = MULT;
		    nova_raiz->num_rel = 1;
		
		    adicionar_filho(nova_raiz, termo_sen);
		    adicionar_filho(nova_raiz, u_linha);
		
		    return nova_raiz;
        	break;
        }
        case TAN: //fórmula da tangente: (tan(u))' = u' / (cos(u) * cos(u))
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		
		    Func *u_copia1 = mapear_func(alocar_string(u->exp), f->var);
		    Func *u_copia2 = mapear_func(alocar_string(u->exp), f->var);
		
		    //Constroi o primeiro cos(u)
		    char *expr_cos1 = malloc(snprintf(NULL, 0, "cos(%s)", u_copia1->exp) + 1);
		    sprintf(expr_cos1, "cos(%s)", u_copia1->exp);
		    Func *termo_cos1 = mapear_func(expr_cos1, f->var);
		    free(expr_cos1);
		    liberar_func(u_copia1); // Limpa a cópia temporária do argumento
		
		    //Constroi o segundo cos(u)
		    char *expr_cos2 = malloc(snprintf(NULL, 0, "cos(%s)", u_copia2->exp) + 1);
		    sprintf(expr_cos2, "cos(%s)", u_copia2->exp);
		    Func *termo_cos2 = mapear_func(expr_cos2, f->var);
		    free(expr_cos2);
		    liberar_func(u_copia2); // Limpa a cópia temporária do argumento
		
		    // Denominador: uma MULTIPLICAÇÃO: cos(u) * cos(u)
		    Func *denominador = criar_func(f->var, alocar_string("cos(u)*cos(u)"));
		    denominador->rel = MULT;
		    denominador->num_rel = 1;
		    adicionar_filho(denominador, termo_cos1);
		    adicionar_filho(denominador, termo_cos2);
		
		    //Cria a raiz final como uma DIVISÃO (/): u' / (cos(u) * cos(u))
		    Func *nova_raiz = criar_func(f->var, alocar_string("/"));
		    nova_raiz->rel = DIV;
		    nova_raiz->num_rel = 1;
		    adicionar_filho(nova_raiz, u_linha);
		    adicionar_filho(nova_raiz, denominador);
		
		    return nova_raiz;
        	break;
        }
        default:
            break;
    }
    return NULL;
}

Func* reta_tangente_func(const Func *f, double x0){
    //Calcula o valor de y no ponto x0
    double y0 = avaliar_func(f, x0);

    //Calcula a derivada numérica m = f'(x0) usando o método das diferenças finitas
    double h = 0.00001;
    double m = (avaliar_func(f, x0 + h) - avaliar_func(f, x0 - h)) / (2 * h);

    //Cria a string contendo a equação da reta: m * (x - x0) + y0
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "(%f)*(%c - %f) + %f", m, f->var, x0, y0);

    int len = 0;
    for(len = 0; buffer[len] != '\0'; len++);
    char *expr_reta = malloc((len + 1) * sizeof(*expr_reta));
    for(int i = 0; i <= len; i++) expr_reta[i] = buffer[i];

    //Mapeia a string gerada para uma nova árvore Func independente
    Func *reta_func = mapear_func(expr_reta, f->var);

    return reta_func;
}

int main(){

    Func *f = mapear_func(alocar_string("2*x*x"), 'x');
    Func *s = derivar_func(f, 'x');

    printf("%s", s->exp);

    liberar_func(f);
    liberar_func(s);

    return 0;

}