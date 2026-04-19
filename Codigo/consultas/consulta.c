#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "consulta.h"
#include "../estructuras/entidades.h"

void consulta_estudiante(char *ruta, char *parametros, char *salida[])
{
    if (strlen(parametros) < 8) {
        printf("[ERROR] Parametros invalidos. Deben ser 8 bits.\n");
        return;
    }

    FILE *tabla = fopen(ruta, "r");
    if (!tabla) {
        printf("[ERROR] No se pudo abrir %s\n", ruta);
        return;
    }

    char linea[256];
    int contador_salida = 0; // Para saber en qué índice de salida[] vamos

    // 1. Extraemos toda la línea correctamente
    while(fgets(linea, sizeof(linea), tabla) != NULL)
    {
        linea[strcspn(linea, "\n")] = 0; // Limpiar salto de línea

        char *snum       = strtok(linea, ",");
        char *dni        = strtok(NULL, ",");
        char *nombre     = strtok(NULL, ",");
        char *apellido_m = strtok(NULL, ",");
        char *apellido_p = strtok(NULL, ",");
        char *sexo       = strtok(NULL, ",");
        char *fecha_naci = strtok(NULL, ",");
        char *clase      = strtok(NULL, ",");

        // 2. Verificamos que la línea estaba completa
        if (snum && clase) 
        {
            char buffer_resultado[150] = ""; // Aquí armaremos lo que el usuario pidió

            // 3. Evaluamos qué campos concatenar según los parámetros
            if (parametros[0] == '1') { strcat(buffer_resultado, snum); strcat(buffer_resultado, ","); }
            if (parametros[1] == '1') { strcat(buffer_resultado, dni); strcat(buffer_resultado, ","); }
            if (parametros[2] == '1') { strcat(buffer_resultado, nombre); strcat(buffer_resultado, ","); }
            if (parametros[3] == '1') { strcat(buffer_resultado, apellido_m); strcat(buffer_resultado, ","); }
            if (parametros[4] == '1') { strcat(buffer_resultado, apellido_p); strcat(buffer_resultado, ","); }
            if (parametros[5] == '1') { strcat(buffer_resultado, sexo); strcat(buffer_resultado, ","); }
            if (parametros[6] == '1') { strcat(buffer_resultado, fecha_naci); strcat(buffer_resultado, ","); }
            if (parametros[7] == '1') { strcat(buffer_resultado, clase); strcat(buffer_resultado, ","); }

            // --- Extra: Eliminar la ultima coma sobrante ---
            int len = strlen(buffer_resultado);
            if (len > 0 && buffer_resultado[len - 1] == ',') {
                buffer_resultado[len - 1] = '\0'; // Reemplaza el espacio con el fin de cadena
            }

            // 4. Guardamos el resultado en el arreglo de salida
            salida[contador_salida] = malloc(strlen(buffer_resultado) + 1);
            strcpy(salida[contador_salida], buffer_resultado);
            
            contador_salida++;
        }
    }
    
    fclose(tabla);
}