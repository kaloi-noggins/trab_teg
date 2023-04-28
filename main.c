#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "grafo.h"
#include <string.h>
#include <math.h>

int main() {

    FILE *data_set = fopen("IrisDataset.csv", "r");
    FILE *distancias_f = fopen("distancias.csv", "w");
    FILE *grafo_f = fopen("grafo.csv", "w");

    if ( !data_set )
        return -1;
    
    int i, j, w;
    float entrada[150][4];
    float distancias[150][150];
    float max = 0;
    int min = 0;
    float soma;
    char linha[30];
    char delim = ',';
    char *token = NULL;
    Grafo *grafo = cria_grafo();
    
    for(i = 0; i < 150; i++) {
        token = strtok(fgets(linha, 30, data_set), &delim);
        j = 0;
        while( token != NULL ) {
            entrada[i][j] = atof(token);
            token = strtok(NULL, &delim);
            j++;
        }   
    }

    // Calcula distâncias entre vetores
    for(i = 0; i < 150; i++) {
        for(j = i + 1; j < 150; j++) {
            soma = 0;

            for(w = 0; w < 4; w++) 
              soma += pow(entrada[j][w] - entrada[i][w], 2);

            distancias[i][j] = sqrt(soma);
            
            if ( distancias[i][j] > max )
                max = distancias[i][j];
            
            else if ( distancias[i][j] < min )
                min = distancias[i][j];
            
        }
    }

    // Normaliza distâncias
    for(i = 0; i < 150; i++) 
        for(j = i + 1; j < 150; j++)
            distancias[i][j] = (distancias[i][j] - min) / (max - min);
    
    // Verifica por adjacencias
    for(i = 0; i < 150; i++) 
        for(j = i + 1; j < 150; j++)
            if ( distancias[i][j] <= 0.02 )
                adicionar_aresta(grafo, i, j);
    
    // Persistindo as distâncias e grafo
    for(i = 0; i < 150; i++) {

        for(j = i + 1; j < 150; j++) 
            fprintf(distancias_f, "%d,%d,%f\n", i, j, distancias[i][j]);

        int *temp = adjacencias(grafo, i);
        
        for(w = 0; w < grau_vertice(grafo, i); w++)
            fprintf(grafo_f, "%d,%d\n", i, temp[w]);
        
        free(temp);
    }

    return 1;
    
}