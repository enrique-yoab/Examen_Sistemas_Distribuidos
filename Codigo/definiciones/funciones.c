#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../extensiones/entidades.h"
#include "../extensiones/consulta.h"

void crear_directorio(ARCHIVERO *dir, char *carpeta, char *archivos[], char *tipo, int cantidad_archivos)
{   // Obtenemos la cantidad de archivos a crear
    printf("Se esta creando un diccionario con %d rutas de archivos %s\n", cantidad_archivos, tipo);
    for(int i = 0; i < cantidad_archivos; i ++)
    {
        size_t len = strlen(carpeta) + strlen(archivos[i]) + strlen(tipo) + 1;
        char *ruta_completa = (char *)malloc(len);
        strcpy(ruta_completa, carpeta);
        strcat(ruta_completa, archivos[i]);
        strcat(ruta_completa, tipo);
        dir->rutas[i] = ruta_completa;
        dir->cantidad = i + 1;
    }
}

ANALISIS_ARCHIVO analizar_archivo(char *ruta)
{
    ANALISIS_ARCHIVO analisis = {0, 0};
    FILE *archivo = fopen(ruta, "r");

    char linea[512];
    int primera_linea = 1;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        analisis.num_lineas++;
        if (primera_linea) {
            // Contar columnas: número de comas + 1
            char *ptr = linea;
            while (*ptr) {
                if (*ptr == ',') {
                    analisis.num_columnas++;
                }
                ptr++;
            }
            analisis.num_columnas++; // Contar ultimo renglon por sintaxis .csv
            primera_linea = 0;
        }
    }
    // Cerramos el archivo y regresamos el analisis
    fclose(archivo);
    return analisis;
}

void imprimir_rutas(ARCHIVERO *dir)
{
    int i = 0;
    printf("---------> Mostrando rutas <---------\n");
    while (i < dir->cantidad)
    {
        printf("Ruta[%d] : %s \n", i + 1, dir->rutas[i]);
        i++;
    }
    printf("-------------------------------------\n");
}
