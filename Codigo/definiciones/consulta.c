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
    ESTUDIANTE alumno;
    FILE *tabla = fopen(dir->rutas[cliente->numero_tabla], "r");

    char linea[512];
    int hallados = 0;
    // Reservamos espacio inicial para los resultados (ej. máximo 100 coincidencias o dinámico)
    cliente->resultado = malloc(sizeof(char*) * 100); 

    while(fgets(linea, sizeof(linea), tabla) != NULL)
    {
        linea[strcspn(linea, "\n")] = 0; 
        alumno.snum = strtok(linea, ",");
        alumno.nombre = strtok(NULL, ",");
        alumno.nombre = strtok(NULL, ",");    
        alumno.apellido_m = strtok(NULL, ",");    
        alumno.apellido_p = strtok(NULL, ",");    
        alumno.fecha_na = strtok(NULL, ",");    
        alumno.sexo = strtok(NULL, ",");        
        alumno.domicilio_permanente = strtok(NULL, ",");    
        alumno.telefono_permanente = strtok(NULL, ",");    
        alumno.id_semestre = strtok(NULL, ",");  
    }
    cliente->cantidad_resultados = hallados;
    fclose(tabla);
}