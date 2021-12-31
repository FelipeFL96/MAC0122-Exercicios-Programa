/******************************************/
/**                                      **/
/**  Felipe Fernandes de Lima    9345321 **/
/**  Exercício-Programa 02               **/
/**  Professor: Carlos Eduardo Ferreira  **/
/**  Turma: 50                           **/
/**                                      **/
/******************************************/

#include <stdio.h>
#include <stdlib.h>

/*Definindo bloco*/
/*Conjunto de espaços do tabuleiro capaz de receber uma palavra*/
typedef struct{
    char TipoDaFila; /*l se o bloco for em uma linha, c se for em uma coluna*/
    int fila;        /*Linha ou coluna em que o bloco está*/
    int ini;
    int fim;
}bloco;

/*Definindo a pilha (tipo: bloco)*/
typedef struct{
    bloco *v;
    int topo;
    int max;
}pilha;

/*Funções da Pilha*/
void criaPilha(pilha*, int);
void destroiPilha(pilha*);
int pilhaVazia(pilha*);
bloco TopoDaPilha(pilha);
void empilha(bloco, pilha*);
bloco desempilha(pilha*);

/*Funções para a entrada*/
int** geraTabuleiro(int, int);
char** alocaPalavras(int, int);

/*Funções para a operação*/
char** encaixaPalavras(char**, int, int**, int, int);
int cabe(char*, char**, bloco);
void encaixa(char*, char**, int**, bloco);
void desfazEncaixe(int**, char**, bloco);

/*Funções auxiliares*/
void imprimeTabela(char**, int, int);
int max(int, int);

int main(){
    int i, j, m, n, p, tam, **tab, contador;
    char **palavras, **solucao;

    printf("EP2 - Inventando palavras cruzadas:\n");
    printf("\nInsira os dados:\n\n");
    scanf("%d %d", &m, &n);
    solucao = 0;
    contador = 1;

    while((m != 0)&&(n != 0)){
        tab = geraTabuleiro(m, n);
        scanf("%d", &p);
        tam = max(m, n);
        palavras = alocaPalavras(p, tam);

        printf("\nInstancia %d\n", contador);

        if(palavras)
            solucao = encaixaPalavras(palavras, p, tab, m, n);

        if(solucao)
            imprimeTabela(solucao, m, n);
        else
            printf("Nao ha solucao!\n");

        printf("\n\nInsira os dados:\n\n");
        scanf("%d %d", &m, &n);

        contador++;

        /*Desalocando memória*/
        for(i = 0; i < m; i++){
            free(tab[i]);
            free(solucao[i]);
        }
        free(tab);
        free(solucao);
        for(i = 0; i < p; i++)
            free(palavras[i]);
        free(palavras);
    }

    return 0;
}

/*FUNÇÕES DA PILHA*/

void criaPilha(pilha *P, int tam){
    int i;
    P->v = (bloco*)malloc(tam*sizeof(bloco));
    for(i = 0; i < tam; i++){
        P->v[i].fila = 0;
        P->v[i].ini = 0;
        P->v[i].fim = 0;
        P->v[i].TipoDaFila = 0;
    }
    P->topo = 0;
    P->max = tam;
}

void destroiPilha(pilha *P){
    P->max = 0;
    P->topo = 0;
    free(P->v);
}

int pilhaVazia(pilha *P){
    if(P->topo == 0)
        return 1;
    return 0;
}

bloco TopoDaPilha(pilha P){
    return(P.v[P.topo - 1]);
}

void empilha(bloco x, pilha *P){
    if(P->topo == P->max){
        printf("Xiii... Pilha cheia!");
        return;
    }
    P->v[P->topo] = x;
    P->topo++;
}

bloco desempilha(pilha *P){
    if(pilhaVazia(P)){
        printf("Pilha vazia!");
    }
    else{
        P->topo--;
        return(P->v[P->topo]);
    }
}

/*FUNÇÕES PARA A ENTRADA*/

/*Função que recebe os dados do tamanho de uma matriz e devolve para um
ponteiro duplo uma matriz tabuleiro.
Ela também faz a leitura dos valores que preenchem essa matriz*/
int** geraTabuleiro(int m, int n){
    int **tab, i, j;

    /*Gerando a matriz tabuleiro (m X n)*/
    tab = (int**)malloc(m*sizeof(int*));
    for(i = 0; i < m; i++){
        tab[i] = (int*)malloc(n*sizeof(int));
    }

    /*Recebendo o preenchimento do tabuleiro*/
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            scanf("%d", &tab[i][j]);
        }
    }

    return tab;
}

