#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -------------------------DEFINIR STRUCT---------------------||

typedef struct 
{
    char* nombre;
    int prioridad;
    float tiempo;
} Tarea;

// ------------------------FUNCIONES AUXILIARES----------------||

double get_time (clock_t start, clock_t end) // Para calcular el tiempo con clock()
{
    return (((double)(end - start) * 1000.0) / CLOCKS_PER_SEC);
}

Tarea create_task(char* nombre, int prioridad, float tiempo) // Crear una tarea cualquiera
{
    Tarea task;

    task.nombre = (char*) malloc(strlen(nombre) + 1);
    strcpy(task.nombre, nombre);
    task.prioridad = prioridad;
    task.tiempo = tiempo;

    return task;
}

// ------------------------INSERT------------------------------||

int insertar (char*** nombres, int** prioridades, float** tiempos, int* size, char* nombre, int prioridad, float tiempo) 
{
    int tareas_en_lista = 0;
    while (tareas_en_lista < (*size) && (*nombres)[tareas_en_lista] != NULL) 
    {
        tareas_en_lista++;
    }

    // Se revisa la posicion que acorde a prioridad y si no encuentra va a la ultima posicion disponible
    int posicion = 0;
    while (posicion < tareas_en_lista && prioridad >= (*prioridades)[posicion]) 
    {
        posicion++;
    }

    // Mueve todas las tareas para abajo de la lista para hacer hueco
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

    // Se inserta la tarea en el hueco
    strcpy((*nombres)[posicion], nombre);
    (*prioridades)[posicion] = prioridad;
    (*tiempos)[posicion] = tiempo;

    return posicion;
}

int insertar_structs (Tarea** lista_tareas, Tarea tarea_insrt, int* list_size) // Misma logica que anterior pero para tareas
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

// ------------------------SEARCH------------------------------||

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

int buscar_structs (Tarea* lista_tareas, int list_size, char* nombre) // Misma logica que anterior pero para tareas
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


// ------------------------DELETE------------------------------||

int eliminar (char*** nombres, int** prioridades, float** tiempos, int* size, char* nombre) 
{
    int posicion_eliminada = buscar(*nombres, *size, nombre);

    // Si no se encuentra se queda en -1
    if (posicion_eliminada == -1) 
    {
        return -1;
    }

    free((*nombres)[posicion_eliminada]);

    // Se mueven todas las tareas para arriba de la lista para llenart el hueco
    for (int i = posicion_eliminada; i < (*size) - 1; i++) 
    {
        (*nombres)[i] = (*nombres)[i + 1];
        (*prioridades)[i] = (*prioridades)[i + 1];
        (*tiempos)[i] = (*tiempos)[i + 1];
    }

    // Se disminuye el tamaño de la lista y se hace realloc para usar menos bloques de memoria Heap
    (*size)--;

    *nombres = (char**) realloc(*nombres, (*size) * sizeof(char*));
    *prioridades = (int*) realloc(*prioridades, (*size) * sizeof(int));
    *tiempos = (float*) realloc(*tiempos, (*size) * sizeof(float));

    // Se asegura que se entrego la memoria
    if (*nombres == NULL || *prioridades == NULL || *tiempos == NULL)
    {
        printf("Error: Perdida de memoria o falta de memoria encontrada.\n");
        exit(1);
    }

    return posicion_eliminada;
}

