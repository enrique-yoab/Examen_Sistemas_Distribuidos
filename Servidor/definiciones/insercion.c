#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../extensiones/entidades.h"
#include "../extensiones/insercion.h"

void solicitud_insercion(INSERCION *cliente, ARCHIVERO *dir)
{
    // 1. Verificamos que la tabla sea válida
    if (cliente->numero_tabla < 0 || cliente->numero_tabla > 13) {
        cliente->error = strdup("ERROR: Tabla no valida para insercion");
        return;
    }

    // 2. Validar campos vacíos rápidamente
    // Buscamos si hay dos comas juntas (,,) o si empieza/termina con coma
    if (strstr(cliente->datos, ",,") != NULL || 
        cliente->datos[0] == ',' || 
        cliente->datos[strlen(cliente->datos) - 1] == ',') 
    {
        cliente->error = strdup("ERROR: Los datos contienen campos vacios.");
        return;
    }

    // 3. Extraer la llave primaria de los datos para validarla
    char datos_copia[1024];
    strcpy(datos_copia, cliente->datos);
    char *llave_nueva = strtok(datos_copia, ",");

    if (llave_nueva == NULL) {
        cliente->error = strdup("ERROR: Formato de datos invalido.");
        return;
    }

    char *ruta_archivo = dir->rutas[cliente->numero_tabla];

    // 4. Validamos que la llave NO exista (Para INSERT, "EXITO" significa que la llave está libre)
    char *estado = validar_llave(llave_nueva, cliente->numero_tabla, ruta_archivo);
    
    if (strcmp(estado, "EXITO") != 0) {
        // Si no es EXITO, es porque validar_llave encontró duplicados o hubo un error
        cliente->error = strdup("ERROR: La llave primaria ya existe en la base de datos.");
        return;
    }

    // 5. Procedemos a insertar al final del archivo (Modo "a" - Append)
    FILE *archivo = fopen(ruta_archivo, "a");
    if (archivo == NULL) {
        cliente->error = strdup("ERROR: No se pudo abrir el archivo para insertar.");
        return;
    }

    // Escribimos la línea completa directamente
    fprintf(archivo, "%s\n", cliente->datos);
    fclose(archivo);

    cliente->error = strdup("EXITO: Registro insertado correctamente.");
}

void solicitud_eliminacion(ELIMINACION *cliente, ARCHIVERO *dir)
{
    // 1. Validamos que el registro SÍ EXISTA
    // Recordamos que si validar_llave retorna "EXITO", significa que la llave está libre (No existe)
    char *estado = validar_llave(cliente->primary_key, cliente->num_tabla, dir->rutas[cliente->num_tabla]);
    if(strcmp(estado, "EXITO") == 0) 
    {
        cliente->error = strdup("ERROR: La llave no existe. No se puede eliminar un registro fantasma.");
        return;
    }

    // 2. Rutas
    char *ruta_original = dir->rutas[cliente->num_tabla];
    char *ruta_temporal = dir->rutas_update[cliente->num_tabla]; // Usamos la misma carpeta ./buffer/

    FILE *archivo_lectura = fopen(ruta_original, "r");
    FILE *archivo_escritura = fopen(ruta_temporal, "w");

    if (!archivo_lectura || !archivo_escritura) {
        cliente->error = strdup("ERROR: Fallo critico al abrir los archivos para eliminacion.");
        if(archivo_lectura) fclose(archivo_lectura);
        if(archivo_escritura) fclose(archivo_escritura);
        return;
    }

    char linea[1024];

    // 3. Procesamiento de líneas
    while (fgets(linea, sizeof(linea), archivo_lectura)) {
        
        char linea_copia[1024];
        strcpy(linea_copia, linea);
        char *llave_actual = strtok(linea_copia, ",");

        // 4. Si encuentra la llave hace un salto de linea y la "elimina"
        if (llave_actual != NULL && strcmp(llave_actual, cliente->primary_key) == 0) {
            // ¡ENCONTRAMOS EL REGISTRO!
            // Para eliminarlo, simplemente usamos 'continue' para saltarnos la escritura
            // y pasar a la siguiente línea del while.
            continue; 
        } else {
            // Si no es la llave que buscamos, lo copiamos al archivo temporal
            fputs(linea, archivo_escritura);
        }
    }

    // 5. Limpieza y reemplazo
    fclose(archivo_lectura);
    fclose(archivo_escritura);

    remove(ruta_original);
    
    if (rename(ruta_temporal, ruta_original) == 0) {
        cliente->error = strdup("EXITO: Registro eliminado permanentemente.");
    } else {
        cliente->error = strdup("ERROR: No se pudo completar la eliminacion fisica.");
    }
}