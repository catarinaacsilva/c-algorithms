#include <stdio.h> 
#include <stdlib.h>

int Count = 0;
int schroder(int);

int main (void) {
	int result = 0;

	for(int i=0; i<=12; i++){
		result = schroder(i);
		fprintf (stdout, "Schroder de %d Resultado: %d Multiplicações: %d \n",i, result, Count);	
		Count = 0;
	}
}

int schroder(int n){
	int array[n+1];
	int tmp = 0;

	array[0] = 1;

	for(int j=1; j<=n; j++){
		for(int i=0; i<j; i++){
			tmp += array[i] * array[j-i-1];
			Count++;
		}
		array[j] = tmp + array[j-1];
		tmp = 0;
	}

	return array[n];
	
}
