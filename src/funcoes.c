#include "../include/funcoes.h"
#include "../include/relacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define expr_len 256

/* 
FUNÇÕES INTERNAS
*/

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
        printf("%d", tamanho_vetor);
        return NULL;
    }

    int c = 0;
    for(int i = inicio; i < fim; i++, c++){
        nova_expr[c] = expr[i];
    }

    return nova_expr;

}

static char* juntar_strings(char *str1, char *str2){ // acho q pode melhorar a complexidade
    int i, j;
    for(i = 0; str1[i] != '\0'; i++);
    for(j = 0; str2[j] != '\0'; j++);
    int tamanho_f = i+j;
    char* r = malloc(tamanho_f * sizeof *r);
    for(int k = 0; k < str1[k] != '\0'; k++){
        r[k] = str1[k];
    }
    for(int k = 0; k < str2[k] != '\0'; k++){
        r[i+k] = str2[k];
    }

    return r;
}

static void retirar_parenteses(Func *f){
    char *expr = f->exp;
    int n = 0, i = 0;
    int quant_p = 0;

    if(expr[0] != '(') return;

    for(i = 0; expr[i] != '\0'; i++){
        
        if(expr[i] == '(') n++;
        if(expr[i] == ')') {
            if(n == 1) quant_p++;
            n--;
        }

        if(n < 0) continue;
        
    }

    if(quant_p == 1){

        if(expr[0] == '(' && expr[i-1] == ')'){
            char *new_expr = limitar_string(expr, 1, i-1);
            free(expr);
            f->exp = new_expr;
            retirar_parenteses(f); // caso haja casos como: ((...))
            return;
        }

    }

}

static Func* criar_func(char var, const char* exp){

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

    print_rel(f->rel);

    printf("\nnc: %d\n--------------\n", f->num_children);

    for(int i = 0; i < f->num_children; i++){
        printFunc(f->children[i]);
    }
}




/*
 @brief Busca grupos de expressões matemáticas seguindo as regras de associatividade e os adiciona em uma Func raiz
 @param f estrutura Func a qual vai ser adicionado os filhos, também de estrutura Func
 @note ao final da função, é esperado que f->children tenha sido preenchido completamente de forma recursiva
*/
static void encontrar_filhos(Func *f){

    if(f == NULL) return;

    retirar_parenteses(f);

    int n = 0;
    int tamanho_expr = 0;

    int rel_1 = -1;
    int rel_2 = -1;
    int rel_3 = -1;
    int rel_4 = -1;
    
    char *expr = f->exp;

    char *rel_tri = malloc(3*sizeof *rel_tri);
    
    for(tamanho_expr = 0; expr[tamanho_expr] != '\0'; tamanho_expr++);

    for(int i = tamanho_expr; i > 0; i--){ // percorre pela c2eita pela associatividade
        
        if(expr[i] == ')') n--;
        if(expr[i] == '(') n++;
        
        if(n < 0) continue;

        if( expr[i] == '+' || expr[i] == '-' ) {
            rel_1 = i; 
            break;
        }

        if( ( expr[i] == '*' || expr[i] == '/' ) && rel_2 == -1) rel_2 = i;

        if ( expr[i] == '^' && rel_4 == -1 && rel_2 == -1) rel_4 = i; 

        if( i-2 >= 0 ){
            if(expr[i-2] != 's' && expr[i-2] != 'c' && expr[i-2] != 't') continue;

            rel_tri[0] = expr[i-2]; rel_tri[1] = expr[i-1]; rel_tri[2] = expr[i];
            enum RelationType relt = identificar_rel(rel_tri);
            if(relt == SEN || relt == COS || relt == TAN){
                rel_3 = i-2;
            }
            // char *rel = expr[i-2] + expr[i-1] + expr[i];
            
        }
    }
    int i = -1;
    if( rel_1 != -1 ) i = rel_1;
    else if( rel_2 != -1) i = rel_2;
    else if( rel_3 != -1) {

        i = rel_3;
        char *c_exp_l = limitar_string(expr, i+3, tamanho_expr);

        Func *c_l = criar_func(f->var, c_exp_l);
        adicionar_filho(f, c_l);

        enum RelationType rel = identificar_rel(rel_tri);
        f->rel = rel;
        f->num_rel = 1;
    
        encontrar_filhos(c_l);
        return;
    }
    else if ( rel_4 != -1) i = rel_4;
    else return;

    free(rel_tri);
    
    char *c_exp_r = limitar_string(expr, i+1, tamanho_expr);
    char *c_exp_l = limitar_string(expr, 0, i);
    
    Func *c_r = criar_func(f->var, c_exp_r);
    Func *c_l = criar_func(f->var, c_exp_l);
    
    adicionar_filho(f, c_l);
    adicionar_filho(f, c_r);
    
    enum RelationType rel = identificar_rel(&expr[i]);
    f->rel = rel;
    f->num_rel = 1;
    
    encontrar_filhos(c_l);
    encontrar_filhos(c_r);

}   

