#include <stdlib.h>
#include "grafo.h"

void adicionar_aresta(Grafo *grafo, int x, int y) {
   add_elem_lista(grafo->vetor_vertices[x], y);
}

int *adjacencias(Grafo *grafo, int x) {
    return conteudo(grafo->vetor_vertices[x]);
}

Grafo *cria_grafo() {
    Grafo *novo_grafo = (Grafo*)malloc(sizeof(Grafo));
    int i;
    for(i = 0; i < 150; i++) 
        novo_grafo->vetor_vertices[i] = cria_lista();
    
    return novo_grafo;
}

int grau_vertice(Grafo *grafo, int x) {
    return tamanho(grafo->vetor_vertices[x]);
}