/*Função que recebe os dados do tamanho de uma matriz e devolve para um
ponteiro duplo a matriz com as palavras da instância. Ela armazena a leitura
das palavras em um vetor e transfere seus caracteres para a matriz*/
char** alocaPalavras(int pal, int let){
    int i, j, ok;
    char **Palavras;
    char *palavra;
    ok = 1;

    /*Gerando uma variável sring para leitura das palavras*/
    palavra = (char*)malloc((let+2)*sizeof(char));
    for(i = 0; i < (let+2); i++) palavra[i] = 0;

    /*Gerando uma Matriz (pal X let+1) para as palavras*/
    Palavras = (char**)malloc(pal*sizeof(char*));
    for(i = 0; i < pal; i++){
        Palavras[i] = (char*)malloc((let+1)*sizeof(char));
        for(j = 0; j < (let+1); j++)
            Palavras[i][j] = 0;
    }

    /*Gravando cada palavra em uma linha da matriz*/
    for(i = 0; i < pal; i++){
        scanf("%s", palavra);
        for(j = 0; palavra[j] != 0; j++){
            if(j > (let-1)){
                free(palavra);
                ok = 0;
            }
            Palavras[i][j] = palavra[j];
        }
    }

    /*Caso haja problema em alguma palavra*/
    if(!ok) return 0;

    free(palavra);
    return Palavras;
}

/*FUNÇÕES PARA A OPERAÇÃO*/

/*Função que gera a matriz solução do problema e, através da estratégia backtracking
 em uma pilha de blocos, procura preenchê-la com as strings da matriz das palavras*/
char** encaixaPalavras(char** palavras, int numPal, int** tab, int m, int n){
    char **solucao;
    int p, i, j, ok;
    bloco atual;
    pilha blocos;

    /*Gerando pilha*/
    criaPilha(&blocos, numPal);

    /*Gerando a matriz solução (m X n)*/
    solucao = (char**)malloc(m*sizeof(char*));
    for(i = 0; i < m; i++){
        solucao[i] = (char*)malloc(n*sizeof(char));
        for(j = 0; j < n; j++)
            solucao[i][j] = 0;
    }

    /*Inicialização do bloco atual*/
    atual.fim = 0;
    atual.fila = 0;
    atual.TipoDaFila = 'l'; /*As primeiras tentativas de encaixe serão nas linhas*/

    p = 0;
    while(p < numPal){
        ok = 0;

        /*Testando encaixe nas linhas*/
        if(atual.TipoDaFila == 'l'){
            for(i = atual.fila; i < m; i++){
                atual.fila = i;
                j = atual.fim;
                while(j < n){
                    /*Identificando bloco para uma palavra*/
                    while((j < n)&&(tab[i][j] == (-1)))
                        j++;
                    atual.ini = j;
                    while((j < n)&&(tab[i][j] != (-1)))
                        j++;
                    atual.fim = j;

                    if(cabe(palavras[p], solucao, atual)){
                        encaixa(palavras[p], solucao, tab, atual);
                        ok = 1;
                        break;
                    }
                    else
                        j++;
                }
                if(ok) break;
                else atual.fim = 0;
            }
            if(!ok){
                atual.TipoDaFila = 'c';
                atual.fila = 0;
            }
        }

        /*Testando encaixe nas colunas*/
        if(atual.TipoDaFila == 'c'){
            for(j = atual.fila; j < n; j++){
                atual.fila = j;
                i = atual.fim;
                while(i < m){
                    /*Identificando bloco para uma palavra*/
                    while((i < m)&&(tab[i][j] == (-1)))
                        i++;
                    atual.ini = i;
                    while((i < m)&& (tab[i][j] != (-1)))
                        i++;
                    atual.fim = i;

                    if(cabe(palavras[p], solucao, atual)){
                        encaixa(palavras[p], solucao, tab, atual);
                        ok = 1;
                        break;
                    }
                    else
                        i++;
                }
            if(ok) break;
            else atual.fim = 0;
            }
        }

        /*Implementação do backtrack*/
        if(ok){
            empilha(atual, &blocos);
            p++;
            /*Reinicialização do bloco atual*/
            atual.TipoDaFila = 'l';
            atual.fila = 0;
            atual.fim = 0;
        }
        else{
            if(pilhaVazia(&blocos))
                return 0;

            atual = desempilha(&blocos);
            desfazEncaixe(tab, solucao, atual);
            p--;
        }
    }

    /*Completando a matriz de saída com '*' em lugar de 0s*/
    for(i = 0; i < m; i++)
        for(j = 0; j < n; j++)
            if(solucao[i][j] == 0)
                solucao[i][j] = '*';

    return solucao;
}

