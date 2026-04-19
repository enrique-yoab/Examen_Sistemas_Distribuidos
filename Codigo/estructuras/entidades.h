#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdio.h>
#include "../archivero/archivos.h"

typedef struct 
{
    char snum[10];       // Para guardar "S1001"
    int dni;
    char nombre[50];     // Tamaño fijo para facilitar el guardado en archivos
    char apellido_m[50];
    char apellido_p[50];
    char sexo;           // 'M' o 'F'
    char fecha_nacimiento[11]; // "AAAA-MM-DD"
    char clase[13]; // "1er, 2do, 3ro, 4to, 5to, 6to, 7mo, 8vo, 9no, 10mo" + "semestre"
} ESTUDIANTE;

typedef struct
{
    char snum_dueno[10]; // ¡IMPORTANTE! La llave foránea que conecta con Estudiante
    int id_dir;
    char calle[100];
    char ciudad[50];
    char estado[50];
    char codigo_postal[11];
    char tipo;           // 'A' Actual, 'P' Permanente
    char telefono[15];   // Los teléfonos como string por los guiones/paréntesis
} DIRECCION;

// 2. Después definimos los prototipos que usan esos tipos
void agregar_estudiante(ESTUDIANTE *persona, FILE *tabla);
void eliminar_estudiante(char *snum, FILE *tabla);
void agregar_dir(DIRECCION *dir, FILE *tabla);
void eliminar_dir(char *snum, FILE *tabla);
void llenar_estudiantes(ARCHIVERO dicc);
void llenar_direcciones(ARCHIVERO dicc);

#endif