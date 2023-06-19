#ifndef STACK
#define STACK

typedef struct Nodo_S {
    struct Nodo_S* baixo;
    int valor;
} Nodo_S;

typedef struct {
    Nodo_S* topo;
} Stack;

Stack* cria_stack();
int push(Stack* stack, int valor);
int pop(Stack* stack, int* retorno);
int stack_vazio(Stack* stack);
int existe_no_stack(Stack* stack, int x);
void printa_stack(Stack* stack);
#endif