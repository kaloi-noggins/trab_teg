#include <stdlib.h>
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
    stack->topo = stack->topo->baixo;
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