#include <stdlib.h>
#include <stdio.h>
#include "headers/stack.h"

Stack* cria_stack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->topo = NULL;
    return stack;
}

int stack_vazio(Stack* stack) {
    return stack->topo == NULL;
}

int push(Stack* stack, int valor) {
    Nodo_S* novo = (Nodo_S*)malloc(sizeof(Nodo_S));
    if ( novo == NULL )
        return 0;
    
    novo->valor = valor;
    novo->baixo = stack->topo;
    stack->topo = novo;
    return 1;
}

int pop(Stack* stack, int* retorno) {
    if ( stack_vazio(stack) )
        return 0;
    
    *retorno = stack->topo->valor;
    Nodo_S* temp = stack->topo->baixo;
    free(stack->topo);
    stack->topo = temp;
    return 1;
}

void destroi_stack(Stack* stack) {
    Nodo_S* temp = stack->topo;
    Nodo_S* temp1;

    while ( temp != NULL ) {
        temp1 = temp->baixo;
        free(temp);
        temp = temp1;
    }

    free(stack);
} 

void printa_stack(Stack* stack) {
    if ( stack->topo == NULL ) {
        printf("Stack vazio\n");
        return;
    }

    Nodo_S* temp = stack->topo;

    while ( temp != NULL ) {
        printf("%d ", temp->valor);
        temp = temp->baixo;
    }

    printf("\n");
}

int existe_no_stack(Stack* stack, int x) {
    if ( stack->topo == NULL )
        return 0;

    Nodo_S* temp = stack->topo;
    int encontrou = 0;

    while ( temp != NULL && !encontrou ) {
        if ( temp->valor == x )
            encontrou = 1;

        temp = temp->baixo;
    }

    if ( !encontrou )
        return 0;
    
    return 1;
}
