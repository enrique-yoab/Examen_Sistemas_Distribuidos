#ifndef INSERCION_H 
#define INSERCION_H
#include "entidades.h"

void solicitud_insercion(INSERCION *cliente, ARCHIVERO *dir);
ESTUDIANTE *verificar_estudiante(INSERCION *cliente);
DIRECCION *verificar_direccion(INSERCION *cliente);
HISTORIAL *verificar_historial(INSERCION *cliente);
CARRERA *verificar_carrera(INSERCION *cliente);
INSCRIPCION *verificar_inscripcion(INSERCION *cliente);
SECCION *verificar_seccion(INSERCION *cliente);
PROFESOR *verificar_profesor(INSERCION *cliente);
DEPARTAMENTO *verificar_departamento(INSERCION *cliente);
CURSO *verificar_curso(INSERCION *cliente);
AÑOS *verificar_años(INSERCION *cliente);
NIVELES *verificar_niveles(INSERCION *cliente);
HORARIO *verificar_horario(INSERCION *cliente);
GRADO *verificar_grado(INSERCION *cliente);

#endif