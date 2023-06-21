#include <stdlib.h>
#include "headers/stack.h"
#include "headers/lista.h"
#include "headers/grafo.h"
#include <stdio.h>

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

// nao_visitados é passado como argumento na main. vistados é uma lista vazia que ira guardar todos os vértices
// visitados durante o dfs.
void dfs(Grafo *grafo, int vertice_inicial, Lista* visitados, Lista* nao_visitados) {

    int i, vertice_atual, *vizinhos = NULL;
    Stack* stack = cria_stack();
    push(stack, vertice_inicial);
    
    while( pop(stack, &vertice_atual) ) {
        remover_elem_lista(nao_visitados, vertice_atual);
        add_elem_lista(visitados, vertice_atual);
        vizinhos = adjacencias(grafo, vertice_atual);
        i = 0;
        while( i < grau_vertice(grafo, vertice_atual) )  {
            if ( existe_na_lista(nao_visitados, vizinhos[i]) && !existe_no_stack(stack, vizinhos[i]) ) {
                push(stack, vizinhos[i]);
            }
            i += 1;
        }
        free(vizinhos);
    }

    free(stack);
}

void printa_grafo(Grafo* grafo) {
    for(size_t i = 0; i < 150; i++) {
        printf("Vértice %ld: ", i);
        printa_lista(grafo->vetor_vertices[i]);
    }
}

void destruir_grafo(Grafo* grafo) {
    for(int i = 0; i < 150; i++)
        destruir_lista(grafo->vetor_vertices[i]);
    
    free(grafo);
}