/******************************************/
/**                                      **/
/**  Felipe Fernandes de Lima    9345321 **/
/**  Exercício-Programa 05               **/
/**  Professor: Carlos Eduardo Ferreira  **/
/**  Turma: 50                           **/
/**                                      **/
/******************************************/

#include <stdio.h>
#include <stdlib.h>

/*Estrutura para uma fila do tabuleiro*/
/*Está sendo considerada FILA um conjunto de posições em sequência no tabuleiro
onde é possível ganhar ou perder o jogo. Ou seja, uma linha horizontal, vertical
ou diagonal de n posições.*/
typedef struct cel{
    int x, y, z, dir;
    int numA, numB;
    struct cel *prox;
}fila;
typedef fila* apontador;

/*Variáveis globais*/
char corJog;     /*Cor com a qual o programa irá jogar*/
char corAdv;     /*Cor com a qual o usuário irá jogar*/
char venceu = 0; /*Variável que diz se houve vencedor*/

/*Funções*/
char*** geraTab(int);
int insereNoTab(char***, int, int, char, int);
apontador insereNaLista(apontador, apontador);
apontador removeDaLista(apontador, apontador);
apontador atualizaFila(char***, apontador, apontador, int);
apontador geraFilas(int);
void decideJogada(char***, apontador, int*, int*, int);

int main(int argc, char *argv[]){
    char ***tab;
    apontador lista, p, q;
    int n, x, y, ok, jogada;

    n = atoi(argv[2]);
    if (argv[1][0] == 'b' || argv[1][0] == 'B') corJog = 'B';
    else corJog = 'A';

    if(corJog == 'B')corAdv = 'A';
    else corAdv = 'B';

    /*Preparando o tabuleiro e as filas para análise*/
    tab = geraTab(n);
    lista = geraFilas(n);

    if(corJog == 'B'){
        setbuf(stdout, NULL);
        printf("\n0 0");
        ok = insereNoTab(tab, 0, 0, corJog, n);
        p = lista;
        while(p != NULL){
            q = p;
            p = p->prox;
            lista = atualizaFila(tab, lista, q, n);
        }
    }
    jogada = 1;
    while(((n%2 == 0)&&(jogada < (n*n*n)/2))||
          ((n%2 != 0)&&(jogada < (n*n*n-1)/2)&&(corJog == 'A'))||
          ((n%2 != 0)&&(jogada < (n*n*n+1)/2)&&(corJog == 'B'))){

        /*Jogada do adversário*/
        setbuf(stdout, NULL);
        printf("\n");
        scanf("%d %d", &x, &y);
        ok = insereNoTab(tab, x, y, corAdv, n);
        if(!ok){
            setbuf(stdout, NULL);
            printf("\nJogada invalida!!");
        }
        p = lista;
        while(p != NULL){
            q = p;
            p = p->prox;
            lista = atualizaFila(tab, lista, q, n);
        }
        if(venceu != 0) break;

        /*Jogada do programa*/
        decideJogada(tab, lista, &x, &y, n);
        setbuf(stdout, NULL);
        printf("%d %d", x, y);
        ok = insereNoTab(tab, x, y, corJog, n);
        if(!ok){
            setbuf(stdout, NULL);
            printf("\nJogada invalida!!");
        }
        p = lista;
        while(p != NULL){
            q = p;
            p = p->prox;
            lista = atualizaFila(tab, lista, q, n);
        }
        if(venceu != 0) break;

        jogada++;
    }
    if(venceu == 0){
        setbuf(stdout, NULL);
        printf("\nEmpatou!");
    }
    if(venceu == corJog){
        setbuf(stdout, NULL);
        printf("\nEu venci!\n");
    }
    else{
        setbuf(stdout, NULL);
        printf("\nEu perdi!\n");
    }

    return 0;
}

/*FUNÇÕES*/

/*Função que recebe um número n e devolve um ponteiro para uma variável
char tipo array de 3 dimensões, com todas as posições inicializadas em 0*/
char*** geraTab(int n){
    char ***tab;
    int i, j, k;

    tab = (char***)malloc(n*sizeof(char**));
    for(i = 0; i < n; i++){
        tab[i] = (char**)malloc(n*sizeof(char*));
        for(j = 0; j < n; j++){
            tab[i][j] = (char*)malloc(n*sizeof(char));
            for(k = 0; k < n; k++)
                tab[i][j][k] = '0';
        }
    }

    return tab;
}

