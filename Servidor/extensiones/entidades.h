#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdio.h>

// Estructuras complejas para la resolucion de las consultas, modificacion, insercion y eliminacion
// de los datos sobre las tablas
typedef struct
{
    int cantidad;
    char *rutas[14];
    char *rutas_update[14];
} ARCHIVERO;

// EStructura del hilo para ejecutar la funcion de crear directorio
typedef struct {
    ARCHIVERO *dir;
    char *carpeta;
    char *update;
    char **archivos;
    char *extension;
    int tamano;
} HILO_DIR;

typedef struct 
{
    int numero_tabla;            // indice de tablas estudiante = 0, direcciones[1] ...
    char *error;                 // Buffer de mensajes de error
    char *parametros;            // Máscara: "1" captura, "0" ignora
    char *llave;                 // Identificador único (llave primaria)
    int cantidad_resultados;     // Cuántos resultados se encontraron
    char **resultado;            // Array dinámico de strings con resultados
} CONSULTA;
    
typedef struct 
{
    int numero_tabla;
    char *error;
    void *estructura;
} INSERCION;

typedef struct
{
    int num_tabla;
    char *error;
    char *primary_key;
    char *parametros;
    void *estructura;
} UPDATE;

typedef struct
{
    int num_lineas;
    int num_columnas;
} ANALISIS_ARCHIVO;

// ESTRUCTURA FINAL PARA LA PETICION DEL USUARIO POR EL MOMENTO NO ES RELEVANTE
typedef struct 
{
    int operacion; // 1=Consulta, 2=Modificar, 3=Eliminar, 4=Actualizar
    int tabla;     // El índice de tu ARCHIVERO (0=Estudiante, 1=Direccion, etc.)
    char snum[4];  // La llave primaria para buscar
} PETICION;

// ENTIDADES PRINCIPALES PARA LAS INSERCIONES EN LA TABLA
typedef struct
{
    char *snum; // LLave primaria de estudiante
    char *DNI; // DNI de los estudiantes
    char *nombre; // Nombres
    char *apellido_m; // Apellido materno
    char *apellido_p; // Apellido paterno
    char *fecha_na; // Fecha nacimiento
    char *sexo; // Genero de la persona
    char *semestre; // Semestre cursando
    char *domicilio_permanente; // Domicilio Permanente
    char *telefono_permanente; // Telefono Permanente
} ESTUDIANTE;

typedef struct
{
    char *id_dir; //Llave primaria de direccion
    char *snum; //Llave foranea de estudiante
    char *domicilio;// Domicilio actual del estudiante
    char *ciudad; // Ciudad del domicilio actual
    char *estado; // Estado del domicilio actual
    char *codigo_pst; // Codigo postal del domicilio actual
} DIRECCION;

typedef struct 
{
    char *snum; // Llave foraea del estudiante
    char *matricula; // Matricula de carrera del estudiante
    char *ID_carrera; // llave foranea de la carrera 
    char *tipo; // primera, segunda, tercera, cuarta, etc.
} HISTORIAL;

typedef struct
{
    char *ID_carrera;
    char *Major;
    char *carrera;
} CARRERA;

typedef struct 
{
    char *id_folio; //
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
    char *oficina;
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
} YEAR;

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
    char *ID_TMP;
    char *ID_grado;
    char *ID_AUX;
    char *Num_grado;
    char *Lettergrade;
} GRADO;

typedef struct
{
    char *ID_semestre;
    char *nombre;
} SEMESTRE;

void imprimir_rutas(ARCHIVERO *dir);
void crear_directorio(ARCHIVERO *dir, char *carpeta, char *archivos[], char *tipo, char *update, int cantidad_archivos);
int levantar_servicio(int puerto);
char *validar_llave(char *llave, int num_table, char *ruta);
ANALISIS_ARCHIVO analizar_archivo(char *ruta);
ESTUDIANTE *verificar_estudiante(INSERCION *cliente);
DIRECCION *verificar_direccion(INSERCION *cliente);
HISTORIAL *verificar_historial(INSERCION *cliente);
CARRERA *verificar_carrera(INSERCION *cliente);
INSCRIPCION *verificar_inscripcion(INSERCION *cliente);
SECCION *verificar_seccion(INSERCION *cliente);
PROFESOR *verificar_profesor(INSERCION *cliente);
DEPARTAMENTO *verificar_departamento(INSERCION *cliente);
CURSO *verificar_curso(INSERCION *cliente);
YEAR *verificar_year(INSERCION *cliente);
NIVELES *verificar_niveles(INSERCION *cliente);
HORARIO *verificar_horario(INSERCION *cliente);
GRADO *verificar_grado(INSERCION *cliente);
SEMESTRE *verificar_semestre(INSERCION *cliente);


#endif