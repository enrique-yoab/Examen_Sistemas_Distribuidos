#ifndef CONSULTA_H 
#define CONSULTA_H
#include "entidades.h"

void solicitud_consulta(CONSULTA *cliente, ARCHIVERO *dir);
void consulta_tabla(CONSULTA *cliente, ARCHIVERO *dir, int num_columnas);
ANALISIS_ARCHIVO analizar_archivo(char *ruta);

#endif