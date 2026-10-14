#include "../include/funcoes.h"
#include <stdio.h>
#include <stdlib.h>

/* 
FUNÇÕES INTERNAS
*/

static Func* criar_func(char var, char* exp){

    Func *f = malloc(sizeof(*f));

    if(f == NULL) {
        printf("erro criar func");
        return NULL;
    }

    f->var = var;
    f->rel = NULL;
    f->exp = exp;
    f->children = NULL;
    f->num_children = 0;

    return f;

}

/* 
 @brief adiciona um elemento da estrutura Func no vetor f->children
 @param f estrutura Func
 @param child estrutura Func a ser adicionada em f->chilren
 @return retorna 0 se ocorreu com sucesso, 1 caso contrário
 @note essa função não libera os elementos alocados
*/
static int adicionar_filho(Func *f, Func *child){
    int tamanho_vetor = f->num_children;
    Func **currentChildren = f->children;

    Func **tempChildren = realloc(currentChildren, (tamanho_vetor+1) * sizeof(**tempChildren));

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
 @brief retorna os índices da string que contém parênteses
 @param expr expressão matemática em formato de string
 @return Vetor indices de índices dos parênteses, com indices[0] sendo o tamanho do vetor
 @pre expr != NULL
 @note a função não libera indices
*/
static int* verificar_parenteses(char *expr){

    int aberto = 0;

    int *indices = malloc(sizeof(*indices));

    if(indices == NULL){
        printf("erro ao alocar");
        return NULL;
    }
    int tamanho_vetor = 1;
    indices[0] = tamanho_vetor;
    for(int i = 0; expr[i] != '\0'; i++){
        if(expr[i] == '(' || expr[i] == ')'){

            // vamos ignorar parenteses internos
            if(expr[i] == '(') aberto++;
            if(expr[i] == ')') aberto--;

            if(expr[i] == '(' && aberto != 1) continue;
            if(expr[i] == ')' && aberto != 0) continue;

            int *vetorTemp = realloc(indices, (tamanho_vetor+1)*sizeof(*indices));
            if(vetorTemp == NULL) {
                printf("Erro ao realocar");
                continue;
            }

            tamanho_vetor++;

            indices = vetorTemp;
            indices[tamanho_vetor-1] = i;

        }
    }

    if(tamanho_vetor == 1) return NULL;
    indices[0] = tamanho_vetor;
    return indices;
}


/* 
 @brief limita uma string em um intervalo aberto (indice1, indice2)
 @param expr string a ser limitada
 @param indice1 índice que representa o início do intervalo aberto
 @param indice2 índice que representa o fim do intervalo aberto
 @return retorna nova_expr, uma nova string limitada alocada na heap 
 @pre expr != NULL
 @pre (indice1, indice2) índices válidos
 @note essa função não libera a memória alocada da nova string
*/
static char* limitar_string(char *expr, int indice1, int indice2){

    int tamanho_vetor = indice2-indice1; // por causa do \0 no final

    char *nova_expr = malloc(tamanho_vetor * sizeof *nova_expr);

    if(nova_expr == NULL){
        printf("erro ao alocar");
        return NULL;
    }

    int c = 0;
    for(int i = indice1+1; i < indice2; i++, c++){
        nova_expr[c] = expr[i];
    }

    return nova_expr;

}

static char* adicionar_relacoes(char *expr, int *indices){
    char *rel = malloc(sizeof(*rel));

    if(indices == NULL) return rel;
    
    int tamanho_rel = 0;

    int tamanho_ind = indices[0];

    for(int i = 1; i < tamanho_ind; i+=2){

        if(i >= tamanho_ind-1) break;

        // printf(" %c ", expr[i+1]);
        
        char *relTemp = realloc(rel, (tamanho_rel+1)*sizeof(*rel));
        if(relTemp == NULL) continue;
        tamanho_rel++;

        rel = relTemp;
        rel[tamanho_rel-1] = expr[indices[i+1]+1];
        
    }

    if(tamanho_rel == 0) return NULL;
    return rel;

}


/* 
FUNÇÕES PÚBLICAS
*/

Func* mapear_func(const char *expr, char var){

    Func *f = criar_func(var, expr);

    if(f == NULL) return NULL;

    int *indices = verificar_parenteses(expr);
    char *rel = adicionar_relacoes(expr, indices);
    f->rel = rel;
    if(indices == NULL) return f;

    int tamanho_vetor = indices[0];


    for(int i = 1; i < tamanho_vetor; i+=2){
        printf("%c %c", expr[indices[i]], expr[indices[i+1]]);
        char *c = limitar_string(expr, indices[i], indices[i+1]);
        Func *child = mapear_func(c, var);
        if(child == NULL){
            break;
        }

        adicionar_filho(f, child);
    }

    return f;

}

void liberar_func(Func *f){

    if(f == NULL) return;
    
    for(int i = 0; i < f->num_children; i++){
        liberar_func(f->children[i]);
    }

    free(f->rel);

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
    
    char exp[22] = "(2x+3sen(x+2))/(4x+2)";

    // Func *f = mapear_func("(2x+3sen(x+2))*(4x+2)", 'x');
    Func *f = mapear_func("(((2)*(x))+((3)*(sen(x+2))))*(((4)*(x))+(2))", 'x');
    printf("\n");

    printFunc(f);

    liberar_func(f);

}