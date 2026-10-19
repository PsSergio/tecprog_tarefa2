#include "../include/funcoes.h"
#include "../include/relacoes.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define expr_len 50000

// static void printFunc(Func *f){
//     printf("Var: %c\n", f->var);

//     for(int i = 0 ; f->exp[i] != '\0'; i++){
//         printf("%c", f->exp[i]);
//     }

//     printf("\n");

//     print_rel(f->rel);

//     printf("\nnc: %d\n--------------\n", f->num_children);

//     for(int i = 0; i < f->num_children; i++){
//         printFunc(f->children[i]);
//     }
// }

Func* derivar_func(Func *f, char var){
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
		
            sprintf(d->exp, "(%s+%s)", u_linha->exp, v_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(d->exp, d->var);
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
		
            sprintf(d->exp, "(%s-%s)", u_linha->exp, v_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(d->exp, d->var);
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
            
            Func *r = mapear_func(d->exp, d->var);

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
		
            sprintf(d->exp, "(((%s)*(%s)-(%s)*(%s))/((%s)*(%s)))", u_linha->exp, v->exp, u->exp, v_linha->exp, v->exp, v->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(d->exp, d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;

        }
        case SEN: // (sen(u))' = cos(u)*u'
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		
            sprintf(d->exp, "(cos(%s)*(%s))", u->exp, u_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(d->exp, d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;
        }
        case COS: // (cos(u))' = -sen(u)*u'
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		
            sprintf(d->exp, "((-1)*(sen(%s)*(%s)))", u->exp, u_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(d->exp, d->var);

            liberar_func(d);

            Func *s = simplificar_func(r);

            liberar_func(r);
            return s;
        }
        case TAN: // (tan(u))' = u' / (cos(u) * cos(u))
        {
		    Func *u = f->children[0];
		
		    Func *u_linha = derivar_func(u, var);
		    
            sprintf(d->exp, "((%s)/((cos(%s))*(cos(%s))))", u_linha->exp, u->exp, u->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(d->exp, d->var);

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
		
            sprintf(d->exp, "((%s)*((%s)^(%s-1))*(%s))", a->exp, u->exp, a->exp, u_linha->exp);
            d->exp = realloc(d->exp, strlen(d->exp)+1);
            
            Func *r = mapear_func(d->exp, d->var);

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

Func* reta_tangente_func(Func *f, double x0){
    if(f == NULL) return NULL;
    
    double y0 = avaliar_func(f, x0);

    if(!isfinite(y0) || isnan(y0)) return NULL;

    Func *derivada = derivar_func(f, f->var);

    double m = avaliar_func(derivada, x0);

    if(!isfinite(m) || isnan(m)) {
        // liberar_func(derivada);
        return NULL;
    }

    //Cria a string contendo a equação da reta: m * (x - x0) + y0
    char *expr = malloc(expr_len *sizeof *expr);
    sprintf(expr, "(%f)*(%c - %f) + %f", m, f->var, x0, y0);
    
    expr = realloc(expr, strlen(expr)+1);

    Func *reta_func = mapear_func(expr, f->var);

    return reta_func;
}
