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
    char *id_dir;
    char *snum;
    char *domicilio;
    char *ciudad;
    char *estado;
    char *codigo_pst;
} DIRECCION;

typedef struct 
{
    char *snum;
    char *matricula;
    char *ID_carrera;
    char *tipo;
} HISTORIAL;

typedef struct 
{
    char *id_folio;
    char *snum;
    char *ID_AUX;
} INSCRIPCION;

typedef struct
{
    char *ID_AUX;
    char *dpto;
    char *Num_curso;
    char *ID_seccion;
    char *Num_seccion;
    char *ID_año;
    char *ID_horas;
    char *ID_nivel;
    char *ID_profesor;
} SECCION;

typedef struct
{
    char *ID_profesor;
    char *profesor;
    char *Nombre_curso;
    char *Descripcion;
} PROFESOR;

typedef struct
{
    char *depto;
    char *nombre;
    char *telefono;
    char *facultad;
} DEPARTAMENTO;

typedef struct
{
    char *num_curso;
    char *semestre;
} CURSO;

typedef struct
{
    char *ID_año;
    char *Año;
} AÑOS;

typedef struct
{
    char *ID_nivel;
    char *nivel;
} NIVELES;

typedef struct
{
    char *ID_horario;
    char *horas;
} HORARIO;

typedef struct
{
    char *ID_semestre;
    char *nombre;
} SEMESTRE;

typedef struct
{
    char *ID_TMP;
    char *ID_grado;
    char *ID_AUX;
    char *Num_grado;
    char *Lettergrade;
} GRADO;

typedef struct
{
    int num_lineas;
    int num_columnas;
} ANALISIS_ARCHIVO;

#endif