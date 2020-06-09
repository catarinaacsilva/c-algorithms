#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

/* alusão da função que implementa o algoritmo pretendido */
/* allusion to the function that implements the algorithm */
void DeleteRepElements (int [], int *);

/* variável global para contar as operações aritméticas executadas pelo algoritmo */
/* global variable for counting the arithmetic operations executed by the algorithm */
int Comp = 0;
int Copy = 0;

int main (void)
{
	/* declaração dos arrays de teste - usar o pretendido para cada execução */
	/* declaration of the test arrays - use each one for each execution */

	//int Array[] = { 1, 2, 2, 2, 3, 3, 4, 5, 8, 8 };
	//int Array[] = { 1, 2, 2, 2, 3, 3, 3, 3, 8, 8 };
	//int Array[] = { 1, 2, 5, 4, 7, 0, 3, 9, 6, 8 };
	
	//Arrays para teste!
	//int Array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int Array[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	
	//int Array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };


	int NElem = sizeof (Array) / sizeof (int); int i;

	/* invocação do algoritmo pretendido - algorithm invocation */ 
	DeleteRepElements (Array, &NElem);

	fprintf (stdout, "Sequencia resultante -> ");	
	for (i = 0; i < NElem; i++)
	    fprintf (stdout, "%d\t", Array[i]);
    fprintf (stdout, "\nN. de comparacoes = %2d N. de deslocamentos = %2d\n", Comp, Copy);
    

	/* apresentação do resultado e do número de operações de comparação executadas pelo algoritmo */
	/* presenting the result and the number of comparation operations executed by the algorithm */

	exit (EXIT_SUCCESS);
}

/* implementação do algoritmo pretendido */
/* não se esqueça de contar as operações aritméticas executadas pelo algoritmo usando a variável global */

/* implementation of the pretended algorithm */
/* do not forget to count the arithmetic operations using the global variable */

void DeleteRepElements (int array[], int * n) {
	int num = *n;	
	
	assert (num > 1);
	
	for(int i=0; i<num-1; i++){
		for(int j=i+1; j<num; j++){
			Comp++;
			if(array[j] == array[i]){
				for(int k=j; k<num-1; k++){
					array[k] = array[k+1];
					printf("%d\n", array[k]);
					Copy++;
				}
				num = num-1;
				j--;
			}	
		}
	}

	*n = num;

}
