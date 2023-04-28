#ifndef LISTA
#define LISTA

typedef struct Nodo {
    int vertice;
    struct Nodo *proximo;
} Nodo;

typedef struct {
    Nodo *comeco;
    int tamanho;
} Lista;

Lista *cria_lista();
void add_elem_lista(Lista *lista, int x);
int *conteudo(Lista *lista);
int tamanho(Lista* lista);

#endif