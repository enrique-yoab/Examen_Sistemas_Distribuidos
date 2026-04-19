#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entidades.h"
#include "../archivero/archivos.h"

void agregar_estudiante(ESTUDIANTE *persona, FILE *tabla) {
    // Escribimos en formato "CSV" con pipes
    fprintf(tabla, "%s|%d|%s|%s|%s|%c|%s|%s\n", 
            persona->snum, 
            persona->dni, 
            persona->nombre, 
            persona->apellido_m, 
            persona->apellido_p, 
            persona->sexo, 
            persona->fecha_nacimiento,
            persona->clase);
}

void agregar_dir(DIRECCION *dir, FILE *tabla)
{
    fprintf(tabla, "%s|%d|%s|%s|%s|%s|%d|%c\n", 
            dir->snum_dueno, 
            dir->id_dir,
            dir->telefono,
            dir->calle,
            dir->ciudad,
            dir->estado,
            dir->codigo_postal,
            dir->tipo
        );
}

void llenar_estudiantes(ARCHIVERO dicc)
{
    // Estudiantes
    int datos = 20;
    int contador = 1001;
    char *Snum[datos];
    for(int i = 0; i < datos; i ++) Snum[i] = (char *)malloc(datos * sizeof(char));

    for(int i = 0; i < datos; i ++)
    {
        sprintf(Snum[i], "S%d", contador);
        contador++;
    }

    char *names[] = {"Maria Luisa", "Carlota", "Nicolás", "Eric", "Maria", "Carla", "Angela", "Esteban", "Roberto", "Óscar", "Maria", "Aurelio", "Ramiro", "Aldonza", "Pedro", "Dolores", "Jorge", "Abraham", "René", "Elena"};
    char *apellido_m[] = {"Araceli", "Ariadna", "Mitzi", "Armando", "Fidel", "Octavio", "Espartaco", "Gabriela", "Ana", "Lourdes", "Patricio", "Rodrigo", "Alejandro", "José Emilio", "Beatriz", "Abraham", "Natividad", "Zacarías", "Ana María", "Alfredo"};
    char *apellido_p[] = {"Vega", "Nájera", "Toledo", "Vargas", "Pantoja", "Borrego", "Aguilera", "Salazar", "Valencia", "Estévez", "Olmos", "Arias", "Alcaraz", "Barragán", "Longoria", "Reyes", "Alemán", "Anguiano", "Medina", "Zedillo"};
    char sexo[] = {'M', 'F', 'F', 'F', 'F', 'M', 'F', 'F', 'F', 'F', 'M', 'M', 'F', 'F', 'F', 'F', 'M', 'F', 'F', 'F'};
    char *clase[] = {"5to semestre", "3er semestre","1er semestre", "7mo semestre", "5to semestre", "3er semestre", "5to semestre", "5to semestre", "1er semestre", "5to semestre", "1er semestre", "5to semestre", "1er semestre", "3er semestre", "3er semestre", "7mo semestre", "1er semestre", "3er semestre", "5to semestre", "7mo semestre"};
    char *fecha_naci[] = {"2005-10-06", "2003-10-07", "2005-05-20", "2003-12-17", "1996-11-07", "1997-01-21", "1998-09-05", "1995-01-12", "1999-03-13", "2005-12-05", "1995-01-18", "2001-06-08", "2002-10-27", "1999-10-05", "2004-07-05", "2006-12-04", "2001-02-01", "2007-05-09", "2003-07-08", "1995-06-03"};
    int dni[] = {521861420, 912416442, 777290831, 179253543, 82002031, 137616787, 296816353, 555203433, 133767936, 309623581, 131790292, 60845126, 319678542, 661794898, 559149988, 504169559, 828748077, 617296547, 942739743, 694465568};

    for(int i = 0; i < datos; i ++)
    {
        ESTUDIANTE estudiante;
        strcpy(estudiante.snum, Snum[i]);
        estudiante.dni = dni[i];
        strcpy(estudiante.nombre, names[i]);
        strcpy(estudiante.apellido_m, apellido_m[i]);
        strcpy(estudiante.apellido_p, apellido_p[i]);
        estudiante.sexo = sexo[i];
        strcpy(estudiante.fecha_nacimiento, fecha_naci[i]);
        strcpy(estudiante.clase, clase[i]);

        FILE *table_student = fopen(dicc.rutas[0], "a");    
        agregar_estudiante(&estudiante, table_student);
        fclose(table_student);    
    }
    printf("[OK] Tablas 'estudiante' llenada con %d registros\n", datos);
}