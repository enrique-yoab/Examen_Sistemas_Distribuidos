#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../extensiones/entidades.h"
#include "../extensiones/consulta.h"

void solicitud_consulta(CONSULTA *cliente, ARCHIVERO *dir)
{
    switch (cliente->numero_tabla)
    {
    case 0:
        printf("-----||| Se realiza consulta en la tabla Estudiantes |||-----\n");
        consulta_estudiantes(cliente, dir);
        break;
    case 1:
        printf("-----||| Se realiza consulta en la tabla Direccion |||-----\n");
        break;
    case 2:
        printf("-----||| Se realiza consulta en la tabla Carrera |||-----\n");
        break;
    case 3:
        printf("-----||| Se realiza consulta en la tabla Historial |||-----\n");
        break;
    case 4:
        printf("-----||| Se realiza consulta en la tabla Inscripcion |||-----\n");
        break;
    case 5:
        printf("-----||| Se realiza consulta en la tabla Seccion |||-----\n");
        break;
    case 6:
        printf("-----||| Se realiza consulta en la tabla Profesor |||-----\n");
        break;
    case 7:
        printf("-----||| Se realiza consulta en la tabla Departamento |||-----\n");
        break;
    case 8:
        printf("-----||| Se realiza consulta en la tabla Niveles |||-----\n");
        break;
    case 9:
        printf("-----||| Se realiza consulta en la tabla Horario |||-----\n");
        break;
    case 10:
        printf("-----||| Se realiza consulta en la tabla Grado |||-----\n");
        break;
    case 11:
        printf("-----||| Se realiza consulta en la tabla Curso |||-----\n");
        break;
    case 12:
        printf("-----||| Se realiza consulta en la tabla Años |||-----\n");
        break;
    case 13:
        printf("-----||| Se realiza consulta en la tabla Semestre |||-----\n");
        break;
    default:
        printf("ERROR: NO EXISTE ESA TABLA\n");
        break;
    }
}

void consulta_estudiantes(CONSULTA *cliente, ARCHIVERO *dir)
{
    // 1. Validaciones iniciales
    if (cliente->numero_tabla < 0 || cliente->numero_tabla >= dir->cantidad) {
        printf("[ERROR] Tabla fuera de rango.\n");
        return;
    }

    // Analizar el archivo para obtener número de líneas y columnas
    ANALISIS_ARCHIVO analisis = analizar_archivo(dir->rutas[cliente->numero_tabla]);

    FILE *tabla = fopen(dir->rutas[cliente->numero_tabla], "r");

    ESTUDIANTE alumno;
    char linea[512];
    int hallados = 0;
    // Reservar memoria dinámica basada en el número de líneas
    cliente->resultado = malloc(sizeof(char*) * analisis.num_lineas); 

    while(fgets(linea, sizeof(linea), tabla) != NULL)
    {
        linea[strcspn(linea, "\n")] = 0; 
        
        // 1. Mapeo manual con strtok
        alumno.snum   = strtok(linea, ",");
        alumno.DNI    = strtok(NULL, ",");
        alumno.nombre = strtok(NULL, ",");
        alumno.apellido_m   = strtok(NULL, ",");
        alumno.apellido_p   = strtok(NULL, ",");
        alumno.fecha_na  = strtok(NULL, ",");
        alumno.sexo   = strtok(NULL, ",");
        alumno.domicilio_permanente    = strtok(NULL, ",");
        alumno.telefono_permanente    = strtok(NULL, ",");
        alumno.id_semestre    = strtok(NULL, ",");

        char buffer[analisis.num_lineas * analisis.num_columnas * 4];
        // 2. Verificación de LLAVE (Si el cliente busca un snum específico)
        if (alumno.snum && strcmp(alumno.snum, cliente->llave) == 0) 
        {
            memset(buffer, 0, sizeof(buffer));
            // 3. Condicionales basados en la máscara de parámetros
            if (cliente->parametros[0] == '1') { strcat(buffer, alumno.snum);   strcat(buffer, ","); }
            if (cliente->parametros[1] == '1') { strcat(buffer, alumno.DNI);    strcat(buffer, ","); }
            if (cliente->parametros[2] == '1') { strcat(buffer, alumno.nombre); strcat(buffer, ","); }
            if (cliente->parametros[3] == '1') { strcat(buffer, alumno.apellido_m);   strcat(buffer, ","); }
            if (cliente->parametros[4] == '1') { strcat(buffer, alumno.apellido_p);   strcat(buffer, ","); }
            if (cliente->parametros[5] == '1') { strcat(buffer, alumno.fecha_na);  strcat(buffer, ","); }
            if (cliente->parametros[6] == '1') { strcat(buffer, alumno.sexo);   strcat(buffer, ","); }
            if (cliente->parametros[7] == '1') { strcat(buffer, alumno.domicilio_permanente);    strcat(buffer, ","); }
            if (cliente->parametros[8] == '1') { strcat(buffer, alumno.telefono_permanente);    strcat(buffer, ","); }
            if (cliente->parametros[9] == '1') { strcat(buffer, alumno.id_semestre);    strcat(buffer, ","); }

            // Quitar coma final y guardar
            int len = strlen(buffer);
            if (len > 0) buffer[len-1] = '\0';

            cliente->resultado[hallados] = strdup(buffer);
            hallados++;
        }
    }
    cliente->cantidad_resultados = hallados;
    fclose(tabla);
}