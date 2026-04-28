#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../extensiones/entidades.h"
#include "../extensiones/consulta.h"

extern int TAM_MAX;

void procesar_consulta(int socket_cliente, int tabla, char *llave, char *mascara, ARCHIVERO *dir) {
    if (llave == NULL || mascara == NULL) {
        char *msj = "ERROR: Faltan parametros en la consulta.\n";
        send(socket_cliente, msj, strlen(msj), 0);
        return;
    }

    CONSULTA c = {tabla, NULL, mascara, llave, 0, NULL};
    solicitud_consulta(&c, dir);

    // 1. Filtro de Errores vs Éxito
    if (c.error != NULL) {
        if (strncmp(c.error, "ERROR", 5) == 0) {
            send(socket_cliente, c.error, strlen(c.error), 0);
            free(c.error);
            return; // Terminamos aquí si hubo error
        }
        free(c.error); 
    } 
    
    // 2. Respuesta Vacía
    if (c.cantidad_resultados == 0) {
        char *msj_vacio = "No se encontraron registros coincidentes.\n";
        send(socket_cliente, msj_vacio, strlen(msj_vacio), 0);
        return;
    } 
    
    // 3. Protocolo Ping-Pong
    if (c.cantidad_resultados > 0) {
        char metadatos[50];
        char buffer_ack[TAM_MAX]; // Buffer local solo para los ACKs

        sprintf(metadatos, "CANTIDAD|%d", c.cantidad_resultados);
        send(socket_cliente, metadatos, strlen(metadatos), 0);

        bzero(buffer_ack, TAM_MAX);
        recv(socket_cliente, buffer_ack, TAM_MAX, 0); // Esperamos 1er ACK

        for (int i = 0; i < c.cantidad_resultados; i++) {
            send(socket_cliente, c.resultado[i], strlen(c.resultado[i]), 0);
            printf("%s\n", c.resultado[i]);
            
            bzero(buffer_ack, TAM_MAX);
            recv(socket_cliente, buffer_ack, TAM_MAX, 0); // Esperamos siguiente ACK
            
            free(c.resultado[i]); 
        }
        free(c.resultado); 
    }
}

void solicitud_consulta(CONSULTA *cliente, ARCHIVERO *dir)
{
    // 1. Validar que el número de tabla esté dentro de los límites
    if (cliente->numero_tabla < 0 || cliente->numero_tabla >= dir->cantidad) {
        cliente->error = strdup("ERROR: LA TABLA SOLICITADA NO EXISTE\n");
        return;
    }

    // 2. Descubrimiento de Esquema (Magia dinámica)
    char *ruta_archivo = dir->rutas[cliente->numero_tabla];
    ANALISIS_ARCHIVO analisis = analizar_archivo(ruta_archivo);
    int columnas_reales = analisis.num_columnas;

    printf("--------||| Se realiza consulta en la tabla %d |||--------\n", cliente->numero_tabla);

    // 3. Verificamos que la máscara de bits coincida con las columnas reales
    if (strlen(cliente->parametros) != columnas_reales) {
        char buffer_error[256];
        sprintf(buffer_error, "ERROR: La mascara ingresada no coincide. Esta tabla requiere exactamente %d parametros (Bits 1/0).\n", columnas_reales);
        cliente->error = strdup(buffer_error);
    } 
    // 4. Si todo está perfecto, lanzamos la consulta física
    else {
        consulta_tabla(cliente, dir, columnas_reales);
    }
}

void consulta_tabla(CONSULTA *cliente, ARCHIVERO *dir, int num_columnas)
{
    ANALISIS_ARCHIVO analisis = analizar_archivo(dir->rutas[cliente->numero_tabla]);

    FILE *tabla = fopen(dir->rutas[cliente->numero_tabla], "r");

    char linea[300];
    int hallados = 0;
    cliente->resultado = malloc(sizeof(char*) * analisis.num_lineas);
    while (fgets(linea, sizeof(linea), tabla) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';

        char *tokens[15];
        int col = 0;
        char *token = strtok(linea, ",");
        while (token != NULL && col < num_columnas) {
            tokens[col++] = token;
            token = strtok(NULL, ",");
        }

        if (col == 0) {
            continue;
        }

        int es_full = (cliente->llave != NULL && strcasecmp(cliente->llave, "full") == 0);
        int es_low = (cliente->llave != NULL && strcasecmp(cliente->llave, "low") == 0);
        int match = 0;

        if (es_full || es_low) {
            match = 1;
        } else if (cliente->llave == NULL) {
            match = 1;
        } else if (tokens[0] != NULL && strcmp(tokens[0], cliente->llave) == 0) {
            match = 1;
        }

        if (!match) {
            continue;
        }

        char buffer[1024] = "";
        int primero = 1;

        if (es_full) {
            for (int i = 0; i < col; i++) {
                if (!primero) strcat(buffer, ",");
                strcat(buffer, tokens[i]);
                primero = 0;
            }
        } else {
            for (int i = 0; i < num_columnas && i < col; i++) {
                if (cliente->parametros[i] == '1') {
                    if (!primero) strcat(buffer, ",");
                    strcat(buffer, tokens[i]);
                    primero = 0;
                }
            }
        }

        // printf("%s\n", buffer);
        cliente->resultado[hallados++] = strdup(buffer);
    }

    cliente->cantidad_resultados = hallados;
    // Liberamos la memoria no ocupada
    if (hallados > 0 && hallados < analisis.num_lineas) {
        cliente->resultado = realloc(cliente->resultado, sizeof(char*) * hallados);
    }
    if (hallados == 0) {
        free(cliente->resultado);
        cliente->resultado = NULL;
        cliente->error = strdup("ERROR: No se encontro ningun registro en la tabla consultada\n");
    }else{
        cliente->error = strdup("EXITO: Se encontro al menos 1 registro en la tabla\n");
    }

    fclose(tabla);
}