#include "../include/funcoes.h"
#include "../include/relacoes.h"
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
    f->rel = DESCONHECIDO;
    f->num_rel = 0;
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

    printf("%d", f->rel);

    printf("\nnc: %d\n--------------\n", f->num_children);

    for(int i = 0; i < f->num_children; i++){
        printFunc(f->children[i]);
    }
}

/* 
 @brief limita uma string em um intervalo fechado-aberto [indice1, indice2)
 @param expr string a ser limitada
 @param inicio índice que representa o início do intervalo fechado
 @param fim índice que representa o fim do intervalo aberto
 @return retorna nova_expr, uma nova string limitada alocada na heap 
 @pre expr != NULL
 @pre [indice1, indice2) índices válidos
 @note essa função não libera a memória alocada da nova string
*/
static char* limitar_string(char *expr, int inicio, int fim){

    int tamanho_vetor = fim-inicio; // por causa do \0 no final

    char *nova_expr = malloc(tamanho_vetor * sizeof *nova_expr);

    if(nova_expr == NULL){
        printf("erro ao alocar");
        return NULL;
    }

    int c = 0;
    for(int i = inicio; i < fim; i++, c++){
        nova_expr[c] = expr[i];
    }

    return nova_expr;

}

static void encontrar_filhos(Func *f){
    // int *indices_filhos = verificar_parenteses(expr);

    if(f == NULL) return;

    int n = 0;
    int tamanho_expr = 0;
    
    char *expr = f->exp;
    for(tamanho_expr = 0; expr[tamanho_expr] != '\0'; tamanho_expr++);

    for(int i = tamanho_expr; i > 0; i--){ // percorre pela direita pela associatividade
        
        if(expr[i] == ')') n--;
        if(expr[i] == '(') n++;
        
        if(n < 0) continue;

        if(expr[i] == '+' || expr[i] == '-'){

            char *c_exp_r = limitar_string(expr, i+1, tamanho_expr);
            char *c_exp_l = limitar_string(expr, 0, i);

            Func *c_r = criar_func(f->var, c_exp_r);
            Func *c_l = criar_func(f->var, c_exp_l);

            adicionar_filho(f, c_l);
            adicionar_filho(f, c_r);

            enum RelationType rel = identificar_rel(&expr[i]);

            f->rel = rel;
            f->num_rel = 1;

            return;
        }
    }
}

/* 
FUNÇÕES PÚBLICAS
*/

Func* mapear_func(const char *expr, char var){

    Func *f = criar_func(var, expr);

    encontrar_filhos(f);

    for(int i = 0; i < f->num_children; i++){
        encontrar_filhos(f->children[i]);
    }

    return f;

}

void liberar_func(Func *f){

    if(f == NULL) return;
    
    for(int i = 0; i < f->num_children; i++){
        liberar_func(f->children[i]);
    }

    free(f->exp);

    free(f->children);

    free(f);

}

Func* simplificar_func(const Func *f){
    return NULL;
}

double avaliar_func(const Func *f, double x){
    return 0;
}


static char* alocar_string(char *expr){

    int i = 0;
    for(i = 0; expr[i] != '\0'; i++);
    char *new_expr = malloc(i* sizeof *new_expr);

    for(int j = 0; expr[j] != '\0'; j++){
        new_expr[j] = expr[j];
    }

    return new_expr;

}

/* 
FUNÇÃO MAIN - TESTE
*/

int main(){
    
    char *expr = alocar_string("(2x+3sen(x+2))+4x+2");

    Func *f = mapear_func(expr, 'x');
    // Func *f = mapear_func("(((2)*(x))+((3)*(sen(x+2))))*(((4)*(x))+(2))", 'x');
    printf("\n");

    printFunc(f);

    liberar_func(f);

    // enum RelationType type = identificar_rel("+");

    // printf("%d", type);

}