#include <stdlib.h>
#include "headers/lista.h"

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

    if ( lista->comeco ) {
        novo->proximo = lista->comeco;
        lista->comeco->anterior = novo;
        novo->anterior = NULL;
    }

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

void destruir_lista(Lista* lista) {
    Nodo* temp = lista->comeco;
    Nodo* proximo;
    
    while ( temp != NULL ) {
        proximo = temp->proximo;
        free(temp);
        temp = proximo;
    }
    
    free(lista);
}

int existe_na_lista(Lista* lista, int x) {
    if ( tamanho(lista) == 0 )
        return 0;

    Nodo* temp = lista->comeco;
    int encontrou = 0;

    while ( temp != NULL && !encontrou ) {

        if ( temp->vertice == x )
            encontrou = 1;

        temp = temp->proximo;
    }

    if ( !encontrou )
        return 0;
    
    return 1;
}

int remover_elem_lista(Lista* lista, int x) {
    if ( tamanho(lista) == 0 )
        return 0;

    Nodo* temp = lista->comeco->proximo;

    if ( lista->comeco->vertice == x ) {
        lista->comeco->proximo->anterior = NULL;
        temp = lista->comeco->proximo;
        free(lista->comeco);
        lista->comeco = temp;
        lista->tamanho -= 1;
        return 1;
    }

    int encontrou = 0;

    while ( temp != NULL && !encontrou )  {
        if ( temp->vertice == x )
            encontrou = 1;
        
        else if ( temp->vertice != x )
            temp = temp->proximo;
    }

    if ( encontrou ) {
        temp->anterior->proximo = temp->proximo;
        temp->proximo->anterior = temp->anterior;
        free(temp);
        lista->tamanho -= 1;
    }

    return 0;
}

int tamanho(Lista* lista) {
    return lista->tamanho;
}