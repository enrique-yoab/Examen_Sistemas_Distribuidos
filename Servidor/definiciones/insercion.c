#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "../extensiones/entidades.h"
#include "../extensiones/insercion.h"

void solicitud_insercion(INSERCION *cliente, ARCHIVERO *dir)
{
    switch (cliente->numero_tabla)
    {
    case 0:
        // Convertimos la estructura vacia a estructura de estudiante 
        // y revisamos que no tenga ningun dato vacio
        ESTUDIANTE *persona = verificar_estudiante(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_estudiante(persona, dir->rutas[cliente->numero_tabla]));
        break;
    case 1:
        DIRECCION *direccion = verificar_direccion(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_direccion(direccion, dir->rutas[cliente->numero_tabla]));
        break;
    case 2:
        CARRERA *carrera = verificar_carrera(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_carrera(carrera, dir->rutas[cliente->numero_tabla]));
        break;
    case 3:
        HISTORIAL *historial = verificar_historial(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_historial(historial, dir->rutas[cliente->numero_tabla]));
        break;
    case 4:
        INSCRIPCION *inscripcion = verificar_inscripcion(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_inscripcion(inscripcion, dir->rutas[cliente->numero_tabla]));
        break;
    case 5:
        SECCION *seccion = verificar_seccion(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_seccion(seccion, dir->rutas[cliente->numero_tabla]));
        break;
    case 6:
        PROFESOR *profesor = verificar_profesor(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_profesor(profesor, dir->rutas[cliente->numero_tabla]));
        break;
    case 7:
        DEPARTAMENTO *departamento = verificar_departamento(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_departamento(departamento, dir->rutas[cliente->numero_tabla]));
        break;
    case 8:
        NIVELES *niveles = verificar_niveles(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_nivel(niveles, dir->rutas[cliente->numero_tabla]));
        break;
    case 9:
        HORARIO *horario = verificar_horario(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_horario(horario, dir->rutas[cliente->numero_tabla]));
        break;
    case 10:
        GRADO *grado = verificar_grado(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_grado(grado, dir->rutas[cliente->numero_tabla]));
        break;
    case 11:
        CURSO *curso = verificar_curso(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_curso(curso, dir->rutas[cliente->numero_tabla]));
        break;
    case 12:
        YEAR *años = verificar_year(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_year(años, dir->rutas[cliente->numero_tabla]));
        break;
    case 13:
        SEMESTRE *sem = verificar_semestre(cliente);
        if (cliente->error != NULL) break;
        cliente->error = strdup(insert_semestre(sem, dir->rutas[cliente->numero_tabla]));
        break;
    default:
        cliente->error = strdup("ERROR: Tabla no válida");
        break;
    }
}

char *insert_estudiante(ESTUDIANTE *persona, char *ruta)
{
    FILE *archivo = fopen(ruta, "a"); 
    fprintf(archivo, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
        persona->snum,
        persona->DNI,
        persona->nombre,
        persona->apellido_m,
        persona->apellido_p,
        persona->fecha_na,
        persona->sexo,
        persona->domicilio_permanente,
        persona->telefono_permanente,
        persona->semestre
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente el estudiante";
}

char *insert_direccion(DIRECCION *dir, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s,%s,%s,%s\n",
        dir->id_dir,
        dir->snum,
        dir->domicilio,
        dir->ciudad,
        dir->estado,
        dir->codigo_pst
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente la direccion";
}

char *insert_historial(HISTORIAL *historia, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s,%s\n",
        historia->snum,
        historia->matricula,
        historia->ID_carrera,
        historia->tipo
    );
    fclose(archivo);
    return "EXITO: se inserto correctamente el historial";
}

char *insert_carrera(CARRERA *carrera, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s\n",
        carrera->ID_carrera,
        carrera->Major,
        carrera->carrera
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente la carrera";
}

char *insert_inscripcion(INSCRIPCION *insc, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s\n",
        insc->id_folio,
        insc->snum,
        insc->ID_AUX
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente la inscripcion";
}

char *insert_seccion(SECCION *secc, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
        secc->ID_AUX,
        secc->dpto,
        secc->Num_curso,
        secc->ID_seccion,
        secc->Num_seccion,
        secc->ID_año,
        secc->ID_horas,
        secc->ID_nivel,
        secc->ID_profesor
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente la seccion";
}

char *insert_profesor(PROFESOR *profe, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s,%s\n",
        profe->ID_profesor,
        profe->profesor,
        profe->Nombre_curso,
        profe->Descripcion
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente al profesor";    
}

char *insert_departamento(DEPARTAMENTO *depa, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s,%s,%s\n",
        depa->depto,
        depa->nombre,
        depa->oficina,
        depa->telefono,
        depa->facultad
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente el departamento";    
}

char *insert_curso(CURSO *curso, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s\n",
        curso->num_curso,
        curso->semestre
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente al curso";  
}

char *insert_year(YEAR *año, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s\n",
        año->ID_año,
        año->Año
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente al Año";  
}

char *insert_nivel(NIVELES *level, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s\n",
        level->ID_nivel,
        level->nivel
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente el nivel"; 
}

char *insert_horario(HORARIO *hor, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s\n",
        hor->ID_horario,
        hor->horas
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente el horario"; 
}

char *insert_grado(GRADO *grado, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s,%s,%s,%s\n",
        grado->ID_TMP,
        grado->ID_AUX,
        grado->ID_grado,
        grado->Num_grado,
        grado->Lettergrade
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente el grado"; 
}

char *insert_semestre(SEMESTRE *sem, char *ruta)
{
    FILE *archivo = fopen(ruta, "a");
    fprintf(archivo, "%s,%s\n",
        sem->ID_semestre,
        sem->nombre
    );
    fclose(archivo);
    return "EXITO: Se inserto correctamente el semestre"; 
}