Func* clonar_func(const Func *f){
    
    char* expr = alocar_string(f->exp);
    Func *n_f = criar_func(f->var, expr);
    n_f->rel = f->rel;

    for(int i = 0; i < f->num_children; i++){
        Func *c = clonar_func(f->children[i]);
        adicionar_filho(n_f, c);
    }

    return n_f;
    
}

static double is_number(Func *f){
    char *fim = NULL;
    if(f == NULL) return NAN;
    double v = strtod(f->exp, &fim);
    
    if(fim == f->exp) return NAN;
    return v;
}

static bool arvores_sao_iguais(const Func *c1, const Func *c2){

    if(c1 == NULL && c2 == NULL) return true;
    if(c1 == NULL || c2 == NULL) return false;
    if(c1->num_children != c2->num_children) return false;
    if(c1->rel != c2->rel) return false;

    if(c1->num_children == 0){
        retirar_parenteses(c1);
        retirar_parenteses(c2);
        return (strcmp(c1->exp, c2->exp) == 0);
    }

    for(int i = 0; i < c1->num_children; i++){
        bool r = arvores_sao_iguais(c1->children[i], c2->children[i]);
        if(!r) {
            return false;
        }
    }

    return true;

}

static Func* simplificar_func_uni(const Func *c, enum RelationType rel){
    char *expr = malloc(expr_len *sizeof *expr);

    Func *s = criar_func(c->var, expr);
    double v = 0.0;
    v = is_number(c);
    switch (rel)
    {
    case SEN:
        if(!isnan(v)){
            sprintf(s->exp, "%f", sin(v));
            break;
        }
        sprintf(s->exp, "sen(%s)", c->exp);
        s->rel = SEN;
        break;
    case COS:
        if(!isnan(v)){
            sprintf(s->exp, "%f", cos(v));
            break;
        }
        sprintf(s->exp, "cos(%s)", c->exp);
        s->rel = COS;
        break;
    case TAN:
        if(!isnan(v)){
            sprintf(s->exp, "%f", tan(v));
            break;
        }
        sprintf(s->exp, "tan(%s)", c->exp);
        s->rel = TAN;
        break;
    default:
        break;
    }
    s->exp = realloc(s->exp, strlen(s->exp)+1);
    return s;
}

