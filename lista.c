#include <stdlib.h>
#include "lista.h"

Lista *cria_lista(){ 
    Lista *nova_lista = (Lista*)malloc(sizeof(Lista));
    nova_lista->comeco = NULL;
    nova_lista->tamanho = 0;
    return nova_lista;
}

void add_elem_lista(Lista *lista, int x) {
    Nodo *novo = (Nodo*)malloc(sizeof(Nodo));
    novo->proximo = NULL;
    novo->vertice = x;

    if ( lista->comeco ) 
        novo->proximo = lista->comeco;
    
    lista->comeco = novo;
    lista->tamanho += 1;
}

int *conteudo(Lista *lista) {
    if ( lista->tamanho == 0 )
        return NULL;

    Nodo *temp = lista->comeco;
    int *vetor = (int*)malloc(sizeof(int) * lista->tamanho);
    int i;

    for(i = 0; i < lista->tamanho; i++) {
        vetor[i] = temp->vertice;
        temp = temp->proximo;
    }

    return vetor;
}

int tamanho(Lista* lista) {
    return lista->tamanho;
}

