#include <stdlib.h>
#include "headers/lista.h"
#include "headers/grafo.h"
#include "headers/stack.h"

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

void dfs(Grafo *grafo, int vertice_inicial, void (*callback)(int)) {

    int visitados[150], i, vertice_atual, 
    *vizinhos = NULL, qtd_vizinhos;

    // Inicializando array de visitados
    for(i = 0; i < 150; i++)
        visitados[i] = 0;

    Stack* stack = cria_stack();
    push(stack, vertice_inicial);
    
    while( !stack_vazio(stack) ) {
        pop(stack, &vertice_atual);
        visitados[vertice_atual] = 1;
        callback(vertice_atual);
        qtd_vizinhos = tamanho(grafo->vetor_vertices[vertice_atual]);
        vizinhos = conteudo(grafo->vetor_vertices[vertice_atual]);
        i = 0;

        while( i < qtd_vizinhos ) 
            if ( visitados[vizinhos[i]] == 0 )
                push(stack, vizinhos[i]);
        
        free(vizinhos);
    }
}