/*Função que recebe as informações de um tabuleiro 3D(tab, n), um pino dele e a cor
de uma bolinha e insere na primeira posição não ocupada do pino o valor da cor*/
int insereNoTab(char ***tab, int x, int y, char cor, int n){
    int k, ok = 1;

    /*Verificação da existência do pino*/
    if((x >= n)||(y >= n)){
        setbuf(stdout, NULL);
        printf("Hmm... Esse pino não existe!");
        ok = 0;
    }

    /*Inserção da bolinha no pino (x,y)*/
    for(k = 0; (k < n)&&(tab[x][y][k] != '0'); k++);
    if(k < n)
        tab[x][y][k] = cor;
    else{
        setbuf(stdout, NULL);
        printf("O pino (%d,%d) ja esta cheio!", x, y);
        ok = 0;
    }

    return ok;
}

/*Função que recebe apontadores para uma lista e para uma célula do tipo fila
e devolve a lista com a célula inserida conforme certas condições de ordenação*/
apontador insereNaLista(apontador lista, apontador fila){
    apontador p, q;

    if(lista == NULL) return fila;
    p = lista;
    q = NULL;

    /*Aqui são postas as condições para ordenação da lista de filas*/
    /*A ideia é que fiquem em primeiro as que só têm brancas ou só têm azuis (dando
    preferência para a cor do jogador) em ordem das mais para as menos preeenchidas*/
    while(p != NULL){
            if((fila->numB == 0)&&(p->numB == 0)&&(fila->numA > p->numA))break;
            if((fila->numA == 0)&&(p->numA == 0)&&(fila->numB > p->numB))break;
        if(corJog == 'A'){
            if((fila->numB == 0)&&(p->numA == 0)&&(fila->numA >= p->numB))break;
            if((fila->numA == 0)&&(p->numB == 0)&&(fila->numB > p->numA))break;
        }
        else if(corJog == 'B'){
            if((fila->numB == 0)&&(p->numA == 0)&&(fila->numA > p->numB))break;
            if((fila->numA == 0)&&(p->numB == 0)&&(fila->numB >= p->numA))break;
        }
        q = p;
        p = p->prox;
    }

    /*Inserção da fila na lista*/
    if(q == NULL){
        fila->prox = p;
        lista = fila;
    }
    else{
        fila->prox = p;
        q->prox = fila;
    }
    return lista;
}

/*Função que recebe apontadores para uma lista e uma célula do tipo fila, busca essa
célula e, quando encontrada, a remove, devolvendo um apontador para a lista modificada*/
apontador removeDaLista(apontador lista, apontador fila){
    apontador p, q;
    p = lista;
    q = NULL;

    if(lista == NULL) return NULL;

    /*Busca a fila para remoção*/
    while((p != NULL)&&((p->dir != fila->dir)||
        (p->x != fila->x)||(p->y != fila->y)||(p->z != fila->z))){
        q = p;
        p = p->prox;
    }

    /*Remoção da fila*/
    if(q == NULL){
        lista = lista->prox;
        p->prox = NULL;
    }
    else{
        q->prox = p->prox;
        p->prox = NULL;
    }

    return lista;
}

/*A partir daqui estão as funções que analisam o tabuleiro por direção. Como foram
encontradas 13 direções diferentes em que se pode percorrê-lo e foram feitas condições
específicas para cada uma nesta implementação, as funções a seguir ficaram extremamente
longas. Entretanto, cada um dos blocos dentro delas (representando uma direção) faz
basicamente o  mesmo que todos os outros com apenas algumas adaptaçõees.*/

