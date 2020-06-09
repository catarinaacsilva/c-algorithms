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
	int tmp = 0;

	if(n == 0)
		return 1;

	for(int i=0; i<n; i++){
		tmp += schroder(i) * schroder(n-i-1);
		Count++;
	}
	
	return (tmp + schroder(n-1)); 
	
}

