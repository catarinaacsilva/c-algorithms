#include <stdio.h>
#include <stdlib.h>


int Armstrong (void);

int count;

int main (void){
	count = 0;
	Armstrong();
	printf("%d\n", count);	
}


int Armstrong (void){
	int max = 9;
	int result=0;
	for(int a=1; a<=max; a++){
		for(int b=0; b<=max; b++){
			for(int c=0; c<=max; c++){
				result = a*a*a + b*b*b + c*c*c;
				count += 6;
				if(result == a*100 + b*10 + c)	
					printf("%d %d %d \n", a,b,c);		
			}
		}
	}
}