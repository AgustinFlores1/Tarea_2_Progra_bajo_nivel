#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -------------------------DEFINIR STRUCT--------------------------
typedef struct 
{
    char* nombre;
    int prioridad;
    float tiempo;
} Tarea;

// ------------------------FUNCIONES AUXILIARES----------------------
double get_time (clock_t start, clock_t end) 
{
    return (((double)(end - start) * 1000.0) / CLOCKS_PER_SEC);
}

Tarea create_task(char* nombre, int prioridad, float tiempo)
{
    Tarea task;

    task.nombre = (char*) malloc(strlen(nombre) + 1);
    strcpy(task.nombre, nombre);
    task.prioridad = prioridad;
    task.tiempo = tiempo;

    return task;
}

// ------------------------INSERT------------------------------------
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

int insertar_structs (Tarea** lista_tareas, Tarea tarea_insrt, int* list_size) 
{
    int tareas_en_lista = 0;
    while (tareas_en_lista < (*list_size) && (*lista_tareas)[tareas_en_lista].nombre != NULL)
    {
        tareas_en_lista++;
    }

    int posicion = 0;
    while (posicion < tareas_en_lista && (tarea_insrt.prioridad >= (*lista_tareas)[posicion].prioridad))
    {
        posicion++;
    }

    for (int i = tareas_en_lista; i > posicion; i--)
    {
        (*lista_tareas)[i] = (*lista_tareas)[i - 1];
    }

    (*lista_tareas)[posicion] = tarea_insrt;

    return posicion;
}

// ------------------------DELETE------------------------------------
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

    (*size)--;

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

int eliminar_structs (Tarea** lista_tareas, int* list_size ,char* nombre)
{
    int posicion_eliminada = -1;
    for (int i = 0; i < (*list_size); i++)
    {
        if (strcmp((*lista_tareas)[i].nombre, nombre) == 0)
        {
            posicion_eliminada = i;
            break;
        }
    }

    if (posicion_eliminada == -1)
    {
        return -1;
    }

    free((*lista_tareas)[posicion_eliminada].nombre);

    for (int i = posicion_eliminada; i < (*list_size) - 1; i++)
    {
        (*lista_tareas)[i] = (*lista_tareas)[i + 1];
    }

    (*list_size)--;

    (*lista_tareas) = (Tarea*) realloc((*lista_tareas), (*list_size) * sizeof(Tarea));

    return posicion_eliminada;
}

// -----------------------SEARCH-------------------------------------
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

int buscar_structs (Tarea* lista_tareas, int list_size, char* nombre)
{
    int posicion = -1;
    for (int i = 0; i < list_size; i++)
    {
        if (strcmp(lista_tareas[i].nombre, nombre) == 0)
        {
            posicion = i;
            break;
        }
    }

    return posicion;
}

// -----------------------EXECUTE-----------------------------------
float ejecutar (char*** nombres, int** prioridades, float** tiempos, int* size)
{
    float tiempo_ejecucion = (*tiempos)[0];

    if ((*size) <= 0) 
    {
        return 0.0;
    }

    free((*nombres)[0]);

    for (int i = 0; i < (*size) - 1; i++) 
    {
        (*nombres)[i] = (*nombres)[i + 1];
        (*prioridades)[i] = (*prioridades)[i + 1];
        (*tiempos)[i] = (*tiempos)[i + 1];
    }

    (*size)--;

    *nombres = (char**) realloc(*nombres, (*size) * sizeof(char*));
    *prioridades = (int*) realloc(*prioridades, (*size) * sizeof(int));
    *tiempos = (float*) realloc(*tiempos, (*size) * sizeof(float));

    return tiempo_ejecucion;
}

float ejecutar_structs (Tarea** lista_tareas, int* list_size)
{
    float tiempo_ejecucion = (*lista_tareas)[0].tiempo;

    if ((*list_size) <= 0)
    {
        return 0.0;
    }

    free((*lista_tareas)[0].nombre);

    for (int i = 0; i < (*list_size) - 1; i++)
    {
        (*lista_tareas)[i] = (*lista_tareas)[i + 1];
    }

    (*list_size)--;

    (*lista_tareas) = (Tarea*) realloc((*lista_tareas), (*list_size) * sizeof(Tarea));

    return tiempo_ejecucion;
}

// ------------------------FREE--------------------------------------
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

int liberar_structs (Tarea** lista_tareas, int * size_struct) 
{
    if (lista_tareas == NULL) 
    {
        return 0;
    }

    for (int i = 0; i < (*size_struct); i++)
    {
        free((*lista_tareas)[i].nombre);
    }
    free(*lista_tareas);
    *lista_tareas = NULL;
    *size_struct = 0;

    return 1;
}

// -----------------------VALIDAR-----------------------------------
int validar(Tarea* lista_tareas, int size_struct, char** nombres, int* prioridades, float* tiempos, int size_ptr)
{
    if (size_ptr != size_struct)
    {
        return 0;
    }

    for (int i = 0; i < size_struct; i++)
    {
        if (lista_tareas[i].nombre == NULL || nombres[i] == NULL)
        {
            if (lista_tareas[i].nombre != nombres[i]) 
            {
                return 0;
            }
        }
        else if (strcmp(lista_tareas[i].nombre, nombres[i]) != 0 || lista_tareas[i].prioridad != prioridades[i] || lista_tareas[i].tiempo != tiempos[i])
        {
            return 0;
        }
    }

    return 1;
}

