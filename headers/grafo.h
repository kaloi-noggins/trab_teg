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
void dfs(Grafo* grafo, int vertice_inicial, Lista* visitados, Lista* nao_visitados);
void printa_grafo(Grafo* grafo);
#endif