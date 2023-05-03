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
    system("python scripts/dataset_preprocessing.py");

    // abertura dos arquivos
    FILE *data_set = fopen("arquivos/dataset", "r");
    FILE *distancias_f = fopen("arquivos/distancias.csv", "w");
    FILE *grafo_f = fopen("arquivos/grafo.csv", "w");

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
    printf("Entrada:\n");
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_ATRIBUTOS; j++)
        {
            printf("%.2f ", entrada[i][j]);
        }
        printf("\n");
    }

    // calculo das distancias
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
        {
            if (i == j)
            {
                // a distance de um vertice a ele mesmo é 0
                distancias[i][j] = 0;
            }
            else if (i > j)
            {
                // como o grafo não é orientado nem ponderado, as distancias
                // acima e abaixo da diagonal são iguais
                distancias[i][j] = distancias[j][i];
            }
            else
            {
                // calculo da distancia euclidiana entre cada um dos vertices
                float soma = 0;
                for (size_t k = 0; k < NUM_ATRIBUTOS; k++)
                {
                    soma += pow(entrada[j][k] - entrada[i][k], 2);
                }

                distancias[i][j] = sqrt(soma);

                // atualiza os minimos e máximos para normalização
                if (distancias[i][j] > max)
                    max = distancias[i][j];

                else if (distancias[i][j] < min)
                    min = distancias[i][j];
            }
        }
    }

    // print distancias
    printf("Distancias:\n");
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
        {
            printf("%.2f ", distancias[i][j]);
        }
        printf("\n");
    }

    // normalização das distancias
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
        {
            distancias[i][j] = ((distancias[i][j] - min) / (max - min));
        }
    }

    // print distancias normalizada
    printf("Distancias normalizada:\n");
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = 0; j < NUM_LINHAS; j++)
        {
            printf("%.2f ", distancias[i][j]);
        }
        printf("\n");
    }

    // Verifica por adjacencias. Como o grafo não é orientado nem ponderado
    // somente a parte inferior a diagonal da matriz é contada
    for (size_t i = 0; i < NUM_LINHAS; i++)
    {
        for (size_t j = i + 1; j < NUM_LINHAS; j++)
        {
            if (distancias[i][j] <= 0.3)
            {
                adicionar_aresta(grafo, i, j);
            }
        }
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
            fprintf(grafo_f, "%d,%d\n", i, temp[k]);
        }

        free(temp);
    }

    // executa o script de python que converte o .csv do
    // grafo em um arquivo .dot, que sera usado pelo graphviz
    // para a visualização do grafo
    system("python scripts/csv_to_dot.py");
    // utiliza o graphviz para renderizar o grafo gerado
    system("neato -x -Goverlap=scale -Tpng arquivos/grafo.dot > arquivos/grafo.png");
    // limpeza dos arquivos temporarios. Descomentar linha abaixo para limpar
    //system("rm arquivos/dataset arquivos/grafo.dot");

    return 1;
}