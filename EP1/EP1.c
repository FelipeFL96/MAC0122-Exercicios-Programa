/******************************************/
/**                                      **/
/**  Felipe Fernandes de Lima    9345321 **/
/**  Exercício-Programa 01               **/
/**  Professor: Carlos Eduardo Ferreira  **/
/**  Turma: 50                           **/
/**                                      **/
/******************************************/

#include <stdio.h>
#include <math.h>

int achaPotencia(int);
int numDigitos(int);
int chao(double);

int main(){
    int n, k;
    printf("MAC0122 - EP1\n");
    printf("\nInsira o conjunto de instancias: \n\n");

     do{
        scanf("%d", &n);
        k = achaPotencia(n);
        if(k > 0){
            printf("k = %d\n\n", k);
        }
        else if(n == 0){
            printf("Tchau!!\n");
        }
        else{
            printf("Erro! Nao foi possivel encontrar um\nresultado para esta entrada!\n\n");
        }
    }while(n != 0);

    return 0;
}

int achaPotencia(int n){
    int k, digitos_extra;
    double lim_inf, lim_sup;

    if(n == 0){
        return 0;
    }
int contador = 0;
    for(digitos_extra = numDigitos(n) + 1;;digitos_extra++){
        /*Calculo do limite inferior de k*/
        lim_inf = (log10(n)+digitos_extra) / log10(2);
        /*Calculo do limite superior de k*/
        lim_sup = (log10(n+1)+digitos_extra) / log10(2);

        if(chao(lim_inf) != chao(lim_sup)){
            k = chao(lim_sup);
            break;
        }
        else if(digitos_extra > 1000000000){
            return -1;
        }
    }

    return k;
}

int numDigitos(int num){
    int digitos;
    for(digitos = 0; num != 0; digitos++)
        num /= 10;
    return digitos;
}

int chao(double num){
    int chao = num;
    return chao;
}