int eliminar_structs (Tarea** lista_tareas, int* list_size ,char* nombre) // Misma logica que anterior pero para tareas
{
    int posicion_eliminada = buscar_structs((*lista_tareas), (*list_size), nombre);

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

// ------------------------EXECUTE-----------------------------||

float ejecutar (char*** nombres, int** prioridades, float** tiempos, int* size)
{
    float tiempo_ejecucion = (*tiempos)[0];

    if ((*size) <= 0) 
    {
        return 0.0;
    }

    free((*nombres)[0]);

    // Se mueven las tareas para arriba de la lista (efectivamente borrando el primer item)
    for (int i = 0; i < (*size) - 1; i++) 
    {
        (*nombres)[i] = (*nombres)[i + 1];
        (*prioridades)[i] = (*prioridades)[i + 1];
        (*tiempos)[i] = (*tiempos)[i + 1];
    }

    // Disminuye el tamaño y se hace realloc para ocupar menos memoria en Heap
    (*size)--;

    *nombres = (char**) realloc(*nombres, (*size) * sizeof(char*));
    *prioridades = (int*) realloc(*prioridades, (*size) * sizeof(int));
    *tiempos = (float*) realloc(*tiempos, (*size) * sizeof(float));

    return tiempo_ejecucion;
}

float ejecutar_structs (Tarea** lista_tareas, int* list_size) // Misma logica que anterior pero para tareas
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


// ------------------------FREE--------------------------------||

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

int liberar_structs (Tarea** lista_tareas, int * size_struct)  // Misma logica que anterior pero para tareas
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


// ------------------------VALIDAR-----------------------------||

int validar(Tarea* lista_tareas, int size_struct, char** nombres, int* prioridades, float* tiempos, int size_ptr)
{
    // No son iguales sin los mismos tamaños
    if (size_ptr != size_struct)
    {
        return 0;
    }

    for (int i = 0; i < size_struct; i++)
    {
        // Se evalua si son NULL para que no falle strcmp()
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


// ------------------------MAIN--------------------------------||
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
    int ptr_size = 0;
    Tarea* lista_tareas;

    // Se indican todas las tareas presentes a insertar
    fscanf(entrada, "%i", &ptr_size);
    int struct_list_size = ptr_size;

    // Se asigna a cada variable del archivo su memoria (Calloc para llenar de ceros en ves de que hayan cosas al azar)
    nombres = (char**) calloc(ptr_size, sizeof(char*));
    prioridades = (int*) calloc(ptr_size, sizeof(int));
    tiempos = (float*) calloc(ptr_size, sizeof(float));
    lista_tareas = (Tarea*) calloc(struct_list_size, sizeof(Tarea));

    // Confirmar que se entrego memoria
    if (nombres == NULL || prioridades == NULL || tiempos == NULL || lista_tareas == NULL)
    {
        printf("Error: No hay suficiente memoria.\n");
        return 1;
    }

    // Creacion archivo de salida
    FILE * salida = fopen("salida.txt", "w"); 


    // Inserta tareas en la lista de tareas segun prioridad
    for (int i = 0; i < ptr_size; i++) 
    {
        char nombre_tarea[20];
        int prioridad_tarea;
        float tiempo_tarea;

        fscanf(entrada, "%s", nombre_tarea);
        fscanf(entrada, "%i", &prioridad_tarea);
        fscanf(entrada, "%f", &tiempo_tarea);

        Tarea tarea_insrt = create_task(nombre_tarea, prioridad_tarea, tiempo_tarea);

        // Operacion Pointers
        start = clock();
        int posicion_ptrs = insertar(&nombres, &prioridades, &tiempos, &ptr_size, nombre_tarea, prioridad_tarea, tiempo_tarea);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        // Operacion Structs
        start = clock();
        int posicion_structs = insertar_structs(&lista_tareas, tarea_insrt, &struct_list_size);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        fprintf(salida, "INSERT %s PUNTEROS %f %i\n", nombre_tarea, actual_time_ptrs, posicion_ptrs);
        fprintf(salida, "INSERT %s STRUCTS %f %i\n", tarea_insrt.nombre, actual_time_structs, posicion_structs);

        // Validacion (Igualdad de tareas y orden)
        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, ptr_size));
    }


    // Se indica el numero de tareas a eliminar
    int tareas_eliminar = 0;
    fscanf(entrada, "%i", &tareas_eliminar);

    // Se eliminan las tareas
    for (int i = 0; i < tareas_eliminar; i++) 
    {
        char nombre_tarea[20];
        fscanf(entrada, "%s", nombre_tarea);

        // Operacion Pointers
        start = clock();
        int posicion_ptrs = eliminar(&nombres, &prioridades, &tiempos, &ptr_size, nombre_tarea);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        // Operacion Structs
        start = clock();
        int posicion_structs = eliminar_structs(&lista_tareas, &struct_list_size, nombre_tarea);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        // Checkeo posicion Pointers
        if (posicion_ptrs == -1)
        {
            fprintf(salida, "DELETE %s PUNTEROS: No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "DELETE %s PUNTEROS %f %i\n", nombre_tarea, actual_time_ptrs, posicion_ptrs);
        }

        // Checkeo posicion Structs
        if (posicion_structs == -1)
        {
            fprintf(salida, "DELETE %s STRUCTS: No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "DELETE %s STRUCTS %f %i\n", nombre_tarea, actual_time_structs, posicion_structs);
        }

        // Validacion (Igualdad de tareas y orden)
        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, ptr_size));
    }


    // Se indica el numero de tareas a buscar
    int tareas_buscar = 0;
    fscanf(entrada, "%i", &tareas_buscar);

    // Se buscan las tareas
    for (int i = 0; i < tareas_buscar; i++)
    {
        char nombre_tarea[20];
        fscanf(entrada, "%s", nombre_tarea);

        // Operacion Pointers
        start = clock();
        int posicion_ptrs = buscar(nombres, ptr_size, nombre_tarea);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        // Operacion Structs
        start = clock();
        int posicion_structs = buscar_structs(lista_tareas, struct_list_size, nombre_tarea);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        // Validacion posicion Pointers
        if (posicion_ptrs == -1)
        {
            fprintf(salida, "SEARCH %s PUNTEROS No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "SEARCH %s PUNTEROS %f %i\n", nombre_tarea, actual_time_ptrs, posicion_ptrs);
        }

        // Validacion posicion Structs
        if (posicion_structs == -1)
        {
            fprintf(salida, "SEARCH %s STRUCTS No se encontro la tarea.\n", nombre_tarea);
        } 
        else 
        {
            fprintf(salida, "SEARCH %s STRUCTS %f %i\n", nombre_tarea, actual_time_structs, posicion_structs);
        }

        // Validar (Igualdad tareas y orden)
        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, ptr_size));
    }


    fclose(entrada);

    // Variables para el tiempo final y de ejecucion
    float ptrs_exec_time = 0;
    float structs_exec_time = 0;
    int numero_ejecutadas = 0;

    // Se ejecutan las tareas
    while (ptr_size > 0)
    {
        numero_ejecutadas++;

        char nombre_ptrs[20];
        strcpy(nombre_ptrs, nombres[0]);

        char nombre_structs[20];
        strcpy(nombre_structs, lista_tareas[0].nombre);
        
        // Operacion Pointers
        start = clock();
        float tiempo_tarea_ptrs = ejecutar(&nombres, &prioridades, &tiempos, &ptr_size);
        end = clock();

        double actual_time_ptrs = get_time(start, end);
        tiempo_ptrs += actual_time_ptrs;

        // Operacion Structs
        start = clock();
        float tiempo_tarea_structs = ejecutar_structs(&lista_tareas, &struct_list_size);
        end = clock();

        double actual_time_structs = get_time(start, end);
        tiempo_structs += actual_time_structs;

        fprintf(salida, "EXECUTE %s PUNTEROS %f %i\n", nombre_ptrs, actual_time_ptrs, (int)tiempo_tarea_ptrs);
        ptrs_exec_time += tiempo_tarea_ptrs;

        fprintf(salida, "EXECUTE %s STRUCTS %f %i\n", nombre_ptrs, actual_time_ptrs, (int)tiempo_tarea_ptrs);
        structs_exec_time += tiempo_tarea_structs;

        // Validacion (Igualdada de tareas y orden)
        fprintf(salida, "%i\n", validar(lista_tareas, struct_list_size, nombres, prioridades, tiempos, ptr_size));
    }


    // Se imprime al archivo el numero de tareas ejecutadas y los tiempos
    fprintf(salida, "%i\n", numero_ejecutadas);
    fprintf(salida, "PUNTEROS %f %f\n", (tiempo_ptrs + ptrs_exec_time), tiempo_ptrs);
    fprintf(salida, "STRUCTS %f %f\n", (tiempo_structs + structs_exec_time), tiempo_structs);

    fclose(salida);

    // Se libera memoria y se valida si fue exitosa la liberacion
    int lib_ptrs = liberar(&nombres, &prioridades, &tiempos, &ptr_size);
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