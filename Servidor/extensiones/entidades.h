#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdio.h>

// Estructuras complejas para la resolucion de las consultas, modificacion, insercion y eliminacion
// de los datos sobre las tablas
typedef struct
{
    int cantidad;
    char *rutas[14];
    char *rutas_update[14];
} ARCHIVERO;

// EStructura del hilo para ejecutar la funcion de crear directorio
typedef struct {
    ARCHIVERO *dir;
    char *carpeta;
    char *update;
    char **archivos;
    char *extension;
    int tamano;
} HILO_DIR;

typedef struct 
{
    int numero_tabla;            // indice de tablas estudiante = 0, direcciones[1] ...
    char *error;                 // Buffer de mensajes de error
    char *parametros;            // Máscara: "1" captura, "0" ignora
    char *llave;                 // Identificador único (llave primaria)
    int cantidad_resultados;     // Cuántos resultados se encontraron
    char **resultado;            // Array dinámico de strings con resultados
} CONSULTA;
    
typedef struct 
{
    int numero_tabla;
    char *error;
    char *datos;
} INSERCION;

typedef struct
{
    int num_tabla;
    char *error;
    char *primary_key;
    char *parametros;
    void *estructura;
} UPDATE;

typedef struct {
    int num_tabla;
    char *error;
    char *primary_key;
} ELIMINACION;

typedef struct
{
    int num_lineas;
    int num_columnas;
} ANALISIS_ARCHIVO;

// ESTRUCTURA FINAL PARA LA PETICION DEL USUARIO POR EL MOMENTO NO ES RELEVANTE
typedef struct 
{
    int operacion; // 1=Consulta, 2=Modificar, 3=Eliminar, 4=Actualizar
    int tabla;     // El índice de tu ARCHIVERO (0=Estudiante, 1=Direccion, etc.)
    char snum[4];  // La llave primaria para buscar
} PETICION;

void imprimir_rutas(ARCHIVERO *dir);
void crear_directorio(ARCHIVERO *dir, char *carpeta, char *archivos[], char *tipo, char *update, int cantidad_archivos);
int levantar_servicio(int puerto);
char *validar_llave(char *llave, int num_table, char *ruta);
ANALISIS_ARCHIVO analizar_archivo(char *ruta);


#endif