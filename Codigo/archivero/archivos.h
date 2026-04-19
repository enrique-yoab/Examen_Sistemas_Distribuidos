#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>

// Esta estructura es el "Catálogo de Tablas"
typedef struct 
{
    int cantidad; // Cuántas tablas tenemos registradas
    char **rutas; // Arreglo dinámico de cadenas de texto (strings)
} ARCHIVERO;

// Prototipos
void crear_archivo(char *ruta_carpeta, char *nombre_tabla, char *ruta_salida);
void eliminar_archivo(char *ruta);
void eliminar_archivero(ARCHIVERO *diccionario);
void almacenar_archivo(char **almacen, int lugar, char *ruta);
void obtener_estudiantes(char *ruta, char atributos[], int *cantidad, char *salida[]);



#endif