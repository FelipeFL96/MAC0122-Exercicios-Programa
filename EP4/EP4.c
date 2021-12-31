/******************************************/
/**                                      **/
/**  Felipe Fernandes de Lima    9345321 **/
/**  Exerc�cio-Programa 04               **/
/**  Professor: Carlos Eduardo Ferreira  **/
/**  Turma: 50                           **/
/**                                      **/
/******************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct cel{
    char *idVoo;
    char operacao;
    int prioridade;
    int t_restante;
    struct cel *prox;
}aviao;

typedef aviao* apontador;

/*FUN��ES*/
apontador criaAviao(char);
apontador insereNaFila(apontador, apontador);
apontador removeDaFila(apontador, apontador);
apontador atualizaTempoRestante(apontador);
apontador buscaUrgencia(apontador);
int defineUrgencia(apontador);
apontador removePerdidos(apontador);

int main(){
    int tempo_atual,     /*Marca a contagem de tempo da execu��o (d� o tempo real)*/
        aberto,          /*Indica o estado do aeroporto (1)aberto; (0)fechado; (-1)fechado para o dia*/
        tempo_entrada;   /*Guarda os instantes de tempo lidos como entrada*/
    char tipo_entrada;   /*Guarda os tipos de entrada: A, F, Z, P, D ou \0 (n�o houve entrada)*/
    apontador fila,      /*Lista de espera dos avi�es para pouso e decolagem*/
              novoAviao; /*Apontador para inser��o de novos avi�es na lista*/

    /*Inicializa��es*/
    novoAviao = NULL;
    fila = NULL;
    tempo_atual = 1;
    aberto = 0;
    tempo_entrada = 0;

    /*In�cio da opera��o do aeroporto*/
    while(aberto != -1){

        /*----------ENTRADA---------*/
        tipo_entrada = 1;

        /*Verifica��o do tempo*/
        /*Aqui � checado se a �ltima entrada corresponde ao tempo atual,
         para que cada entrada seja lida apenas no tempo em que diz chegar*/
        if(tempo_atual > tempo_entrada)
            scanf("%d", &tempo_entrada);
        if(tempo_atual < tempo_entrada)
            tipo_entrada = '\0';

        /*Se o tipo_entrada n�o for nulo (se houve entrada para o tempo atual),
        ent�o � verificado qual �, de fato, o tipo da entrada: A, F, Z, P ou D*/
        if(tipo_entrada != '\0'){
            scanf(" %c", &tipo_entrada);

            if(tipo_entrada == 'A')
                aberto = 1;
            else if(tipo_entrada == 'F')
                aberto = 0;
            else if(tipo_entrada == 'Z')
                aberto = -1;
            else if(tipo_entrada == 'P'){
                novoAviao = criaAviao(tipo_entrada);
                fila = insereNaFila(fila, novoAviao);
            }
            else if(tipo_entrada == 'D'){
                novoAviao = criaAviao(tipo_entrada);
                fila = insereNaFila(fila, novoAviao);
            }
            else printf("\nValor invalido!\n");
        }

        /*---------SA�DA----------*/
        if(tipo_entrada == 'A')
            printf("%d Aeroporto aberto\n", tempo_atual);
        else if(tipo_entrada == 'F')
            printf("%d Aeroporto fechado\n", tempo_atual);
        else if(tipo_entrada == 'Z')
            printf("%d Aeroporto fechado\n", tempo_atual);
        else{
            if(aberto){
                if(fila != NULL){
                    if(fila->operacao == 'P')
                        printf("%d Autorizado pouso de %s na pista principal\n", tempo_atual, fila->idVoo);
                    else if(fila->operacao == 'D')
                        printf("%d Autorizado decolagem de %s na pista principal\n", tempo_atual, fila->idVoo);
                    fila = removeDaFila(fila, fila);/*fila representa a lista e o primeiro item da lista ao mesmo tempo*/

                    /*Busca de urg�ncias na fila*/
                    fila = buscaUrgencia(fila);
                    if((fila != NULL) && (fila->t_restante <= defineUrgencia(fila))){
                        if(fila->operacao == 'P')
                            printf("%d Autorizado pouso de %s na pista auxiliar\n", tempo_atual, fila->idVoo);
                        else if(fila->operacao == 'D')
                            printf("%d Autorizado decolagem de %s na pista auxiliar\n", tempo_atual, fila->idVoo);
                        fila = removeDaFila(fila, fila);
                    }
                }
            }
        }

        /*------ATUALIZA��ES-----*/
        fila = atualizaTempoRestante(fila);
        fila = removePerdidos(fila);
        tempo_atual++;
    }

    return 0;
}

/*Fun��o que recebe um valor 'P' ou 'D' (para pouso ou decolagem) e l� outras
informa��es de entrada de um avi�o e devolve um apontador para a c�lula gerada*/
apontador criaAviao(char op){
    int i;
    apontador novo = malloc(sizeof(aviao));

    /*Gerando novo avi�o*/
    novo->idVoo = malloc(9*sizeof(char));
    for(i = 0; i < 9; i++)
        novo->idVoo[i] = 0;

    scanf("%s", novo->idVoo);
    scanf("%d", &(novo->prioridade));
    scanf("%d", &(novo->t_restante));
    novo->operacao = op;
    novo->prox = NULL;

    return novo;
}

