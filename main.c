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

    // limiar que sera usado para a construção da lista de adjacencias
    float limiar = 0.3;

    printf("Entre com o limiar a ser usado para o cáculo das adjacencias (entre 0 e 1, 0.3 por padrão):\n");
    scanf("%f", &limiar);

    // executa o script de python para fazer o pré tratamento o dataset
    // o pré tratamento consiste na remoção da primeira linha e ultima coluna e
    // a substitução dos , por espaços em branco para tokenização com o strtok
    printf("Fazendo pré processamento do dataset..\n");
    system("python3 scripts/dataset_preprocessing.py");

    // abertura dos arquivos
    FILE *data_set = fopen("arquivos/dataset", "r");
    FILE *distancias_f = fopen("arquivos/distancias_normalizadas.csv", "w");
    FILE *grafo_f = fopen("arquivos/lista_adjacencias.csv", "w");

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

    // print entrada
    // printf("Entrada:\n");
    // for (size_t i = 0; i < NUM_LINHAS; i++)
    // {
    //     for (size_t j = 0; j < NUM_ATRIBUTOS; j++)
    //     {
    //         printf("%.2f ", entrada[i][j]);
    //     }
    //     printf("\n");
    // }

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

    // print distancias
    // printf("Distancias:\n");
    // for (size_t i = 0; i < NUM_LINHAS; i++)
    // {
    //     for (size_t j = 0; j < NUM_LINHAS; j++)
    //         printf("%.2f ", distancias[i][j]);

    //     printf("\n");
    // }

    // normalização das distancias
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
            distancias[i][j] = ((distancias[i][j] - min) / (max - min));
    }

    // print distancias normalizada
    // printf("Distancias normalizada:\n");
    // for (size_t i = 0; i < NUM_LINHAS; i++)
    // {
    //     for (size_t j = 0; j < NUM_LINHAS; j++)
    //         printf("%.2f ", distancias[i][j]);

    //     printf("\n");
    // }

    // Verifica por adjacencias. Como o grafo não é orientado nem ponderado
    // somente a parte inferior a diagonal da matriz é contada
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
            if (distancias[i][j] <= limiar && i != j)
                adicionar_aresta(grafo, i, j);
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

    printa_grafo(grafo);
    
    // executa o script de python que converte o .csv do
    // grafo em um arquivo .dot, que sera usado pelo graphviz
    // para a visualização do grafo
    // printf("Convertendo a lista de adjacencias no arquivo .dot para visualização...\n");
    // system("python3 scripts/csv_to_dot.py");
    // // utiliza o graphviz para renderizar o grafo gerado
    // printf("Construindo visualização do grafo...\n");
    // system("neato -x -Goverlap=scale -Tpng arquivos/grafo.dot > arquivos/grafo.png");
    // // limpeza dos arquivos temporarios. Descomentar linha abaixo para limpar
    // // system("rm arquivos/grafo.dot");
    // printf("Pronto! Gráfico está na pasta arquivos!\n");

    // Realiza clusterização
    Lista** clusters = (Lista **)malloc(sizeof(Lista *));
    Lista* maiores[3] = {NULL, NULL, NULL};
    Lista* nao_visitados = cria_lista();
    int vertice_inicial = 0;
    int num_clusters = 0;
    float centros_geo[3][4];
    int *vertices_cluster;

    for (size_t i = 0; i < 150; i++)
        add_elem_lista(nao_visitados, i);

    while (tamanho(nao_visitados) != 0)
    {
        peek_top(nao_visitados, &vertice_inicial);
        num_clusters += 1;
        Lista *visitados = cria_lista();
        dfs(grafo, vertice_inicial, visitados, nao_visitados);
        printf("%s\n", "--Fim iteração--");
        clusters = (Lista **)realloc(clusters, sizeof(Lista *) * num_clusters);
        clusters[num_clusters - 1] = visitados;
    }

    // // Printa os clusters
    // for (size_t i = 0; i < num_clusters; i++)
    // {
    //     vertices_cluster = conteudo(clusters[i]);
    //     printf("\n--Cluster--\n");
    //     int temp = 0;
    //     for (size_t j = 0; j < tamanho(clusters[i]); j++)
    //     {
    //         printf("%d ", vertices_cluster[j]);
    //         temp++;
    //     }
    //     printf("\n--Fim do cluster--\n");
    //     printf("%d\n", temp);
    //     free(vertices_cluster);
    // }

        if ( num_clusters >= 3 ) {
         // Determinando os três maiores clusters
            for (size_t i = 0; i < num_clusters; i++) {
                if ( !maiores[0] || tamanho(clusters[i]) > tamanho(maiores[0]) )
                    maiores[0] = clusters[i];

                else if ( !maiores[1] || tamanho(clusters[i]) > tamanho(maiores[1]) )
                    maiores[1] = clusters[i];

                else if ( !maiores[2] || tamanho(clusters[i]) > tamanho(maiores[2]) )
                    maiores[2] = clusters[i];
         }

         // Determinando os centros geométricos dos três maiores clusters
            for (size_t i = 0; i < 3; i++) {
            vertices_cluster = conteudo(maiores[i]);
            int tamanho_cluster = tamanho(maiores[i]);
            // Percorrendo vértices do cluster
            for (size_t j = 0; j < tamanho_cluster; j++) {
                // Somando atributos dos vértices
                for (size_t k = 0; k < NUM_ATRIBUTOS; k++)
                    centros_geo[i][k] += entrada[vertices_cluster[j]][k];
                // Divindo pela quantidade de vértices, determinando a média
                for (size_t k = 0; k < NUM_ATRIBUTOS; k++)
                    centros_geo[i][k] = centros_geo[i][k] / tamanho_cluster;

             }
         }

         for (size_t i = 0; i < 3; i++) {
            printf("Centro geométrico %ld: ", i);
            for (size_t j = 0; j < 4; j++) {
                printf("%f ", centros_geo[i][j]);
            }
            printf("\n");
         }
     }

    return 1;
}