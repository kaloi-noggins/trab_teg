#include <stdlib.h>
#include "headers/stack.h"
#include "headers/lista.h"
#include "headers/grafo.h"

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

// Retorna uma lista com os vértices visitados
void dfs(Grafo *grafo, int vertice_inicial, Lista* visitados, Lista* nao_visitados) {

    int i, vertice_atual, *vizinhos = NULL, qtd_vizinhos;
    Stack* stack = cria_stack();
    push(stack, vertice_inicial);
    
    while( pop(stack, &vertice_atual) ) {

        remover_elem_lista(nao_visitados, vertice_atual);
        add_elem_lista(visitados, vertice_atual);
        vizinhos = conteudo(grafo->vetor_vertices[vertice_atual]);
        qtd_vizinhos = tamanho(grafo->vetor_vertices[vertice_atual]);
        i = 0;

        while( i < qtd_vizinhos ) 
            if ( existe_na_lista(nao_visitados, vizinhos[i]) )
                push(stack, vizinhos[i]);
        
        free(vizinhos);

    }

    free(stack);
}