/*Fun��o que recebe o apontador para uma c�lula que representa um avi�o e o insere
em uma lista, conforme as defini��es de prioridade, devolvendo o apontador para a
lista modificada*/
apontador insereNaFila(apontador fila, apontador aviao){
    apontador p, q;

    if(fila == NULL)
        return aviao;

    p = fila; q = NULL;

    if(aviao->prioridade == 1){
        /*Prefer�ncia para tempos restantes menores*/
        while((p != NULL)&&(p->prioridade == 1)&&(p->t_restante <= aviao->t_restante)){
            /*Prefer�ncia para POUSOS quando o tempo restante for igual*/
            if((aviao->t_restante == p->t_restante)&&
               (aviao->operacao == 'P')&&(p->operacao == 'D'))
                break;

            q = p;
            p = p ->prox;
        }
        if(q == NULL){
            aviao->prox = p;
            fila = aviao;
            return fila;
        }
        aviao->prox = p;
        q->prox = aviao;

    }
    else{
        /*Prefer�ncia para prioridade = 1*/
        while((p != NULL)&&(p->prioridade != 0)){
            q = p;
            p = p->prox;
        }
        /*Prefer�ncia para tempos restantes menores*/
        while((p != NULL)&&(p->t_restante <= aviao->t_restante)){
            /*Prefer�ncia para POUSOS quando o tempo restante for igual*/
            if((aviao->t_restante == p->t_restante)&&
               (aviao->operacao == 'P')&&(p->operacao == 'D'))
                break;

            q = p;
            p = p->prox;
        }

        /*Inser��o na lista*/
        if(q == NULL){
            aviao->prox = p;
            fila = aviao;
            return fila;
        }
        aviao->prox = p;
        q->prox = aviao;
    }
    return fila;
}

/*Fun��o que recebe apontadores para uma lista e um avi�o(c�lula da lista), busca
o aviao na lista e, se o encontrar, o remove, devolvendo um apontador para a
lista modificada*/
apontador removeDaFila(apontador fila, apontador aviao){
    apontador p, q;
    int i;

    p = fila;
    q = NULL;
    /*Busca do avi�o recebido (feita atrav�s de seu idVoo)*/
    while(p != NULL){
        for(i = 0; (p->idVoo[i] != '\0')&&(p->idVoo[i] == aviao->idVoo[i]); i++);
        if((p->idVoo[i]) != '\0'){
            q = p;
            p = p->prox;
        }
        else break;
    }

    /*Remo��o do avi�o encontrado*/
    if(p != NULL){
        if(q == NULL)
            fila = p->prox;
        else
            q->prox = p->prox;
        p->prox = NULL;
        free(p);
    }
    return fila;
}

/*Fun��o que recebe o apotador para uma lista de c�lulas do tipo avi�o e reduz
em 1 o atributo t_restante de cada uma delas, devolvendo um apontador para a
lista modificada*/
apontador atualizaTempoRestante(apontador fila){
    apontador p;

    p = fila;
    while(p != NULL){
        p->t_restante -= 1;
        p = p->prox;
    }
    return fila;
}

/*Fun��o que recebe um apontador para uma lista de c�lulas do tipo avi�o, busca nela
aquelas , altera o atributo 'prioridade' das c�lulas cujo atributo 't_restante' seja
menor que um determinado valor e as insere novamente na lista, devolvendo um
apontador para a lista modificada*/
apontador buscaUrgencia(apontador fila){
    apontador p, novo;

    p = fila;
    while(p != NULL){
        /*Busca de avi�es em situa��o de urg�ncia*/
        if((p->prioridade == 0) && (p->t_restante <= defineUrgencia(fila))){
            /*Criando nova c�lula para o avi�o encontrado*/
            novo = malloc(sizeof(aviao));
            novo->idVoo = p->idVoo;
            novo->operacao = p->operacao;
            novo->prioridade = 1;
            novo->prox = NULL;
            novo->t_restante = p->t_restante;

            /*Substituindo o avi�o encontrado*/
            fila = removeDaFila(fila, p);
            fila = insereNaFila(fila, novo);
            p = novo->prox;
        }
        else
            p = p->prox;
    }
    return fila;
}

/*Fun��o que recebe o apontador para uma lista de c�lulas do tipo avi�o e
devolve um inteiro que representa o valor do tempo limite para que um avi�o
seja considerado urg�ncia*/
int defineUrgencia(apontador fila){
    apontador p;
    int cont_p, cont_max, t_rep;
    p = fila;

    if(fila == NULL)return 2;

    while((p != NULL)&&(p->prioridade == 1))
        p = p->prox;

    /*Descobrindo o n�mero m�ximo de repeti��es de tempo restante entre os n�o-priorit�rios*/
    cont_max = 4; /*Inicializar em 4 garante que o t_restante de urg�ncia seja sempre pelo menos 2*/
    t_rep = 0;
    while(p != NULL){
        cont_p = 1;
        while((p->prox != NULL)&&(p->t_restante == p->prox->t_restante)){
            cont_p++;
            p = p->prox;
        }
        if(cont_p >= cont_max){
            cont_max = cont_p;
            t_rep = p->t_restante;
        }
        if(p != NULL)
            p = p->prox;
    }

    /*Mesma contagem para os priorit�rios*/
    p = fila;
    while((p != NULL)&&(p->prioridade == 1)){
        if(p->t_restante == t_rep)
            cont_max++;
        p = p->prox;
    }

    /*Transformando cont_max em n�mero par*/
    if((cont_max%2) != 0) cont_max++;


    return (cont_max/2);
}

/*Fun��o que recebe um apontador para uma lista de c�lulas do tipo avi�o e, remove
dela as c�lulas cujo atributo t_restante seja igual a 0, devolvendo um apontador
para a lista modificada*/
apontador removePerdidos(apontador fila){
    apontador p, q;

    p = fila;
    while(p != NULL){
        if(p->t_restante == 0){
            q = p;
            p = p->prox;
            fila = removeDaFila(fila, q);
        }
        else{
            q = p;
            p = p->prox;
        }
    }
    return fila;
}