/*Função que recebe um valor n de tamanho do tabuleiro e gera para cada direção
células que representem todas as filas possíveis, armazenando-as em uma lista,
devolvida ao final*/
apontador geraFilas(int n){
    apontador lista = NULL, novo;
    int i, j, k;

    /*Filas na direção bidiagonal 12*/
        novo = malloc(sizeof(fila));
        novo->x = n-1; novo->y = n-1; novo->z = n-1; novo->dir = 12;
        novo->numA = 0; novo->numB = 0;
        novo->prox = NULL;
        lista = insereNaLista(lista, novo);
    /*Filas na direção bidiagonal 11*/
        novo = malloc(sizeof(fila));
        novo->x = 0; novo->y = 0; novo->z = 0; novo->dir = 11;
        novo->numA = 0; novo->numB = 0;
        novo->prox = NULL;
        lista = insereNaLista(lista, novo);
    /*Filas na direção bidiagonal 10*/
        novo = malloc(sizeof(fila));
        novo->x = 0; novo->y = n-1; novo->z = 0; novo->dir = 10;
        novo->numA = 0; novo->numB = 0;
        novo->prox = NULL;
        lista = insereNaLista(lista, novo);
    /*Filas na direção bidiagonal 9*/
        novo = malloc(sizeof(fila));
        novo->x = n-1; novo->y = 0; novo->z = 0; novo->dir = 9;
        novo->numA = 0; novo->numB = 0;
        novo->prox = NULL;
        lista = insereNaLista(lista, novo);
    /*Filas na direção diagonal 8*/
    for(k = 0; k < n; k++){
            novo = malloc(sizeof(fila));
            novo->x = n-1; novo->y = 0; novo->z = k; novo->dir = 8;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
    }
    /*Filas na direção diagonal 7*/
    for(k = 0; k < n; k++){
            novo = malloc(sizeof(fila));
            novo->x = 0; novo->y = 0; novo->z = k; novo->dir = 7;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
    }
    /*Filas na direção diagonal 6*/
    for(j = 0; j < n; j++){
            novo = malloc(sizeof(fila));
            novo->x = 0; novo->y = j; novo->z = n-1; novo->dir = 6;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
    }

    /*Filas na direção diagonal 5*/
    for(j = 0; j < n; j++){
            novo = malloc(sizeof(fila));
            novo->x = 0; novo->y = j; novo->z = 0; novo->dir = 5;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
    }
    /*Filas na direção diagonal 4*/
    for(i = 0; i < n; i++){
            novo = malloc(sizeof(fila));
            novo->x = i; novo->y = 0; novo->z = n-1; novo->dir = 4;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
    }
    /*Filas na direção diagonal 3*/
    for(i = 0; i < n; i++){
            novo = malloc(sizeof(fila));
            novo->x = i; novo->y = 0; novo->z = 0; novo->dir = 3;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
    }
    /*Filas na direção reta 2*/
    for(j = 0; j < n; j++){
        for(i = 0; i < n; i++){
            novo = malloc(sizeof(fila));
            novo->x = i; novo->y = j; novo->z = 0; novo->dir = 2;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
        }
    }
    /*Filas na direção reta 1*/
    for(k = 0; k < n; k++){
        for(i = 0; i < n; i++){
            novo = malloc(sizeof(fila));
            novo->x = i; novo->y = 0; novo->z = k; novo->dir = 1;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
        }
    }
    /*Filas na direção reta 0*/
    for(k = 0; k < n; k++){
        for(j = 0; j < n; j++){
            novo = malloc(sizeof(fila));
            novo->x = 0; novo->y = j; novo->z = k; novo->dir = 0;
            novo->numA = 0; novo->numB = 0;
            novo->prox = NULL;
            lista = insereNaLista(lista, novo);
        }
    }

    return lista;
}

/*Função que recebe apontadores para uma lista e uma fila e o array do tabuleiro e
conta o número de bolinhas de cada cor nessa fila, atualizando o valor contido na
célula. Além disso ela também avalia se a fila recebida está completa com uma única
 cor avisando que houve vencedor.*/