/*Função que recebe um vetor com uma palavra, a matriz com a solução e um bloco
e responde se é possível encaixar a palavra no dado bloco da matriz solução*/
int cabe(char *palavra, char **solucao, bloco b){
    int k, tam_palavra, tam_bloco, ok;
    ok = 0;

    /*Comparando tamanhos do bloco e da palavra*/
    tam_bloco = b.fim - b.ini;
    for(tam_palavra = 0; palavra[tam_palavra] != 0; tam_palavra++);
    if(tam_bloco == tam_palavra)
        ok = 1;

    /*Testando casas já preenchidas no bloco linha*/
    if(b.TipoDaFila == 'l'){
        if(ok){
            for(k = 0; k < tam_bloco; k++){
                if(solucao[b.fila][k + b.ini] != 0){
                    if((solucao[b.fila][k + b.ini]) != (palavra[k])){
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }

    /*Testando casas já preenchidas no bloco coluna*/
    else if(b.TipoDaFila == 'c'){
        if(ok){
            for(k = 0; k < tam_bloco; k++){
                if(solucao[k + b.ini][b.fila] != 0){
                    if((solucao[k + b.ini][b.fila]) != (palavra[k])){
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }

    return ok;
}

/*Função que insere os caraceres de um vetor palavra na matriz solucao em
um determinado bloco b e registra essa operação na matriz tabuleiro*/
void encaixa(char *palavra, char **solucao, int **tab, bloco b){
    int k, tam_bloco;

    tam_bloco = b.fim - b.ini;

    /*Encaixe em uma linha*/
    if(b.TipoDaFila == 'l'){
        for(k = 0; k < tam_bloco; k++){
            solucao[b.fila][b.ini + k] = palavra[k];
        }
        for(k = 0; k < tam_bloco; k++){
            tab[b.fila][b.ini + k] += 1;
        }
    }

    /*Encaixe em uma coluna*/
    if(b.TipoDaFila == 'c'){
        for(k = 0; k < tam_bloco; k++){
        solucao[k + b.ini][b.fila] = palavra[k];
        }
        for(k = 0; k < tam_bloco; k++){
            tab[k + b.ini][b.fila] += 1;
        }
    }
    return;
}

/*Função que retira os caracteres de uma palavra de um determinado bloco da
matriz solução. Mas somente se for verificado na matriz tabuleiro que eles
pertencem a uma única palavra*/
void desfazEncaixe(int **tab, char **solucao, bloco b){
    int k;

    /*Desfaz encaixe na linha*/
    if(b.TipoDaFila == 'l'){
        for(k = b.ini; k < b.fim; k++)
            if(tab[b.fila][k] == 1)
                solucao[b.fila][k] = 0;
        for(k = b.ini; k < b.fim; k++)
            tab[b.fila][k] -= 1;
    }

    /*Desfaz encaixe na coluna*/
    else if(b.TipoDaFila == 'c'){
        for(k = b.ini; k < b.fim; k++)
            if(tab[k][b.fila] == 1)
                solucao[k][b.fila] = 0;
        for(k = b.ini; k < b.fim; k++)
            tab[k][b.fila] -= 1;
    }

    return;
}

/*FUNÇÕES AUXILIARES*/

void imprimeTabela(char **M, int lin, int col){
    int i, j;

    for(i = 0; i < lin; i++){
        for(j = 0; j < col; j++){
            printf(" %c ", M[i][j]);
        }
        printf("\n");
    }

    return;
}

int max(int num1, int num2){
    if(num2 > num1)
        return num2;
    else
        return num1;
}
