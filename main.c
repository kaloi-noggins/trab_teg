#include <stdio.h>
#include <stdlib.h>
#include "headers/lista.h"
#include "headers/grafo.h"
#include <string.h>
#include <math.h>

// numero de casos no dataset
#define NUM_LINHAS 150
// numero de features no dataset (petal length, petal width, sepal length, sepal width)
#define NUM_ATRIBUTOS 4

int main()
{
    // executa o script de python para fazer o pré tratamento o dataset
    // o pré tratamento consiste na remoção da primeira linha e ultima coluna e
    // a substitução dos , por espaços em branco para tokenização com o strtok
    printf("Fazendo pré processamento do dataset..\n");
    system("python3 scripts/dataset_preprocessing.py");

    // abertura dos arquivos
    FILE *data_set = fopen("arquivos/dataset", "r");
    FILE *distancias_f = fopen("arquivos/distancias_normalizadas.csv", "w");
    FILE *grafo_f = fopen("arquivos/lista_adjacencias.csv", "w");
    FILE *clusters_f = fopen("arquivos/clusters.json", "w");
    FILE *grafo_dot = fopen("arquivos/grafo.dot", "w");

    if (!data_set)
        return -1;

    // matriz de entrada
    float entrada[NUM_LINHAS][NUM_ATRIBUTOS];
    // matriz de distancias
    float distancias[NUM_LINHAS][NUM_LINHAS];
    // minimo e maximo para normalização
    float max = 0;
    float min = 0;
    // grafo
    Grafo *grafo = cria_grafo();

    // leitura do arquivo de entrada e preenchimento da matriz
    // de entrada
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        char linha[30];
        fgets(linha, 30, data_set);
        char *token = strtok(linha, " ");
        int j = 0;
        while (token)
        {
            entrada[i][j] = atof(token);
            token = strtok(NULL, " ");
            j++;
        }
    }

    // calculo das distancias
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
        {
            // calculo da distancia euclidiana entre cada um dos vertices
            float soma = 0;

            for (size_t k = 0; k < NUM_ATRIBUTOS; k++)
                soma += pow(entrada[j][k] - entrada[i][k], 2);

            distancias[i][j] = sqrt(soma);

            // atualiza os minimos e máximos para normalização
            if (distancias[i][j] > max)
                max = distancias[i][j];

            else if (distancias[i][j] < min)
                min = distancias[i][j];
        }
    }

    // normalização das distancias
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
            distancias[i][j] = ((distancias[i][j] - min) / (max - min));
    }

    // numero de clusters
    int num_clusters = 0;
    // numero de iteracoes
    int iteracoes = 0;
    // limiar que sera usado para a construção da lista de adjacencias e delta
    float limiar = 0.15;
    float delta = 0.00001;
    // lista de clusters
    Lista **clusters = (Lista **)malloc(sizeof(Lista *));

    // itera incrementando o limiar com o delta até encontrar os 3 clusters
    while (num_clusters != 4)
    {
        if (iteracoes > 0)
        {
            for (size_t i = 0; i < num_clusters; i++)
            {
                destruir_lista(clusters[i]);
            }
            free(clusters);
            clusters = (Lista **)malloc(sizeof(Lista *));
            destruir_grafo(grafo);
            grafo = cria_grafo();
            num_clusters = 0;
        }

        // Verifica por adjacencias. Como o grafo não é orientado nem ponderado
        // somente a parte inferior a diagonal da matriz é contada
        for (size_t i = 0; i < NUM_LINHAS; i++)
        {
            for (size_t j = 0; j < NUM_LINHAS; j++)
                if (distancias[i][j] <= limiar && i != j)
                    adicionar_aresta(grafo, i, j);
        }

        // Realiza clusterização
        // Lista *maiores[3] = {NULL, NULL, NULL};
        int vertice_inicial = 0;
        Lista *nao_visitados = cria_lista();
        // float centros_geo[3][4];

        for (size_t i = 0; i < 150; i++)
            add_elem_lista(nao_visitados, i);

        while (tamanho(nao_visitados) != 0)
        {
            peek_top(nao_visitados, &vertice_inicial);
            num_clusters += 1;
            Lista *visitados = cria_lista();
            dfs(grafo, vertice_inicial, visitados, nao_visitados);
            clusters = (Lista **)realloc(clusters, sizeof(Lista *) * num_clusters);
            clusters[num_clusters - 1] = visitados;
        }

        iteracoes++;
        limiar -= delta;

        if (iteracoes % 1000 == 0)
            printf("iteracao: %d\nlimiar atual:%f\n", iteracoes, limiar);
    }

    // Persistindo as distâncias e grafo
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
        {
            fprintf(distancias_f, "%f ", distancias[i][j]);
        }
        fprintf(distancias_f, "\n");

        int *temp = adjacencias(grafo, i);

        for (size_t k = 0; k < grau_vertice(grafo, i); k++)
        {
            fprintf(grafo_f, "%ld,%d\n", i, temp[k]);
        }

        free(temp);
    }

    printf("Grafo:\n");
    printa_grafo(grafo);

    printf("Limiar usado: %f\n", limiar);

    // Printa os clusters no terminal e faz arquivo dot para visaulização =
    int *vertices_cluster;
    fprintf(clusters_f, "{\n");
    for (size_t i = 0; i < num_clusters; i++)
    {
        vertices_cluster = conteudo(clusters[i]);

        fprintf(clusters_f, "\"cluster_%ld\":[", i);

        printf("\n--Cluster--\n");
        int temp = 0;
        for (size_t j = 0; j < tamanho(clusters[i]); j++)
        {
            if (j == tamanho(clusters[i]) - 1)
            {
                fprintf(clusters_f, "%d", vertices_cluster[j]);
            }
            else
            {
                fprintf(clusters_f, "%d,", vertices_cluster[j]);
            }

            printf("%d ", vertices_cluster[j]);
            temp++;
        }

        fprintf(clusters_f, "],\n");

        printf("\nTamanho do cluster: %d", temp);
        printf("\n--Fim do cluster--\n");
        free(vertices_cluster);
    }
    fprintf(clusters_f, "}");

    // construção do .dot para display do grafo
    fprintf(grafo_dot, "graph G{\n");
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        int *temp = adjacencias(grafo, i);
        for (size_t j = 0; j < tamanho(grafo->vetor_vertices[i]); j++)
        {
            if (j == tamanho(grafo->vetor_vertices[i]) - 1)
            {
                fprintf(grafo_dot, "   %ld--%d\n", i, temp[j]);
            }
            else
            {
                fprintf(grafo_dot, "   %ld--%d,\n", i, temp[j]);
            }
        }
    }
    fprintf(grafo_dot, "}");

    // executa o script de python que converte o .csv do
    // grafo em um arquivo .dot, que sera usado pelo graphviz
    // para a visualização do grafo
    printf("\nConvertendo a lista de adjacencias no arquivo .dot para visualização...\n");
    // system("python3 scripts/csv_to_dot.py");
    //  utiliza o graphviz para renderizar o grafo gerado
    printf("Construindo visualização do grafo...\n");
    system("neato -x -Goverlap=scale -Tpng arquivos/grafo.dot > arquivos/grafo.png");
    // limpeza dos arquivos temporarios. Descomentar linha abaixo para limpar
    // system("rm arquivos/grafo.dot");
    printf("Pronto! Gráfico está na pasta arquivos!\n");

    return 1;
}