apontador atualizaFila(char ***tab, apontador lista, apontador F, int n){
    int pos, contAzul = 0, contBranco = 0;

    /*DIREÇÕES EM LINHA RETA*/
    /*Percorre, a partir do ponto, a direção reta X*/
    if(F->dir == 0)
        for(pos = 0; pos < n; pos++){
            if(tab[pos][F->y][F->z] == 'A') contAzul++;
            else if(tab[pos][F->y][F->z] == 'B') contBranco++;
        }
    /*Percorre, a partir do ponto, a direção reta Y*/
    else if(F->dir == 1)
        for(pos = 0; pos < n; pos++){
            if(tab[F->x][pos][F->z] == 'A')contAzul++;
            else if(tab[F->x][pos][F->z] == 'B')contBranco++;
        }
    /*Percorre, a partir do ponto, a direção reta Z*/
    else if(F->dir == 2)
        for(pos = 0; pos < n; pos++){
            if(tab[F->x][F->y][pos] == 'A') contAzul++;
            else if(tab[F->x][F->y][pos] == 'B')contBranco++;
        }

    /*DIREÇÕES DIAGONAIS PLANAS*/
    /*Percorre, a partir do ponto, a direção diagonal YZ*/
    else if(F->dir == 3)
        for(pos = 0; pos < n; pos++){
            if(tab[F->x][pos][pos] == 'A') contAzul++;
            else if(tab[F->x][pos][pos] == 'B')contBranco++;
        }
    /*Percorre, a partir do ponto, a direção diagonal Y(-Z)*/
    else if(F->dir == 4)
        for(pos = 0; pos < n; pos++){
            if(tab[F->x][pos][(n-1)-pos] == 'A') contAzul++;
            else if(tab[F->x][pos][(n-1)-pos] == 'B') contBranco++;
        }
    /*Percorre, a partir do ponto, a direção diagonal XZ*/
    else if(F->dir == 5)
        for(pos = 0; pos < n; pos++){
            if(tab[pos][F->y][pos] == 'A') contAzul++;
            else if(tab[pos][F->y][pos] == 'B')contBranco++;
        }
    /*Percorre, a partir do ponto, a direção diagonal X(-Z)*/
    else if(F->dir == 6)
        for(pos = 0; pos < n; pos++){
            if(tab[pos][F->y][(n-1)-pos] == 'A') contAzul++;
            else if(tab[pos][F->y][-pos] == 'B') contBranco++;
        }
    /*Percorre, a partir do ponto, a direção diagonal XY*/
    else if(F->dir == 7)
        for(pos = 0; pos < n; pos++){
            if(tab[pos][pos][F->z] == 'A') contAzul++;
            else if(tab[pos][pos][F->z] == 'B') contBranco++;
        }
    /*Percorre, a partir do ponto, a direção diagonal (-X)Y*/
    else if(F->dir == 8)
        for(pos = 0; pos < n; pos++){
            if(tab[(n-1)-pos][pos][F->z] == 'A') contAzul++;
            else if(tab[(n-1)-pos][pos][F->z] == 'B') contBranco++;
        }

    /*DIREÇÕES DIAGONAIS TRIDIMENSIONAIS*/
    /*Percorre, a partir do ponto, a direção bidiagonal (-X)YZ*/
    else if(F->dir == 9)
        for(pos = 0; pos < n; pos++){
            if(tab[(n-1)-pos][pos][pos] == 'A') contAzul++;
            else if(tab[(n-1)-pos][pos][pos] == 'B') contBranco++;
        }
    /*Percorre, a partir do ponto, a direção bidiagonal X(-Y)Z*/
    else if(F->dir == 10)
        for(pos = 0; pos < n; pos++){
            if(tab[pos][(n-1)-pos][pos] == 'A') contAzul++;
            else if(tab[pos][(n-1)-pos][pos] == 'B') contBranco++;
        }
    /*Percorre, a partir do ponto, a direção bidiagonal XYZ*/
    else if(F->dir == 11)
        for(pos = 0; pos < n; pos++){
            if(tab[pos][pos][pos] == 'A') contAzul++;
            else if(tab[pos][pos][pos] == 'B') contBranco++;
        }
    /*Percorre , a partir do ponto a direção bidiagonal (-X)(-Y)Z*/
    else if(F->dir == 12)
        for(pos = 0; pos < n; pos++){
            if(tab[(n-1)-pos][(n-1)-pos][pos] == 'A') contAzul++;
            else if(tab[(n-1)-pos][(n-1)-pos][pos] == 'B') contBranco++;
        }
    if(F->numA != contAzul){
        lista = removeDaLista(lista, F);
        F->numA = contAzul;
        lista = insereNaLista(lista, F);
    }
    if(F->numB != contBranco){
        lista = removeDaLista(lista, F);
        F->numB = contBranco;
        lista = insereNaLista(lista, F);
    }

    if(F->numA == n)venceu = 'A';
    if(F->numB == n)venceu = 'B';

    return lista;
}

