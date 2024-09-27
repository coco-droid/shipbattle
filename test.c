#include <stdio.h>
#include <stdlib.h>

int main (void){
    int matrice_A[40][40],Vecteurs_B[40],n;
    do
    {
        printf("Dimensions de la matrice carres:\n");
        scanf("%d",&n);
    } while (n<0 || n>40);
    printf("finish\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            printf("Donner la valeur de la case (%d,%d):\n",i,j);
            scanf("%d",&matrice_A[i][j]);
        }
    }
    for(int i=0;i<n/2;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if (matrice_A[i][j]==matrice_A[j][n-i-1])
            {
                Vecteurs_B[i]=1;
            }
            else
            {
                Vecteurs_B[i]=0;
            }
        }
    }
    for(int i=0;i<n/2;i++)
    {
        if (Vecteurs_B[i]==1)
        {
            printf("La matrice est symetrique\n");
        }
        else
        {
            printf("La matrice n'est pas symetrique\n");
        }
    }
    return 0;
}