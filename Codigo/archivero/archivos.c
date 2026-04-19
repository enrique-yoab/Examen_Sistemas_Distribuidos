#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "archivos.h"
#include "../estructuras/entidades.h"

// La ruta de la carpeta con respecto al archivo principal: ./tablas 

void crear_archivo(char *ruta_carpeta, char *nombre_tabla, char *ruta_salida)
{
    snprintf(ruta_salida, 50, "%s/%s.csv", ruta_carpeta, nombre_tabla);

    FILE *archivo = fopen(ruta_salida, "w"); 
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo en %s\n", ruta_salida);
        return;
    }
    printf("Tabla creada exitosamente en el disco.\n");
    fclose(archivo);
}

void eliminar_archivo(char *ruta)
{
    if (ruta == NULL) {
        printf("Error: La ruta proporcionada es nula.\n");
        return;
    }

    if (remove(ruta) == 0) {
        printf("El archivo '%s' fue eliminado correctamente.\n", ruta);
    } else {
        perror("Error al intentar eliminar el archivo");
    }
}

void eliminar_archivero(ARCHIVERO *diccionario)
{
    // 1. Liberamos las rutas individuales (las filas)
    // CORREGIDO: int i = 0
    for(int i = 0; i < diccionario->cantidad; i++) 
    {
        free(diccionario->rutas[i]);
    }

    // 2. Liberamos la columna vertebral (el arreglo de punteros)
    free(diccionario->rutas);

    // 3. Reiniciamos los valores de seguridad 
    // OJO: Nunca hacemos free(diccionario) porque el struct vive en el main
    diccionario->cantidad = 0;
    diccionario->rutas = NULL; 
    
    printf("Diccionario liberado con exito...\n");
}

void almacenar_archivo(char **almacen, int lugar, char *ruta)
{
    // 1. Asignamos dinámicamente las 50 "columnas" (caracteres) para esta fila
    almacen[lugar] = (char *)malloc(50 * sizeof(char));

    // Validación de seguridad por si el sistema operativo nos niega la memoria
    if (almacen[lugar] == NULL) {
        printf("Error fatal: No se pudo reservar memoria para la ruta.\n");
        return;
    }

    // 2. Copiamos el contenido de 'ruta' a nuestra nueva memoria.
    // Usamos 49 para dejar siempre el último byte libre.
    strncpy(almacen[lugar], ruta, 49);
    
    // 3. Forzamos el carácter nulo al final. Esto evita que el programa 
    // lea "basura" si la ruta original era más larga que nuestro límite.
    almacen[lugar][49] = '\0'; 
}