static Func* simplificar_func_bi(const Func *c1, const Func *c2, enum RelationType rel){
    char *expr = malloc(expr_len *sizeof *expr);
    Func *s = criar_func(c1->var, expr);
    s->rel = DESCONHECIDO;

    double v1 = 0.0, v2 = 0.0;
    v1 = is_number(c1); v2 = is_number(c2);

    switch (rel)
    {
    case SOMA:
        if(!isnan(v1) && !isnan(v2)){
            sprintf(s->exp, "%f", v1+v2);
            break;
        }
        else if(isnan(v1)){
            if(v2 == 0.0){
                sprintf(s->exp, "%s", c1->exp);
                break;
            }
        }
        else if(isnan(v2)){
            if(v1 == 0.0){
                sprintf(s->exp, "%s", c2->exp);
                break;
            }
        }
        sprintf(s->exp, "(%s)+(%s)", c1->exp, c2->exp);
        adicionar_filho(s, c1);
        adicionar_filho(s, c2);
        s->rel = SOMA;
        break;
    case SUB:
        if(!isnan(v1) && !isnan(v2)){ // ambos números
            sprintf(s->exp, "%f", v1-v2);
            break;
        }
        else if(isnan(v1)){ // v1 é x
            
            if(v2 == 0.0){ // v2 é 0
                sprintf(s->exp, "%s", c1->exp);
                break;
            }else if(!isnan(v2)) { // v2 é número

                sprintf(s->exp, "(%s)-(%f)", c1->exp, v2);
                adicionar_filho(s, c1);
                adicionar_filho(s, c2);
                s->rel = SUB;
                break;
            }
        }
        else if(isnan(v2)){
            if(v1 == 0.0){
                sprintf(s->exp, "-%s", c2->exp);
                break;
            }else if(!isnan(v1)) {
                sprintf(s->exp, "(%f)-(%s)", v1, c2->exp);
                adicionar_filho(s, c1);
                adicionar_filho(s, c2);
                s->rel = SUB;
                break;
            }
        }
        
        if((c1->num_children == 0 && c2->num_children == 0) || arvores_sao_iguais(c1,c2)) {
            // fazer func pra comparar arvores
            // temos q simplificar (x+2)-(x+2) por exemplo
            sprintf(s->exp, "%f", 0.0);
        }
        else {
            sprintf(s->exp, "(%s)-(%s)", c1->exp, c2->exp);
            adicionar_filho(s, c1);
            adicionar_filho(s, c2);
            s->rel = SUB;
        }
        break;
    case MULT:
        if(!isnan(v1) && !isnan(v2)){ // ambos números
            sprintf(s->exp, "%f", v1*v2);
            break;
        }
        else if(isnan(v1)){
            if(v2 == 0.0){
                sprintf(s->exp, "%f", 0.0);
                break;
            }else if(v2 == 1.0){
                sprintf(s->exp, "%s", c1->exp);
                break;
            }
        }else if(isnan(v2)){
            if(v1 == 0.0){
                sprintf(s->exp, "%f", 0.0);
                break;
            }else if(v1 == 1.0){
                sprintf(s->exp, "%s", c2->exp);
                break;
            }
        }
        sprintf(s->exp, "(%s)*(%s)", c1->exp, c2->exp);
        adicionar_filho(s, c1);
        adicionar_filho(s, c2);
        s->rel = MULT;
        break;
    case DIV:
        if(!isnan(v1) && !isnan(v2)){
            if(v1 == 0.0 && v2 != 0.0){
                sprintf(s->exp, "%f", 0.0);

                break;
            }else if(v1 != 0.0 && v2 != 0.0){
                sprintf(s->exp, "%f", v1/v2);

                break;
            }
        }
        else if(isnan(v1)){
            if(v2 == 1.0){
                sprintf(s->exp, "%s", c1->exp);

                break;
            }
        }else if(isnan(v2)){
            if(v1 == 0.0){
                sprintf(s->exp, "%f", 0.0);
                break;
            }
        }
        if((c1->num_children == 0 && c2->num_children == 0) || arvores_sao_iguais(c1, c2)){ // fazer func pra comparar arvores
            // temos q simplificar (x+2)/(x+2) por exemplo
            sprintf(s->exp, "%f", 1.0);
            break;
        }
        sprintf(s->exp, "(%s)/(%s)", c1->exp, c2->exp);
        adicionar_filho(s, c1);
        adicionar_filho(s, c2);
        s->rel = DIV;
        break;
    case POT:
        if(!isnan(v1) && !isnan(v2)){
            sprintf(s->exp, "%f", pow(v1,v2));
            break;
        }else if(isnan(v1)){
            if(v2 == 0.0){
                sprintf(s->exp, "%f", 1.0);
                break;
            }else if(v2 == 1.0){
                sprintf(s->exp, "%s", c1->exp);
                break;
            }
        }else if(isnan(v2)){
            if(v1 == 1.0){
                sprintf(s->exp, "%f", 1.0);
                break;
            }
        }

        sprintf(s->exp, "(%s)^(%s)", c1->exp, c2->exp);
        adicionar_filho(s, c1);
        adicionar_filho(s, c2);
        s->rel = POT;
        break;
    default:
        break;
    }
    s->exp = realloc(s->exp, strlen(s->exp)+1);
    
    return s;
}

