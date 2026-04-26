#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {

    // Revision de input
    if (argc < 2) 
    {
        printf("Error: Entrege en terminal el nombre del arhcivo.txt a leer\n");
        return 1;
    }

    // Lectura y validacion de archivo
    FILE * archivo = fopen(argv[1], "r");
    
    if (archivo == NULL) 
    {
        printf("El archivo no existe o no se puede leer.\n");
        return 1;
    }

    printf("Archivo abierto exitosamente\n");

    char** nombres;
    int* prioridades;
    float* tiempos;
    int size;

    // Se indican todas las tareas presentes a procesar
    fscanf(archivo, "%i", &size);

    // Se asigna a cada variable del archivo su memoria
    nombres = (char**) malloc(size * sizeof(char*));
    prioridades = (int*) malloc(size * sizeof(int));
    tiempos = (float*) malloc(size * sizeof(float));

    char nombre_tarea[8];

    // Rellena la memoria alocada con los valores del archivo
    for (int i = 0; i < size; i++) 
    {
        fscanf(archivo, "%s", nombre_tarea);
        fscanf(archivo, "%d", &prioridades[i]);
        fscanf(archivo, "%f", &tiempos[i]);

        nombres[i] = (char*) malloc((strlen(nombre_tarea) + 1) * sizeof(char));
        strcpy(nombres[i], nombre_tarea);
    }

    fclose(archivo);

    // Ordenamos el Heap por prioridad
    for (int i = 0; i < size - 1; i++) 
    {
        for (int j = i + 1; j < size - 1; j++) 
        {
            if (prioridades[i] > prioridades[j]) 
            {
                char* temp_n = nombres[i];
                nombres[i] = nombres[j];
                nombres[j] = temp_n;

                int temp_p = prioridades[i];
                prioridades[i] = prioridades[j];
                prioridades[j] = temp_p;

                float temp_t = tiempos[i];
                tiempos[i] = tiempos[j];
                tiempos[j] = temp_t;
            }
        }
    }

    for (int i = 0; i < size; i++) 
    {
        printf("Tarea: %s, Prioridad: %d, Tiempo: %.2f\n",
        nombres[i], prioridades[i], tiempos[i]);
    }

    for (int i = 0; i < size; i++) 
    {
        free(nombres[i]);
    }
    free(nombres);
    free(prioridades);
    free(tiempos);

    return 0;
}