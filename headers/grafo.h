#ifndef GRAFO
#define GRAFO
#include "lista.h"

typedef struct {
    Lista *vetor_vertices[150];
} Grafo;

Grafo *cria_grafo();
void adicionar_aresta(Grafo *grafo, int x, int y);
int *adjacencias(Grafo *grafo, int x);
int grau_vertice(Grafo *grafo, int x);

#endif