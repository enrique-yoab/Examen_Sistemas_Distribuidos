#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "../extensiones/entidades.h"
#include "../extensiones/consulta.h"

void solicitud_consulta(CONSULTA *cliente, ARCHIVERO *dir)
{
    switch (cliente->numero_tabla)
    {
    case 0:
        printf("--------||| Se realiza consulta en la tabla Estudiantes |||--------\n");
        if(strlen(cliente->parametros) != 10) cliente->error = strdup("ERROR: TABLA ESTUDIANTES OCUPA 10 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 10);
        break;
    case 1:
        printf("--------||| Se realiza consulta en la tabla Direccion |||--------\n");
        if(strlen(cliente->parametros) != 6) cliente->error = strdup("ERROR: TABLA DIRECCION OCUPA 6 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 6);
        break;
    case 2:
        printf("-----||| Se realiza consulta en la tabla Carrera |||-----\n");
        if(strlen(cliente->parametros) != 3) cliente->error = strdup("ERROR: TABLA CARRERA OCUPA 3 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 3);
        break;
    case 3:
        printf("-----||| Se realiza consulta en la tabla Historial |||-----\n");
        if(strlen(cliente->parametros) != 4) cliente->error = strdup("ERROR: TABLA HISTORIAL OCUPA 4 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 4);
        break;
    case 4:
        printf("-----||| Se realiza consulta en la tabla Inscripcion |||-----\n");
        if(strlen(cliente->parametros) != 3) cliente->error = strdup("ERROR: TABLA INSCRIPCION OCUPA 3 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 3);
        break;
    case 5:
        printf("-----||| Se realiza consulta en la tabla Seccion |||-----\n");
        if(strlen(cliente->parametros) != 9) cliente->error = strdup("ERROR: TABLA SECCION OCUPA 9 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 9);
        break;
    case 6:
        printf("-----||| Se realiza consulta en la tabla Profesor |||-----\n");
        if(strlen(cliente->parametros) != 4) cliente->error = strdup("ERROR: TABLA PROFESORES OCUPA 4 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 4);
        break;
    case 7:
        printf("-----||| Se realiza consulta en la tabla Departamento |||-----\n");
        if(strlen(cliente->parametros) != 5) cliente->error = strdup("ERROR: TABLA DEPARTAMENTO OCUPA 5 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 5);
        break;
    case 8:
        printf("-----||| Se realiza consulta en la tabla Niveles |||-----\n");
        if(strlen(cliente->parametros) != 2) cliente->error = strdup("ERROR: TABLA NIVELES OCUPA 2 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 2);
        break;
    case 9:
        printf("-----||| Se realiza consulta en la tabla Horario |||-----\n");
        if(strlen(cliente->parametros) != 2) cliente->error = strdup("ERROR: TABLA HORARIO OCUPA 2 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 2);
        break;
    case 10:
        printf("-----||| Se realiza consulta en la tabla Grado |||-----\n");
        if(strlen(cliente->parametros) != 5) cliente->error = strdup("ERROR: TABLA GRADO OCUPA 5 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 5);
        break;
    case 11:
        printf("-----||| Se realiza consulta en la tabla Curso |||-----\n");
        if(strlen(cliente->parametros) != 2) cliente->error = strdup("ERROR: TABLA CURSO OCUPA 2 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 2);
        break;
    case 12:
        printf("-----||| Se realiza consulta en la tabla Años |||-----\n");
        if(strlen(cliente->parametros) != 2) cliente->error = strdup("ERROR: TABLA AÑOS OCUPA 2 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 2);
        break;
    case 13:
        printf("-----||| Se realiza consulta en la tabla Semestre |||-----\n");
        if(strlen(cliente->parametros) != 2) cliente->error = strdup("ERROR: TABLA SEMESTRE OCUPA 2 PARAMETROS\n");
        else consulta_tabla(cliente, dir, 2);
        break;
    default:
        printf("ERROR: NO EXISTE ESA TABLA\n");
        break;
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