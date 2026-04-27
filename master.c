#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int insertar(char*** nombres, int** prioridades, float** tiempos, int* size, char* nombre, int prioridad, float tiempo) 
{
    int tareas_en_lista = 0;
    while (tareas_en_lista < (*size) && (*nombres)[tareas_en_lista] != NULL) 
    {
        tareas_en_lista++;
    }

    int posicion = 0;
    while (posicion < tareas_en_lista && prioridad >= (*prioridades)[posicion]) 
    {
        posicion++;
    }

    for (int i = tareas_en_lista; i > posicion; i--) 
    {
        (*nombres)[i] = (*nombres)[i - 1];
        (*prioridades)[i] = (*prioridades)[i - 1];
        (*tiempos)[i] = (*tiempos)[i - 1];
    }
    
    (*nombres)[posicion] = (char*) malloc((strlen(nombre) + 1) * sizeof(char));

    if ((*nombres)[posicion] == NULL)
    {
        exit(1);
    }

    (*prioridades)[posicion] = prioridad;
    (*tiempos)[posicion] = tiempo;
    strcpy((*nombres)[posicion], nombre);

    return posicion;
}

int eliminar(char*** nombres, int** prioridades, float** tiempos, int* size, char* nombre) 
{
    int posicion_eliminada = -1;

    for (int i = 0; i < (*size); i++) 
    {
        if (strcmp((*nombres)[i], nombre) == 0) 
        {
            posicion_eliminada = i;
            break;
        }
    }

    if (posicion_eliminada == -1) 
    {
        return -1;
    }

    free((*nombres)[posicion_eliminada]);

    for(int i = posicion_eliminada; i < (*size) - 1; i++) 
    {
        (*nombres)[i] = (*nombres)[i + 1];
        (*prioridades)[i] = (*prioridades)[i + 1];
        (*tiempos)[i] = (*tiempos)[i + 1];
    }

    (*size) = (*size) - 1;

    *nombres = (char**) realloc(*nombres, (*size) * sizeof(char*));
    *prioridades = (int*) realloc(*prioridades, (*size) * sizeof(int));
    *tiempos = (float*) realloc(*tiempos, (*size) * sizeof(float));

    if (*nombres == NULL || *prioridades == NULL || *tiempos == NULL)
    {
        printf("Error: Perdida de memoria o falta de memoria encontrada.\n");
        exit(1);
    }

    return posicion_eliminada;
}

int main(int argc, char ** argv) {

    // Revision de input
    if (argc < 2) 
    {
        printf("Error: Entrege en terminal el nombre del archivo.txt como argumento.\n");
        return 1;
    }

    // Lectura y validacion de archivo
    FILE * archivo = fopen(argv[1], "r");
    
    if (archivo == NULL) 
    {
        printf("El archivo no existe o no se puede leer.\n");
        return 1;
    }

    printf("Archivo abierto exitosamente\n\n");

    char** nombres;
    int* prioridades;
    float* tiempos;
    int size = 0;

    // Se indican todas las tareas presentes a procesar
    fscanf(archivo, "%i", &size);

    // Se asigna a cada variable del archivo su memoria
    nombres = (char**) malloc(size * sizeof(char*));
    prioridades = (int*) malloc(size * sizeof(int));
    tiempos = (float*) malloc(size * sizeof(float));

    if (nombres == NULL || prioridades == NULL || tiempos == NULL)
    {
        printf("Error: No hay suficiente memoria.\n");
        return 1;
    }

    for (int i = 0; i < size; i++) 
    {
        nombres[i] = NULL;
    }

    // Rellena memoria con valores del archivo de tareas
    for (int i = 0; i < size; i++) 
    {
        char nombre_tarea[20];
        int prioridad_tarea;
        float tiempo_tarea;

        fscanf(archivo, "%s", nombre_tarea);
        fscanf(archivo, "%i", &prioridad_tarea);
        fscanf(archivo, "%f", &tiempo_tarea);

        int posicion = insertar(&nombres, &prioridades, &tiempos, &size, nombre_tarea, prioridad_tarea, tiempo_tarea);

        printf("Tarea insertada: %s, Posicion: %i\n", nombre_tarea, posicion);
    }

    printf("\n");

    int tareas_eliminar = 0;
    fscanf(archivo, "%i", &tareas_eliminar);

    for (int i = 0; i < tareas_eliminar; i++) 
    {
        char nombre_tarea[20];

        fscanf(archivo, "%s", nombre_tarea);

        int posicion = eliminar(&nombres, &prioridades, &tiempos, &size, nombre_tarea);

        if (posicion == -1)
        {
            printf("No se enontro la tarea: %s\n", nombre_tarea);
        } 
        else 
        {
            printf("Tarea eliminada: %s, Posicion: %i\n", nombre_tarea, posicion);
        }
    }

    fclose(archivo);

    printf("\nTAREAS A REALIZAR \n");
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