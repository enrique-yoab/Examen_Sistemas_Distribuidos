#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdio.h>

// Estructuras complejas para la resolucion de las consultas, modificacion, insercion y eliminacion
// de los datos sobre las tablas
typedef struct
{
    int cantidad;
    char *rutas[13];
} ARCHIVERO;


typedef struct 
{
    int numero_tabla;            // indice de tablas estudiante = 0, direcciones[1] ...
    char *error;                 // Buffer de mensajes de error
    char *parametros;            // Máscara: "1" captura, "0" ignora
    char *llave;                 // Identificador único (llave primaria)
    int cantidad_resultados;     // Cuántos resultados se encontraron
    char **resultado;            // Array dinámico de strings con resultados
} CONSULTA;
    

// ESTRUCTURA FINAL PARA LA PETICION DEL USUARIO POR EL MOMENTO NO ES RELEVANTE
typedef struct 
{
    int operacion; // 1=Consulta, 2=Modificar, 3=Eliminar, 4=Actualizar
    int tabla;     // El índice de tu ARCHIVERO (0=Estudiante, 1=Direccion, etc.)
    char snum[4];  // La llave primaria para buscar
} PETICION;

typedef struct
{
    int num_lineas;
    int num_columnas;
} ANALISIS_ARCHIVO;

void imprimir_rutas(ARCHIVERO *dir);
void crear_directorio(ARCHIVERO *dir, char *carpeta, char *archivos[], char *tipo, int cantidad_archivos);


#endif