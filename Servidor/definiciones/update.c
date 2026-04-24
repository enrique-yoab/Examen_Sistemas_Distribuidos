#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "../extensiones/entidades.h"
#include "../extensiones/update.h"

void solicitud_update(UPDATE *cliente, ARCHIVERO *dir)
{
    // 1. Validamos la existencia de la llave
    // Reciclamos la lógica: Si retorna "EXITO" es que la llave NO existe (Error para Update)
    char *estado = validar_llave(cliente->primary_key, cliente->num_tabla, dir->rutas[cliente->num_tabla]);
    if(strcmp(estado, "EXITO") == 0) 
    {
        cliente->error = strdup("ERROR: La llave primaria no existe en los registros.\n");
        return;
    }

    // 2. Si llegamos aquí, la llave existe y procedemos a la actualización física
    cliente->error = NULL;
    update_archivo(cliente, dir);
}

void update_archivo(UPDATE *cliente, ARCHIVERO *dir)
{
    char *ruta_original = dir->rutas[cliente->num_tabla];
    char *ruta_temporal = dir->rutas_update[cliente->num_tabla]; 

    FILE *archivo_lectura = fopen(ruta_original, "r");
    
    // [MOMENTO 1: NACIMIENTO] 
    // Aquí el sistema operativo CREA el archivo físicamente en la carpeta ./buffer/
    FILE *archivo_escritura = fopen(ruta_temporal, "w"); 

    if (!archivo_lectura || !archivo_escritura) {
        cliente->error = strdup("ERROR: Fallo critico al abrir/crear los archivos.\n");
        if(archivo_lectura) fclose(archivo_lectura);
        if(archivo_escritura) fclose(archivo_escritura);
        return;
    }

    // [MOMENTO 2: CRECIMIENTO]
    // Copiamos datos y metemos el registro actualizado
    char linea[1024];
    while (fgets(linea, sizeof(linea), archivo_lectura)) {
        char linea_copia[1024];
        strcpy(linea_copia, linea);
        char *llave_actual = strtok(linea_copia, ",");

        if (llave_actual != NULL && strcmp(llave_actual, cliente->primary_key) == 0) {
            fprintf(archivo_escritura, "%s\n", cliente->parametros);
        } else {
            fputs(linea, archivo_escritura);
        }
    }

    // Cerramos para liberar la memoria RAM
    fclose(archivo_lectura);
    fclose(archivo_escritura);

    // [MOMENTO 3: DESTRUCCIÓN DEL VIEJO]
    // Borramos el archivo original de la carpeta ./Datos/
    remove(ruta_original);

    // [MOMENTO 4: REENCARNACIÓN]
    // Movemos el archivo temporal de ./buffer/ hacia ./Datos/ y le cambiamos el nombre
    if (rename(ruta_temporal, ruta_original) == 0) {
        cliente->error = strdup("EXITO: Registro actualizado en el almacenamiento fisico.\n");
    } else {
        cliente->error = strdup("ERROR: No se pudo mover el archivo temporal.\n");
    }
}