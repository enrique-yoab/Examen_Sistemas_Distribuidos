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
    
    if(strcmp(estado, "EXITO") == 0) {
        cliente->error = "ERROR: La llave primaria no existe en los registros.\n";
        return;
    }

    // 2. Si llegamos aquí, la llave existe y procedemos a la actualización física
    cliente->error = NULL;
    update_archivo(cliente, dir);
}

void update_archivo(UPDATE *cliente, ARCHIVERO *dir)
{
    char *ruta_original = dir->rutas[cliente->num_tabla];
    char *ruta_temporal = dir->rutas_update[cliente->num_tabla]; // Usamos el campo pre-configurado

    FILE *archivo_lectura = fopen(ruta_original, "r");
    FILE *archivo_escritura = fopen(ruta_temporal, "w");

    if (archivo_lectura == NULL || archivo_escritura == NULL) {
        cliente->error = "ERROR: Fallo critico al abrir los archivos de base de datos.\n";
        if(archivo_lectura) fclose(archivo_lectura);
        if(archivo_escritura) fclose(archivo_escritura);
        return;
    }

    char linea[1024];
    while (fgets(linea, sizeof(linea), archivo_lectura)) {
        
        char linea_copia[1024];
        strcpy(linea_copia, linea);
        
        // Tokenizamos para obtener la llave de la línea actual
        char *llave_actual = strtok(linea_copia, ",");

        if (llave_actual != NULL && strcmp(llave_actual, cliente->primary_key) == 0) {
            // Reemplazamos la línea vieja por la nueva cadena de parámetros
            fprintf(archivo_escritura, "%s\n", cliente->parametros);
        } else {
            // Mantenemos la línea original
            fputs(linea, archivo_escritura);
        }
    }

    fclose(archivo_lectura);
    fclose(archivo_escritura);

    // // 3. Aplicamos los cambios al sistema de archivos
    // remove(ruta_original);
    // if (rename(ruta_temporal, ruta_original) == 0) {
    //     cliente->error = "EXITO: Registro actualizado en el almacenamiento fisico.\n";
    // } else {
    //     cliente->error = "ERROR: No se pudo renombrar el archivo temporal.\n";
    // }
}