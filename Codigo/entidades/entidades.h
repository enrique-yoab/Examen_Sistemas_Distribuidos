#ifdef ENTIDADES_H
#define ENTIDADES_H
#includes <stdio.h>

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

#endif