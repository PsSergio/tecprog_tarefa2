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

static void printFunc(Func *f){
    printf("Var: %c\n", f->var);

    for(int i = 0 ; f->exp[i] != '\0'; i++){
        printf("%c", f->exp[i]);
    }

    printf("\n");

    print_rel(f->rel);

    printf("\nnc: %d\n--------------\n", f->num_children);

    for(int i = 0; i < f->num_children; i++){
        printFunc(f->children[i]);
    }
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
        case SOMA: { // (u+v)' = u' + v'

	        Func *u = f->children[0];
		    Func *v = f->children[1];
		
		    // Obtém as derivadas recursivamente: u' e v'
		    Func *u_linha = derivar_func(u, var);
		    Func *v_linha = derivar_func(v, var);
		
            sprintf(d->exp, "%s+%s", u_linha->exp, v_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);
            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;

        }
        case SUB: {
        //Mesmo processo que a soma, mas para subtração
        	Func *u = f->children[0];
		    Func *v = f->children[1];
		
		    Func *u_linha = derivar_func(u, var);
		    Func *v_linha = derivar_func(v, var);
		
            sprintf(d->exp, "%s-%s", u_linha->exp, v_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);
            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;
        }
        case MULT: //fórmula do produto: (u*v)' = u'*v + u*v' 
            {
	        Func *u = f->children[0];
	        Func *v = f->children[1];
	
	        Func *u_linha = derivar_func(u, var);
	        Func *v_linha = derivar_func(v, var);

            sprintf(d->exp, "((%s)*(%s))+((%s)*(%s))", u_linha->exp, v->exp, u->exp, v_linha->exp);
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
		
            sprintf(d->exp, "((%s)*(%s)-(%s)*(%s))/((%s)*(%s))", u_linha->exp, v->exp, u->exp, v_linha->exp, v->exp, v->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;

        }
        case SEN: // (sen(u))' = cos(u)*u'
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		
            sprintf(d->exp, "cos(%s)*(%s)", u->exp, u_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;
        }
        case COS: // (cos(u))' = -sen(u)*u'
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		
            sprintf(d->exp, "(-1)*(sen(%s)*(%s))", u->exp, u_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;
        }
        case TAN: // (tan(u))' = u' / (cos(u) * cos(u))
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		    
            sprintf(d->exp, "(%s)/((cos(%s))*(cos(%u)))", u->exp, u_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;
		    
        }
        case POT:
        {
            // (u^a)' = a*(u^(a-1))*u'  para 'a' constante - APENAS TRATAREMOS ESSE CASO ATÉ O MOMENTO
            // se u constante: (u^x)' = u^x * ln(u) NAO TRATAREMOS POR NAO TER AINDA A FUNCAO ln
            // se u funcao: ... NAO TRATAREMOS TBM

            Func *u = f->children[0];
		    Func *a = f->children[1];
            
            if(a->exp[0] == var) return NULL; 
		    Func *u_linha = derivar_func(u, var);
		
            sprintf(d->exp, "(%s)*((%s)^(%s-1))*%s", a->exp, u->exp, a->exp, u_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(alocar_string(d->exp), d->var);

            // printFunc(r);

            liberar_func(d);

            Func *s = (simplificar_func(r));

            // printFunc(s);

            liberar_func(r);
            return s;
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

// int main(){

//     Func *f = mapear_func(alocar_string("x^3"), 'x');
//     Func *s = derivar_func(f, 'x');

//     double res = avaliar_func(s, 2.0);
//     printf("\n\n%f", res);

//     liberar_func(f);
//     liberar_func(s);

//     return 0;

// }