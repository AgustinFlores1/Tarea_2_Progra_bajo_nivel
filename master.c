#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int insertar (char*** nombres, int** prioridades, float** tiempos, int* size, char* nombre, int prioridad, float tiempo) 
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

    strcpy((*nombres)[posicion], nombre);
    (*prioridades)[posicion] = prioridad;
    (*tiempos)[posicion] = tiempo;

    return posicion;
}

int eliminar (char*** nombres, int** prioridades, float** tiempos, int* size, char* nombre) 
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

    for (int i = posicion_eliminada; i < (*size) - 1; i++) 
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

int buscar (char** nombres, int size, char* nombre)
{
    int posicion = -1;

    for (int i = 0; i < size; i++) 
    {
        if (strcmp((nombres)[i], nombre) == 0) 
        {
            posicion = i;
            break;
        }
    } 

    return posicion;
}

float ejecutar (char*** nombres, int** prioridades, float** tiempos, int* size)
{
    if ((*size) <= 0) 
    {
        return 0.0;
    }

    free((*nombres)[0]);

    float tiempo_ejecucion = (*tiempos)[0];

    for (int i = 0; i < (*size) - 1; i++) 
    {
        (*nombres)[i] = (*nombres)[i + 1];
        (*prioridades)[i] = (*prioridades)[i + 1];
        (*tiempos)[i] = (*tiempos)[i + 1];
    }

    (*nombres)[(*size) - 1] = NULL;

    (*size) = (*size) - 1;

    *nombres = (char**) realloc(*nombres, (*size) * sizeof(char*));
    *prioridades = (int*) realloc(*prioridades, (*size) * sizeof(int));
    *tiempos = (float*) realloc(*tiempos, (*size) * sizeof(float));

    return tiempo_ejecucion;
}

int liberar(char*** nombres, int** prioridades, float** tiempos, int* size)
{
    if (nombres == NULL || prioridades == NULL || tiempos == NULL)
    {
        return 0;
    }

    free(*nombres);
    free(*prioridades);
    free(*tiempos);

    *nombres = NULL;
    *prioridades = NULL;
    *tiempos = NULL;
    *size = 0;

    return 1;
}

int main() 
{
    // Variables para el tiempo de ejecucion
    clock_t start, end;
    double tiempo;

    // Lectura y validacion de archivo
    FILE * entrada = fopen("entrada.txt", "r");

    if (entrada == NULL)
    {
        printf("El archivo no existe o no se puede leer.\n");
        return 1;
    }

    char** nombres;
    int* prioridades;
    float* tiempos;
    int size = 0;

    // Se indican todas las tareas presentes a procesar
    fscanf(entrada, "%i", &size);

    // Se asigna a cada variable del archivo su memoria
    nombres = (char**) malloc(size * sizeof(char*));
    prioridades = (int*) malloc(size * sizeof(int));
    tiempos = (float*) malloc(size * sizeof(float));

    if (nombres == NULL || prioridades == NULL || tiempos == NULL)
    {
        printf("Error: No hay suficiente memoria.\n");
        liberar(&nombres, &prioridades, &tiempos, &size);

        return 1;
    }

    // Se rellenan las direcciones para los nombres en NULL
    for (int i = 0; i < size; i++) 
    {
        nombres[i] = NULL;
    }

    // Creacion archivo de salida
    FILE * salida = fopen("salida.txt", "w");

    // Inserta tareas en la lista de tareas segun prioridad
    for (int i = 0; i < size; i++) 
    {
        char nombre_tarea[20];
        int prioridad_tarea;
        float tiempo_tarea;

        fscanf(entrada, "%s", nombre_tarea);
        fscanf(entrada, "%i", &prioridad_tarea);
        fscanf(entrada, "%f", &tiempo_tarea);

        start = clock();
        int posicion = insertar(&nombres, &prioridades, &tiempos, &size, nombre_tarea, prioridad_tarea, tiempo_tarea);
        end = clock();

        double tiempo_individual = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

        fprintf(salida, "INSERT %s %f %i\n", nombre_tarea, tiempo_individual, posicion);

        tiempo += tiempo_individual;
    }


    // Se indica el numero de tareas a eliminar
    int tareas_eliminar = 0;
    fscanf(entrada, "%i", &tareas_eliminar);

    // Se eliminan las tareas
    for (int i = 0; i < tareas_eliminar; i++) 
    {
        char nombre_tarea[20];
        fscanf(entrada, "%s", nombre_tarea);

        start = clock();
        int posicion = eliminar(&nombres, &prioridades, &tiempos, &size, nombre_tarea);
        end = clock();

        double tiempo_individual = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
        tiempo += tiempo_individual;

        if (posicion == -1)
        {
            printf("No se encontro la tarea: %s\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "DELETE %s %f %i\n", nombre_tarea, tiempo_individual, posicion);
        }
    }


    // Se indica el numero de tareas a buscar
    int tareas_buscar = 0;
    fscanf(entrada, "%i", &tareas_buscar);

    // Se buscan las tareas
    for (int i = 0; i < tareas_buscar; i++)
    {
        char nombre_tarea[20];
        fscanf(entrada, "%s", nombre_tarea);

        start = clock();
        int posicion = buscar(nombres, size, nombre_tarea);
        end = clock();

        double tiempo_individual = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
        tiempo += tiempo_individual;

        if (posicion == -1)
        {
            printf("No se encontro la tarea: %s\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "SEARCH %s %f %i\n", nombre_tarea, tiempo_individual, posicion);
        }
    }

    // Se ejecutan las tareas
    float tiempo_ejecutadas = 0;
    int n_ejecutadas = 0;
    while (size > 0)
    {
        n_ejecutadas++;

        char nombre_tarea[20];
        strcpy(nombre_tarea, nombres[0]);
        
        start = clock();
        float tiempo_tarea = ejecutar(&nombres, &prioridades, &tiempos, &size);
        end = clock();

        tiempo_ejecutadas += tiempo_tarea;

        double tiempo_individual = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
        tiempo += tiempo_individual;

        fprintf(salida, "EXECUTE %s %f %i\n", nombre_tarea, tiempo_individual, (int)tiempo_tarea);
    }

    // Se imprime al archivo el numero de tareas ejecutadas y los tiempos
    fprintf(salida, "%i\n", n_ejecutadas);
    fprintf(salida, "%f %f", (tiempo + tiempo_ejecutadas), tiempo);

    fclose(entrada);
    fclose(salida);

    // Se libera memoria y se valida si fue exitosa la liberacion
    int valido = liberar(&nombres, &prioridades, &tiempos, &size);

    return valido;
}