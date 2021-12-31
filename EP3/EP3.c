/******************************************/
/**                                      **/
/**  Felipe Fernandes de Lima    9345321 **/
/**  Exercício-Programa 03               **/
/**  Professor: Carlos Eduardo Ferreira  **/
/**  Turma: 50                           **/
/**                                      **/
/******************************************/

#include <stdio.h>
#include <stdlib.h>

void flip(int, int[], int);
void ordenaPanquecas(int*, int);

int main(){
    int i, *v, tam;

    printf("EP3 - Ordenando Panquecas");

    /*Recebendo o vetor*/
    printf("\n\nInsira o tamanho: ");
    scanf("%d", &tam);
    v = (int*)malloc(tam*sizeof(int));
    printf("Preencha o vetor: ");
    for(i = 0; i < tam; i++)
        scanf("%d", &v[i]);

    printf("\n");
    ordenaPanquecas(v, tam);
    printf("\n");

    free(v);
    return 0;
}

void flip(int pos, int v[], int tam){
    int i, j, *aux;

    aux = (int*)malloc((tam - pos)*sizeof(int));

    j = 0;
    for(i = tam-1; i >= pos; i--){
        aux[j] = v[i];
        j++;
    }
    j = 0;
    for(i = pos; i < tam; i++){
        v[i] = aux[j];
        j++;
    }
    printf(" %d ", pos);

    free(aux);
    return;
}

void ordenaPanquecas(int *pilha, int tam){
    int i, j, maior;

    for(i = 0; i < tam; i++){
        maior = i;
        for(j = i; j < tam; j++){
            if(pilha[j] >= pilha[maior])
                maior = j;
        }
        if(maior != i){
            if(maior != (tam-1))
                flip(maior, pilha, tam);
            flip(i, pilha, tam);
        }
    }
    return;
}
