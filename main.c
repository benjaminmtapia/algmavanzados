#include <stdio.h>
#include <stdlib.h>
#include "string.h"

struct nodo
{
	int costo;
	int arregloCamino[50];
	struct nodo* siguiente;
};
typedef struct nodo nodo;

struct grafo{
	int** matrizAdyacencia;
	nodo* vertices;
	int numero_nodos;
};

typedef struct grafo grafo;
struct lista
{
	nodo* inicio;
	nodo* fin;
	
};
typedef struct lista lista;

void encolar(lista* Q, nodo* c)
{
	if(Q->inicio==NULL)
	{
		Q->inicio=c;
		Q->fin=c;
	}
	else
	{
		Q->fin->siguiente=c;
		Q->fin=c;
	}
	c->siguiente = NULL;
}

lista* crearLista()
{
	lista* l;
	l = (lista*)malloc(sizeof(lista));
	l->inicio = NULL;
	l->fin = NULL;

	return l;
}

nodo* crearNodo(int camino[])
{
	nodo* c;
	c=(nodo*)malloc(sizeof(nodo));
	memcpy(c->arregloCamino,camino,sizeof(c->arregloCamino));
	c->siguiente=NULL;
	return c;
}
nodo* desencolar(lista* Q)
{
	if(Q->inicio==NULL)
		return NULL;
	else
	{
		nodo* r;
		
		r=Q->inicio;
		Q->inicio=Q->inicio->siguiente;
		return r;
	}
}

int calcularCosto(nodo* camino, int n, grafo*G){
	int costo = 0;
	int indice1 = 0 ;
	int indice2 = 0 ;
	for(int i = 0 ; i < n-1; i++){
		indice1 = camino->arregloCamino[i];
		indice2 = camino->arregloCamino[i+1];
		costo = costo + G->matrizAdyacencia[indice1-1][indice2-1];
	}
	return costo;
}


void imprimircamino(int camino[], int n){
	for (int i = 0; i < n; ++i)
	{
		printf("%d ", camino[i] );
	}

}
void determinarCostos(lista*Q, int nodos, grafo* G){
	nodo*cursor = Q->inicio;
	int costo;
	while(cursor!=NULL){
		costo = 0;
		cursor->costo =calcularCosto(cursor,nodos,G);
		cursor = cursor->siguiente;
	}

}
int determinarMinimo(lista*Q, int nodos){
	int minimo;
	nodo* cursor = Q->inicio;
	minimo = cursor->costo;
	while(cursor!=NULL){
		if(cursor->costo < minimo){
			minimo = cursor->costo;
		}
		cursor = cursor->siguiente;
	}
	//printf("EL COSTO MINIMO ES : %d\n",minimo );
	return minimo;
}
void imprimirLista(lista* Q, int nodos){
	nodo* cursor = Q->inicio;
	while(cursor!=NULL){
		
		imprimircamino(cursor->arregloCamino,nodos);
		printf(" costo: %d", cursor->costo );
		printf("\n");
		cursor = cursor->siguiente;
	}
}
void entregarMinimo(lista* Q, int n){
	int minimo = determinarMinimo(Q,n);
	nodo* cursor = Q->inicio;
	FILE* salida;
	salida = fopen("salida.out","w");
	while(cursor!=NULL){
		if(cursor->costo == minimo){
			fprintf(salida, "%d\n",minimo+2 );
			fprintf(salida, "%d-",0 );
			for(int i = 0; i<n; i++){
				fprintf(salida, "%d-", cursor->arregloCamino[i]);
			}
			fprintf(salida, "%d\n",0 );
			break;
		}
		cursor = cursor ->siguiente;
	}

}
void leerarchivo(grafo* G){
		//char senal[10];
		int nodos;
		FILE* archivo;
		char buffer[25*100];
		archivo=fopen("entrada.in","r");
		int cont=0;
		char* valor;
		char* valor2;
		char* valor3;
		valor = (char*) malloc(sizeof(char));
		valor2= (char*) malloc(sizeof(char));
		valor3= (char*) malloc(sizeof(char));
		if(archivo==NULL){
		printf("%s\n","Falló apertura de archivo" );
		exit(1);
		}
		fgets(buffer, 25*100, archivo);
		valor = strtok(buffer, "\n");

		nodos = atoi(valor);
		G->numero_nodos = nodos ;

		//CREACION MATRIZ DE ADYACENCIA
		int** matrizAdyacencia=(int**)malloc(sizeof(int*)*nodos);
			for (int i = 0; i < nodos; i++)
			{
			matrizAdyacencia[i] = (int*)malloc(sizeof(int*)*nodos);
			for (int j = 0; j < nodos; ++j)
			{
				matrizAdyacencia[i][j] = 0;
			}
			}

			int nodoa = 0;
			int nodob = 0;
			int costo = 0;

			while(!feof(archivo)){
			fgets(buffer, 25*100, archivo);
			valor	=	strtok(buffer," ");
			valor2	=	strtok(NULL," ");
			valor3  =	strtok(NULL,"\n");
			nodoa= atoi(valor);
			nodob= atoi(valor2);
			costo= atoi(valor3);
			matrizAdyacencia[nodoa-1][nodob-1] = costo;
			matrizAdyacencia[nodob-1][nodoa-1] = costo;
			}

			fclose(archivo);
			G->matrizAdyacencia = matrizAdyacencia;
}

void BruteForce(int vertice, int contador, int cantidad,int camino[], int* visitados, lista* listaCaminos ){

	if(contador == cantidad){
		camino[contador-1] = vertice;	
		nodo* nodoCamino = crearNodo(camino);
		
		encolar(listaCaminos,nodoCamino);
		//imprimircamino(camino,cantidad);
		//printf("\n");
	
	}
	else{
		visitados[vertice-1] = 1;
		for (int i = 0; i < cantidad; i++){
			if(visitados[i]!=1){
				camino[contador] =  i+1;
				BruteForce(i+1,contador+1,cantidad,camino,visitados,listaCaminos);
			}
		}
		visitados[vertice-1] = 0;
	}
	


}
int main(int argc, char const *argv[])
{
	grafo* G;
	G = (grafo*) malloc(sizeof(grafo));
	
	leerarchivo(G);
	int n = G->numero_nodos;

	int* nodos = (int*) malloc(sizeof(int)*G->numero_nodos);
	int* visitados = (int*) malloc(sizeof(int)*G->numero_nodos);
	int camino[n];
	for (int i = 0; i < n; i++)
	{
		nodos[i]=i+1;
		visitados[i] = 0;
		camino[i]=0;
	}
	lista* listaCaminos = crearLista();
	for (int i = 1; i <= n; i++){
		camino[0] = i;
		BruteForce(i,1,n,camino,visitados,listaCaminos);
	}
	for (int i = 0; i < n; ++i)
	{
		for( int j = 0 ; j < n; j++){
			printf("%d ",G->matrizAdyacencia[i][j] );
		} 
		printf("\n");
	}
	determinarCostos(listaCaminos,n,G);
	//imprimirLista(listaCaminos,n);
	determinarMinimo(listaCaminos,n);
	entregarMinimo(listaCaminos,n);
	return 0;
}