#ifndef INSERCION_H 
#define INSERCION_H
#include "entidades.h"

void solicitud_insercion(INSERCION *cliente, ARCHIVERO *dir);
char *insert_estudiante(ESTUDIANTE *persona, char *ruta);
char *insert_direccion(DIRECCION *dir, char *ruta);
char *insert_historial(HISTORIAL *historia, char *ruta);
char *insert_carrera(CARRERA *carrera, char *ruta);
char *insert_inscripcion(INSCRIPCION *insc, char *ruta);
char *insert_seccion(SECCION *secc, char *ruta);
char *insert_profesor(PROFESOR *profe, char *ruta);
char *insert_departamento(DEPARTAMENTO *depa, char *ruta);
char *insert_curso(CURSO *curso, char *ruta);
char *insert_year(YEAR *año, char *ruta);
char *insert_nivel(NIVELES *level, char *ruta);
char *insert_horario(HORARIO *hor, char *ruta);
char *insert_grado(GRADO *grado, char *ruta);
char *insert_semestre(SEMESTRE *sem, char *ruta);

#endif