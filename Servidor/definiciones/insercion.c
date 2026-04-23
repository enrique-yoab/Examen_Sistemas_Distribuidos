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
        
        break;
    case 1:
        DIRECCION *direccion = verificar_direccion(cliente);
        if (cliente->error != NULL) break;
        break;
    case 2:
        CARRERA *carrera = verificar_carrera(cliente);
        if (cliente->error != NULL) break;
        break;
    case 3:
        HISTORIAL *historial = verificar_historial(cliente);
        if (cliente->error != NULL) break;
        break;
    case 4:
        INSCRIPCION *inscripcion = verificar_inscripcion(cliente);
        if (cliente->error != NULL) break;
        break;
    case 5:
        SECCION *seccion = verificar_seccion(cliente);
        if (cliente->error != NULL) break;
        break;
    case 6:
        PROFESOR *profesor = verificar_profesor(cliente);
        if (cliente->error != NULL) break;
        break;
    case 7:
        DEPARTAMENTO *departamento = verificar_departamento(cliente);
        if (cliente->error != NULL) break;
        break;
    case 8:
        NIVELES *niveles = verificar_niveles(cliente);
        if (cliente->error != NULL) break;
        break;
    case 9:
        HORARIO *horario = verificar_horario(cliente);
        if (cliente->error != NULL) break;
        break;
    case 10:
        GRADO *grado = verificar_grado(cliente);
        if (cliente->error != NULL) break;
        break;
    case 11:
        CURSO *curso = verificar_curso(cliente);
        if (cliente->error != NULL) break;
        break;
    case 12:
        AÑOS *años = verificar_años(cliente);
        if (cliente->error != NULL) break;
        break;
    case 13:
        // Semestre no tiene estructura específica, asumir que es similar a otros
        break;
    default:
        cliente->error = strdup("ERROR: Tabla no válida");
        break;
    }
}

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
        persona->semestre == NULL || 
        persona->domicilio_permanente == NULL || 
        persona->telefono_permanente == NULL) 
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
    if (cur->num_curso == NULL || 
        cur->semestre == NULL) 
    {
        cliente->error = strdup("ERROR: Uno o más campos de CURSO son NULL");
        return NULL;
    }else
    {
        return cur;
    }
}

AÑOS *verificar_años(INSERCION *cliente)
{
    AÑOS *años = (AÑOS *)cliente->estructura;
    if (años->ID_año == NULL || 
        años->Año == NULL) 
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
    if (niv->ID_nivel == NULL || 
        niv->nivel == NULL) 
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
    if (hor->ID_horario == NULL || 
        hor->horas == NULL) 
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