// -----------------------MAIN--------------------------------------
int main() 
{
    // Variables para el tiempo de ejecucion
    clock_t start, end;
    double tiempo_ptrs;
    double tiempo_structs;

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
    Tarea* lista_tareas;

    // Se indican todas las tareas presentes a procesar
    fscanf(entrada, "%i", &size);
    int struct_list_size = size;

    // Se asigna a cada variable del archivo su memoria
    nombres = (char**) calloc(size, sizeof(char*));
    prioridades = (int*) calloc(size, sizeof(int));
    tiempos = (float*) calloc(size, sizeof(float));
    lista_tareas = (Tarea*) calloc(struct_list_size, sizeof(Tarea));

    if (nombres == NULL || prioridades == NULL || tiempos == NULL || lista_tareas == NULL)
    {
        printf("Error: No hay suficiente memoria.\n");
        return 1;
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

        Tarea tarea_insrt = create_task(nombre_tarea, prioridad_tarea, tiempo_tarea);

        start = clock();
        int posicion_ptrs = insertar(&nombres, &prioridades, &tiempos, &size, nombre_tarea, prioridad_tarea, tiempo_tarea);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        start = clock();
        int posicion_structs = insertar_structs(&lista_tareas, tarea_insrt, &struct_list_size);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        fprintf(salida, "INSERT %s PUNTEROS %f %i\n", nombre_tarea, actual_time_ptrs, posicion_ptrs);
        fprintf(salida, "INSERT %s STRUCTS %f %i\n", tarea_insrt.nombre, actual_time_structs, posicion_structs);
        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, size));
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
        int posicion_ptrs = eliminar(&nombres, &prioridades, &tiempos, &size, nombre_tarea);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        start = clock();
        int posicion_structs = eliminar_structs(&lista_tareas, &struct_list_size, nombre_tarea);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        if (posicion_ptrs == -1)
        {
            fprintf(salida, "DELETE %s PUNTEROS: No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "DELETE %s PUNTEROS %f %i\n", nombre_tarea, actual_time_ptrs, posicion_ptrs);
        }

        if (posicion_structs == -1)
        {
            fprintf(salida, "DELETE %s STRUCTS: No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "DELETE %s STRUCTS %f %i\n", nombre_tarea, actual_time_structs, posicion_structs);
        }

        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, size));
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
        int posicion_ptrs = buscar(nombres, size, nombre_tarea);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        start = clock();
        int posicion_structs = buscar_structs(lista_tareas, struct_list_size, nombre_tarea);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        if (posicion_ptrs == -1)
        {
            fprintf(salida, "SEARCH %s PUNTEROS No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "SEARCH %s PUNTEROS %f %i\n", nombre_tarea, actual_time_ptrs, posicion_ptrs);
        }

        if (posicion_structs == -1)
        {
            fprintf(salida, "SEARCH %s STRUCTS No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "SEARCH %s STRUCTS %f %i\n", nombre_tarea, actual_time_structs, posicion_structs);
        }

        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, size));
    }

    // Se ejecutan las tareas
    float ptrs_exec_time = 0;
    float structs_exec_time = 0;
    int n_ejecutadas = 0;
    while (size > 0)
    {
        n_ejecutadas++;

        char nombre_ptrs[20];
        strcpy(nombre_ptrs, nombres[0]);

        char nombre_structs[20];
        strcpy(nombre_structs, lista_tareas[0].nombre);
        
        start = clock();
        float tiempo_tarea_ptrs = ejecutar(&nombres, &prioridades, &tiempos, &size);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        start = clock();
        float tiempo_tarea_structs = ejecutar_structs(&lista_tareas, &struct_list_size);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        fprintf(salida, "EXECUTE %s PUNTEROS %f %i\n", nombre_ptrs, actual_time_ptrs, (int)tiempo_tarea_ptrs);
        ptrs_exec_time += tiempo_tarea_ptrs;

        fprintf(salida, "EXECUTE %s STRUCTS %f %i\n", nombre_ptrs, actual_time_ptrs, (int)tiempo_tarea_ptrs);
        structs_exec_time += tiempo_tarea_structs;

        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, size));
    }

    // Se imprime al archivo el numero de tareas ejecutadas y los tiempos
    fprintf(salida, "%i\n", n_ejecutadas);
    fprintf(salida, "PUNTEROS %f %f\n", (tiempo_ptrs + ptrs_exec_time), tiempo_ptrs);
    fprintf(salida, "STRUCTS %f %f\n", (tiempo_structs + structs_exec_time), tiempo_structs);

    fclose(entrada);
    fclose(salida);

    // Se libera memoria y se valida si fue exitosa la liberacion
    int lib_ptrs = liberar(&nombres, &prioridades, &tiempos, &size);
    int lib_structs = liberar_structs(&lista_tareas, &struct_list_size);

    if (!lib_ptrs || !lib_structs)
    {
        printf("Error: No se pudo liberar correctamente la memoria\n");
        return 1;
    }
    else
    {
        return 0;
    }
}