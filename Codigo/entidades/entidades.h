#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdio.h>

typedef struct
{
    char snum[4]; // LLave primaria de estudiante
    int DNI; // DNI de los estudiantes
    char nombre[15]; // Nombres
    char apellido_m[15]; // Apellido materno
    char apellido_p[15]; // Apellido paterno
    char fecha_na[11]; // Fecha nacimiento
    char sexo; // Genero de la persona
    char semestre[13]; // Semestre cursando
    char domicilio_permanente[50]; // Domicilio Permanente
    char telefono_permanente[21]; // Telefono Permanente
} ESTUDIANTE;

typedef struct
{
    int id_dir;
    char snum[4];
    char domicilio[50];
    char ciudad[30];
    char estado[30];
    char codigo_pst[15];
} DIRECCION;

typedef struct 
{
    char snum[4];
    char matricula[7];
    char ID_carrera[4];
    char tipo[10];
} HISTORIAL;

typedef struct 
{
    char id_folio[5];
    char snum[4];
    char ID_AUX[4];
} INSCRIPCION;

typedef struct
{
    char ID_AUX[4];
    char dpto[4];
    char Num_curso[8];
    char ID_seccion[7];
    char Num_seccion[2];
    char ID_año[5];
    char ID_horas[5];
    char ID_nivel[5];
    char ID_profesor[5];
} SECCION;

typedef struct
{
    char ID_profesor[5];
    char profesor[35];
    char Nombre_curso[35];
    char Descripcion[50];
} PROFESOR;

typedef struct
{
    char depto[4];
    char nombre[40];
    char telefono[30];
    char facultad[40];
} DEPARTAMENTO;

typedef struct
{
    char num_curso[8];
    char semestre[10];
} CURSO;

typedef struct
{
    char ID_año[5];
    char Año[5];
} AÑOS;

typedef struct
{
    char ID_nivel[5];
    char nivel[15];
} NIVELES;

typedef struct
{
    char ID_horario[5];
    char horas[3];
} HORARIO;

typedef struct
{
    char ID_TMP[5];
    char ID_grado[5];
    char ID_AUX[4];
    char Num_grado[4];
    char Lettergrade;
} GRADO;

#endif