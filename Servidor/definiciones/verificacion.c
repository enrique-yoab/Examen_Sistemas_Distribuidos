#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "../extensiones/entidades.h"

ESTUDIANTE *verificar_estudiante(INSERCION *cliente)
{
    ESTUDIANTE *persona = (ESTUDIANTE *)cliente->estructura;
    if (persona->snum == NULL || 
        persona->DNI == NULL || 
        persona->nombre == NULL || 
        persona->apellido_m == NULL || 
        persona->apellido_p == NULL || 
        persona->fecha_na == NULL || 
        persona->sexo == NULL || 
        persona->domicilio_permanente == NULL || 
        persona->telefono_permanente == NULL || 
        persona->semestre == NULL)
    {
        cliente->error = strdup("ERROR: Uno o más campos de ESTUDIANTE son NULL");
        return NULL;
    }else
    {
        return persona;
    }
}

DIRECCION *verificar_direccion(INSERCION *cliente)
{
    DIRECCION *dir = (DIRECCION *)cliente->estructura;
    if (dir->id_dir == NULL || 
        dir->snum == NULL || 
        dir->domicilio == NULL || 
        dir->ciudad == NULL || 
        dir->estado == NULL || 
        dir->codigo_pst == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de DIRECCION son NULL");
        return NULL;
    }else
    {
        return dir;
    }
}

HISTORIAL *verificar_historial(INSERCION *cliente)
{
    HISTORIAL *hist = (HISTORIAL *)cliente->estructura;
    if (hist->snum == NULL || 
        hist->matricula == NULL || 
        hist->ID_carrera == NULL || 
        hist->tipo == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de HISTORIAL son NULL");
        return NULL;
    }else
    {
        return hist;
    }
}

CARRERA *verificar_carrera(INSERCION *cliente)
{
    CARRERA *carr = (CARRERA *)cliente->estructura;
    if (carr->ID_carrera == NULL || 
        carr->Major == NULL || 
        carr->carrera == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de CARRERA son NULL");
        return NULL;
    }else
    {
        return carr;
    }
}

INSCRIPCION *verificar_inscripcion(INSERCION *cliente)
{
    INSCRIPCION *insc = (INSCRIPCION *)cliente->estructura;
    if (insc->id_folio == NULL || 
        insc->snum == NULL || 
        insc->ID_AUX == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de INSCRIPCION son NULL");
        return NULL;
    }else
    {
        return insc;
    }
}

SECCION *verificar_seccion(INSERCION *cliente)
{
    SECCION *sec = (SECCION *)cliente->estructura;
    if (sec->ID_AUX == NULL || 
        sec->dpto == NULL || 
        sec->Num_curso == NULL || 
        sec->ID_seccion == NULL || 
        sec->Num_seccion == NULL || 
        sec->ID_año == NULL || 
        sec->ID_horas == NULL || 
        sec->ID_nivel == NULL || 
        sec->ID_profesor == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de SECCION son NULL");
        return NULL;
    }else
    {
        return sec;
    }
}

PROFESOR *verificar_profesor(INSERCION *cliente)
{
    PROFESOR *prof = (PROFESOR *)cliente->estructura;
    if (prof->ID_profesor == NULL || 
        prof->profesor == NULL || 
        prof->Nombre_curso == NULL || 
        prof->Descripcion == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de PROFESOR son NULL");
        return NULL;
    }else
    {
        return prof;
    }
}

DEPARTAMENTO *verificar_departamento(INSERCION *cliente)
{
    DEPARTAMENTO *dept = (DEPARTAMENTO *)cliente->estructura;
    if (dept->depto == NULL || 
        dept->nombre == NULL || 
        dept->telefono == NULL || 
        dept->facultad == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de DEPARTAMENTO son NULL");
        return NULL;
    }else
    {
        return dept;
    }
}

CURSO *verificar_curso(INSERCION *cliente)
{
    CURSO *cur = (CURSO *)cliente->estructura;
    if (cur->num_curso == NULL || cur->semestre == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de CURSO son NULL");
        return NULL;
    }else
    {
        return cur;
    }
}

YEAR *verificar_year(INSERCION *cliente)
{
    YEAR *años = (YEAR *)cliente->estructura;
    if (años->ID_año == NULL || años->Año == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de AÑOS son NULL");
        return NULL;
    }else
    {
        return años;
    }
}

NIVELES *verificar_niveles(INSERCION *cliente)
{
    NIVELES *niv = (NIVELES *)cliente->estructura;
    if (niv->ID_nivel == NULL || niv->nivel == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de NIVELES son NULL");
        return NULL;
    }else
    {
        return niv;
    }
}

HORARIO *verificar_horario(INSERCION *cliente)
{
    HORARIO *hor = (HORARIO *)cliente->estructura;
    if (hor->ID_horario == NULL || hor->horas == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de HORARIO son NULL");
        return NULL;
    }else
    {
        return hor;
    }
}

GRADO *verificar_grado(INSERCION *cliente)
{
    GRADO *grad = (GRADO *)cliente->estructura;
    if (grad->ID_TMP == NULL || 
        grad->ID_grado == NULL || 
        grad->ID_AUX == NULL || 
        grad->Num_grado == NULL || 
        grad->Lettergrade == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de GRADO son NULL");
        return NULL;
    }else
    {
        return grad;
    }
}

SEMESTRE *verificar_semestre(INSERCION *cliente)
{
    SEMESTRE *sem = (SEMESTRE *)cliente->estructura;
    if(sem->ID_semestre == NULL || sem->nombre == NULL)
    {
        cliente->error = strdup("ERROR: Uno o más campos de SEMESTRE son NULL");
        return NULL;
    }else
    {
        return sem;
    }
}
