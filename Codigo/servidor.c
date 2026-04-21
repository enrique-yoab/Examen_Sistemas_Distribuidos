#include <stdio.h>
#include "./extensiones/consulta.h"
#include "./extensiones/entidades.h"

#define RUTA_TABLAS "./Datos/"

char *rutas[] = {"Estudiante", "Direccion", 
    "Carrera", "Historial", 
    "Inscripcion", "Seccion", 
    "Profesor", "Departamento", 
    "Niveles", "Horario", 
    "Grado", "Curso", "Años"};

int main(void){
    ESTUDIANTE persona;
    printf("Hola mundo\n");
    int tamano = sizeof(rutas) / sizeof(rutas[0]);
    printf("Existen %d tablas principales\n", tamano);
    consulta_estudiante(NULL, NULL);
    consulta_domicilio(NULL, NULL, NULL);
    return 0;
}