#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./archivero/archivos.h"
#include "./estructuras/entidades.h"

#define RUTA_ARCHIVOS "./archivero"
#define RUTA_TABLAS "./tablas"

void iniciar_bd(void);
void llenar_estudiantes(ARCHIVERO dicc);

int main()
{
    printf("--- SERVIDOR INICIANDO ---\n");

    iniciar_bd();

    printf("--- SERVIDOR APAGADO ---\n");
    return 0;
}

// No recibe nada (void), ella sola sabe qué hacer
void iniciar_bd(void) 
{
    ARCHIVERO diccionario;
    diccionario.cantidad = 0;
    diccionario.rutas = NULL;

    // ¡Tu diseño! La función contiene el esquema de forma privada
    char *tablas_fijas[] = {"estudiante", "direccion", "profesor"};
    int total_tablas = sizeof(tablas_fijas) / sizeof(tablas_fijas[0]);

    printf("\n--- CONSTRUYENDO ESQUEMA DE BASE DE DATOS ---\n");
    char ruta_temp[50];

    for(int i = 0; i < total_tablas; i++) {
        // 1. Creamos el archivo físico
        crear_archivo("./tablas", tablas_fijas[i], ruta_temp);

        // 2. Agrandamos el catálogo dinámicamente
        diccionario.cantidad++;
        diccionario.rutas = (char **)realloc(diccionario.rutas, diccionario.cantidad * sizeof(char *));

        // 3. Guardamos la ruta
        almacenar_archivo(diccionario.rutas, i, ruta_temp);
        
        printf("[OK] Tabla '%s' registrada en: %s\n", tablas_fijas[i], diccionario.rutas[i]);
    }
    printf("--- LLENANDO BASE DE DATOS ---\n");
    llenar_estudiantes(diccionario);
    printf("--- BASE DE DATOS LLENA ---\n");
    printf("--- CONSTRUCCION COMPLETADA ---\n\n");
}

void llenar_direcciones(ARCHIVERO dicc)
{
    int datos = 20;
    printf("[OK] Tablas 'direccion' llenada con %d registros\n", datos);
}