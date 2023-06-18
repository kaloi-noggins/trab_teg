#include <stdio.h>
#include <stdlib.h>
#include "./headers/lista.h"
#include "./headers/stack.h"

int main() {
    Lista* lista = cria_lista();
    Stack* stack = cria_stack();
    int* valores;
    int valor_pop;

    for(int i = 0; i < 10; i++) {
        add_elem_lista(lista, i);
        push(stack, i);
    }

    valores = conteudo(lista);
    printf("Lista original: ");

    for(int i = 0; i < tamanho(lista); i++) 
        printf("%d->",valores[i]);

    free(valores);
    remover_elem_lista(lista, 3);
    valores = conteudo(lista);
    printf("\nLista sem três: ");

    for(int i = 0; i < tamanho(lista); i++) 
        printf("%d->",valores[i]);

    printf("\nExiste 3 = %d\n", existe_na_lista(lista, 3));
    printf("Existe 1 = %d\n", existe_na_lista(lista, 1));
    
    printf("Stack:\n") ;

    while ( pop(stack, &valor_pop) ) 
        printf("%d\n", valor_pop);
    
}