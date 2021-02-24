#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int main(int argc, char *argv[]){
	char op = 'Y';
	int size, ID,i=0;
	char nombre[100];
	FILE *archivo = fopen("Datalog.txt","w");
	if(archivo == NULL){
		printf("No jala");
	}
	fprintf(archivo,"Lista\n");
	while(i<atoi(argv[2]) && toupper(op) == 'Y'){
		printf("Enter name:\n");
		scanf(" %s",&nombre);
		printf("\n");
		printf("Enter ID:\n");
		scanf("%d",&ID);
		printf("\n");
		if(archivo == NULL)
		printf("No jala");
		fprintf(archivo," %s %d \n",nombre,ID);
		printf("Desea continuar? [Y/n]\n");
		scanf("%c",&op);
		scanf("%c",&op);
		printf("\n");
		i+=1;
	}
	fclose(archivo);
	return 0;
}
