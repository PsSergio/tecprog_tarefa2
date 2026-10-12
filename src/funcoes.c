#include "../include/funcoes.h"
#include <stdio.h>
#include <stdlib.h>

/* 
FUNÇÕES INTERNAS
*/

static Func* criar_func(char var, char* rel, char* exp){

    Func *f = malloc(sizeof *f);

    if(f == NULL) {
        printf("erro criar func");
        return NULL;
    }

    f->var = var;
    f->rel = rel;
    f->exp = exp;
    f->children = NULL;
    f->num_children = 0;

    return f;

}

static int adicionar_filho(Func *f, Func *child){
    int tamanho_vetor = f->num_children;
    Func **currentChildren = f->children;

    Func **tempChildren = realloc(currentChildren, tamanho_vetor+1);

    if(tempChildren == NULL){
        // free(currentChildren);
        printf("erro adicionar filho");
        return 1;
    }

    tempChildren[tamanho_vetor] = child;

    f->num_children++;
    f->children = tempChildren;

    return 0;
}

static void printFunc(Func *f){
    printf("Var: %c\n", f->var);

    for(int i = 0 ; f->exp[i] != '\0'; i++){
        printf("%c", f->exp[i]);
    }

    printf("\n");

    for(int i = 0 ; f->rel[i] != '\0'; i++){
        printf("%c", f->rel[i]);
    }

    printf("\nnc: %d\n--------------\n", f->num_children);

    for(int i = 0; i < f->num_children; i++){
        printFunc(f->children[i]);
    }
}


/* 
FUNÇÕES PÚBLICAS
*/

Func* mapear_func(const char *expr){
    return NULL;
}

void liberar_func(Func *f){

    if(f == NULL) return;
    
    for(int i = 0; i < f->num_children; i++){
        liberar_func(f->children[i]);
    }

    free(f->children);

    free(f);

}
Func* simplificar_func(const Func *f){
    return NULL;
}
double avaliar_func(const Func *f, double x){
    return 0;
}


/* 
FUNÇÃO MAIN - TESTE
*/

int main(){
    
    char exp[10] = "Teste 123";
    char rel[4] = {'+', '-', 'x'};
    char var = 'y';

    Func *f = criar_func(var, &rel, &exp);

    if(f == NULL) return 1;

    printFunc(f);

    char exp2[9] = "Teste 12";
    char rel2[2] = {'*'};
    char var2 = 'x';

    Func *c = criar_func(var2, &rel2, &exp2);

    if(c == NULL) return 1;

    int r = adicionar_filho(f, c);

    if(r == 1) return 1;

    printFunc(f);
    // printFunc(f->children[0]);

    // printf("\n%p\n%p\n\n", f, c);

    liberar_func(f);

    return 0;
}