/* 
FUNÇÕES PÚBLICAS
*/

/* 
 @brief Mapeia uma expressão matemática, recebida como string, para uma estrutura Func
 @param expr Expressão matemática a ser mapeada
 @param var um character que representa a incógnita da equação
 @return Retorna a estrutura Func raiz da equação. Esta possui no máximo dois filhos, também de estrutura Func.
 @note O retorno deve ser liberado da Heap pela função liberar_func após uso
*/
Func* mapear_func(char *expr, char var){

    Func *f = criar_func(var, expr);

    encontrar_filhos(f);

    return f;

}

/*
 @brief Libeara da Heap uma estrutura Func e todos seus filhos de forma recursiva
 @param f estrutura Func a ser liberada da memória
*/
void liberar_func(Func *f){

    if(f == NULL) return;
    
    for(int i = 0; i < f->num_children; i++){
        liberar_func(f->children[i]);
    }

    free(f->exp);

    free(f->children);

    free(f);

}

/* 
 @brief Simplifica uma equação mapeada com base em regras matemáticas básicas
 @param f Função mapeada a ser simplificada
 @return Retorna a nova função simplificada, também mapeada
 @note A função não libera Func f
 @note Caso a Func f não possa ser simplificada, a função retorna uma cópia
*/
Func* simplificar_func(const Func *f){
    if(f == NULL) return NULL;
    
    if(f->num_children == 0){
        return clonar_func(f);
    }else if(f->num_children == 1){

        const Func *c = simplificar_func(f->children[0]);
        return simplificar_func_uni(c, f->rel);

    }

    const Func *c1 = simplificar_func(f->children[0]);
    const Func *c2 = simplificar_func(f->children[1]);
    
    return simplificar_func_bi(c1, c2, f->rel);
    
}

/* 
 @brief avalia o valor numérico de uma equação mapaeada no ponto x
 @param f Função raiz mapeada por mapear_func
 @param x Ponto x em que f será calculado
 @return Retorna o valor numérico calculado no ponto x
 @note O retorno pode ser numérico, NAN ou INFINITY
*/
double avaliar_func(const Func *f, double x){
    if(f == NULL) return NAN;

    if(f->num_children == 0){
        if(f->exp[0] == f->var && f->exp[1] == '\0') return x;
        if(f->exp[0] == '-' && f->exp[1] == f->var &&  f->exp[2] == '\0') return -x;
        if(f->exp[0] == '+' && f->exp[1] == f->var &&  f->exp[2] == '\0') return x;
        double x3 = atof(f->exp);
        
        return x3;
    }

    if(f->num_children == 1){
        double x1 = avaliar_func(f->children[0], x );
        double x3 = aplicar_relacao(x1, 0, f->rel);

        return x3;

    }

    // num_children == 2
    double x1 = avaliar_func(f->children[0], x);
    double x2 = avaliar_func(f->children[1], x);
    double x3 = aplicar_relacao(x1, x2, f->rel);
    return x3;
}



/* 
FUNÇÃO MAIN - TESTE


int main(){

    // char *expr = alocar_string("(2*x+3*sen(x+2))*(4*x*2)");
    char *expr = alocar_string("(sen(x)+3)/((cosx)^3+5*x)");

    // char *expr = alocar_string("(x+x-x)*10");
    // char *expr = alocar_string("(x+x)-(x+x)"); // corrigir
    // char *expr = alocar_string("x+x-x");


    Func *f = mapear_func(expr, 'x');
    
    // printf("\n");

    // printFunc(f);

    Func *s = simplificar_func(f);

    // Func *s = clonar_func(f);
    
    // printFunc(s);

    printf("\n");

    // double x = avaliar_func(f, 1);
    // printf("%f", x);
    
    
    liberar_func(f);
    liberar_func(s);

    // Func *f = criar_func('x', expr);

    // retirar_parenteses(f);

    // printFunc(f);

    // enum RelationType type = identificar_rel("+");

    // printf("%d", type);

}

*/