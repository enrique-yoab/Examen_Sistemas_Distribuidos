#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdio.h>

// Estructuras complejas para la resolucion de las consultas, modificacion, insercion y eliminacion
// de los datos sobre las tablas
typedef struct
{
    int cantidad;
    char *rutas[13];
} ARCHIVERO;


typedef struct 
{
    int numero_tabla;            // indice de tablas estudiante = 0, direcciones[1] ...
    int cantidad_columnas;       // Cantidad total de columnas en la tabla
    char *parametros;            // Máscara: "1" captura, "0" ignora
    char *llave;                 // Identificador único (llave primaria)
    int cantidad_resultados;     // Cuántos resultados se encontraron
    char **resultado;            // Array dinámico de strings con resultados
} CONSULTA;
    

// ESTRUCTURA FINAL PARA LA PETICION DEL USUARIO POR EL MOMENTO NO ES RELEVANTE
typedef struct 
{
    int operacion; // 1=Consulta, 2=Modificar, 3=Eliminar, 4=Actualizar
    int tabla;     // El índice de tu ARCHIVERO (0=Estudiante, 1=Direccion, etc.)
    char snum[4];  // La llave primaria para buscar
} PETICION;


void imprimir_rutas(ARCHIVERO *dir);
void crear_directorio(ARCHIVERO *dir, char *carpeta, char *archivos[], char *tipo, int cantidad_archivos);

// Entidades Principales de la base de datos
typedef struct
{
    char *snum; // LLave primaria de estudiante
    char *DNI; // DNI de los estudiantes
    char *nombre; // Nombres
    char *apellido_m; // Apellido materno
    char *apellido_p; // Apellido paterno
    char *fecha_na; // Fecha nacimiento
    char *sexo; // Genero de la persona
    char *domicilio_permanente; // Domicilio Permanente
    char *telefono_permanente; // Telefono Permanente
    char *id_semestre; // Semestre cursando
} ESTUDIANTE;

typedef struct
{
    char id_dir[5];
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
    char ID_semestre[5];
    char nombre[15];
} SEMESTRE;

typedef struct
{
    char ID_TMP[5];
    char ID_grado[5];
    char ID_AUX[4];
    char Num_grado[4];
    char Lettergrade;
} GRADO;

#endif