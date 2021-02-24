#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int w,l;
	if(argv[2] =="-w"){
		w = atoi(argv[3]);
		l = atoi(argv[5]);
	}else{
		l = atoi(argv[3]);
		w = atoi(argv[5]);
	}
	if(!strcmp(argv[1],"-p")){
		printf("\nPerimetro = %d units\n",2*l+2*w);
	}else if(!strcmp(argv[1],"-a")){
		printf("\nArea = %d units\n",l*w);
	}else{
		printf("\nNo se reconoce esa funcion\n");
	}
return 0;
}
