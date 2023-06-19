#ifndef LISTA
#define LISTA

typedef struct Nodo {
    int vertice;
    struct Nodo *proximo;
    struct Nodo *anterior;
} Nodo;

typedef struct {
    Nodo *comeco;
    int tamanho;
} Lista;

Lista *cria_lista();
void add_elem_lista(Lista *lista, int x);
int *conteudo(Lista *lista);
int tamanho(Lista* lista);
void destruir_lista(Lista* lista);
int existe_na_lista(Lista* lista, int x);
int remover_elem_lista(Lista *lista, int x);
int peek_top(Lista* lista, int* retorno);
void printa_lista(Lista* lista);

#endif