/*Função que recebe um array e um n representando o tabuleiro, o apontador lista e
ponteiros para x e y (um pino) e decide em qual pino deve efetuar jogada*/
void decideJogada(char ***tab, apontador lista, int *x, int *y, int n){
    apontador p;
    int i, j, k, ok = 0;

    p = lista;

    while(!ok){
        i = p->x; j = p->y; k = p->z;

        /*Direção 0-------------------------------------------------*/
        if(p->dir == 0){
            while((i < n)&&(tab[i][j][k] != '0')) i++;
            if(i < n){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 1-------------------------------------------------*/
        else if(p->dir == 1){
            while((j < n)&&(tab[i][j][k] != '0')) j++;
            if(j < n){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 2-------------------------------------------------*/
        else if(p->dir == 2){
            while((k < n)&&(tab[i][j][k] != '0')) k++;
            if(j < n){
                    *x = i;
                    *y = j;
                    ok = 1;
            }
        }
        /*Direção 3-------------------------------------------------*/
        else if(p->dir == 3){
            while((j < n)&&(k < n)&&(tab[i][j][k] != '0')){
                j++;
                k++;
            }
            if((j < n)&&(k < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 4-------------------------------------------------*/
        else if(p->dir == 4){
            while((j < n)&&(k >= 0)&&(tab[i][j][k] != '0')){
                j++;
                k--;
            }
            if((j < n)&&(k >= 0)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 5-------------------------------------------------*/
        else if(p->dir == 5){
            while((i < n)&&(k < n)&&(tab[i][j][k] != '0')){
                i++;
                k++;
            }
            if((j < n)&&(k < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 6-------------------------------------------------*/
        else if(p->dir == 6){
            while((i < n)&&(k >= 0)&&(tab[i][j][k] != '0')){
                i++;
                k--;
            }
            if((j < n)&&(k >= 0)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 7-------------------------------------------------*/
        else if(p->dir == 7){
            while((i < n)&&(j < n)&&(tab[i][j][k] != '0')){
                i++;
                j++;
            }
            if((i < n)&&(j < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 8-------------------------------------------------*/
        else if(p->dir == 8){
            while((i >= 0)&&(j < n)&&(tab[i][j][k] != '0')){
                i--;
                j++;
            }
            if((i >= 0)&&(j < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 9-------------------------------------------------*/
        else if(p->dir == 9){
            while((i >= 0)&&(j < n)&&(k < n)&&(tab[i][j][k] != '0')){
                i--;
                j++;
                k++;
            }
            if((i >= 0)&&(j < n)&&(k < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 10------------------------------------------------*/
        else if(p->dir == 10){
            while((i < n)&&(j >= 0)&&(k < n)&&(tab[i][j][k] != '0')){
                i++;
                j--;
                k++;
            }
            if((i < n)&&(j >= 0)&&(k < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 11------------------------------------------------*/
        else if(p->dir == 11){
            while((i < n)&&(j < n)&&(k < n)&&(tab[i][j][k] != '0')){
                i++;
                j++;
                k++;
            }
            if((i < n)&&(j < n)&&(k < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }
        /*Direção 12------------------------------------------------*/
        else if(p->dir == 12){
            while((i >= 0)&&(j >= 0)&&(k < n)&&(tab[i][j][k] != '0')){
                i--;
                j--;
                k++;
            }
            if((i >= 0)&&(j >= 0)&&(k < n)){
                    *x = i;
                    *y = j;
                    /*Verificando se o ponto escolhido é o topo do pino*/
                    if((k > 0)&&(tab[i][j][k-1] != '0')) ok = 1;
                    else if (k == 0) ok = 1;
            }
        }

        if(!ok) p = p->prox;
    }

    return;
}
