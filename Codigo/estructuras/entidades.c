#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entidades.h"
#include "../archivero/archivos.h"
#include "../consultas/consulta.h"

void agregar_estudiante(ESTUDIANTE *persona, FILE *tabla) {
    // Escribimos en formato "CSV" con pipes
    fprintf(tabla, "%s,%d,%s,%s,%s,%c,%s,%s\n", 
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
    fprintf(tabla, "%s,%d,%s,%s,%s,%s,%s,%c\n", 
            dir->snum_dueno, 
            dir->id_dir,
            dir->calle,
            dir->ciudad,
            dir->estado,
            dir->codigo_postal,
            dir->telefono,
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

    FILE *table_student = fopen(dicc.rutas[0], "a");    
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

        agregar_estudiante(&estudiante, table_student);
    }
    fclose(table_student);    
    printf("[OK] Tablas 'estudiante' llenada con %d registros\n", datos);
}

void llenar_direcciones(ARCHIVERO dicc)
{
    int datos = 20;
    char *consul[datos];
    
    // 1. Obtenemos solo el SNUM (primer bit en 1) de todos los estudiantes
    consulta_estudiante(dicc.rutas[0], "10000000", consul);

    // Datos de los estudiantes para las direcciones
    char *actual[] = {"Boulevard Sur Niño 851 897", "Privada Sur Olivo 683 843", "Diagonal Sur García 485 832", "Pasaje Ochoa 286 Interior 088", "Andador Puebla 544 Edif. 676 - Depto. 319", "Periférico Valenzuela 027 Edif. 011 - Depto. 785", "Diagonal Segovia 385 932", "Pasaje Norte Báez 158 681", "Callejón Olmos 941 Edif. 256 - Depto. 634", "Avenida Santacruz 055 Interior 715", "Calzada Lozada 232 720", "Andador Querétaro 489 Interior 505", "Boulevard Vaca 238 Interior 252", "Ampliación Brito 828 Edif. 517 , Depto. 268", "Continuación Guillen 866 Interior 073", "Prolongación Sur Apodaca 426 Interior 503", "Circunvalación Distrito Federal 724 Edif. 338 - Depto. 251", "Calzada Sur Moya 041 217", "Retorno Hidalgo 828 Interior 858", "Andador Tayikistán 789 Interior 707"};
    char *ciudad[] = {"San Marco Antonio los bajos", "San Cristal los altos", "San Anel los altos", "Nueva Países Bajos", "Vieja Samoa", "Vieja Uganda", "San Rubén de la Montaña", "San José Eduardo de la Montaña", "San Rafaél los altos", "San Ana María de la Montaña", "Vieja Costa Rica", "San Verónica los bajos", "Nueva Kazajstán", "Vieja Iraq", "San Hermelinda de la Montaña", "San Miguel los bajos", "Vieja República Dominicana", "San Irene los bajos", "San Virginia de la Montaña", "Vieja India"};
    char *estado[] = {"Durango", "Querétaro", "Sonora", "Guerrero", "Baja California", "Guerrero", "Puebla", "Tabasco", "Yucatán", "Nayarit", "Colima", "Guanajuato", "Baja California Sur", "Jalisco", "Tlaxcala", "Distrito Federal", "Quintana Roo", "Querétaro", "Oaxaca", "Chihuahua"};
    char *cp[] = {"63265", "22487", "07110", "41139-2439", "83058-9696", "04922-6067", "54226", "33278-0324", "82869", "97199-7711", "04354", "86893-3995", "77245", "19683", "68371-4509", "71535-4856", "96509", "62817", "21284-0556", "87694"};
    char *permanente[] = {"Viaducto Arabia Saudita 216 Edif. 175 - Depto. 932", "Privada Solano 707 144", "Circunvalación Sonora 325 584", "Eje vial Correa 983 Interior 098", "Corredor Lovato 924 Interior 283", "Pasaje Castro 589 712", "Corredor Martínez 952 Interior 789", "Eje vial Veracruz de Ignacio de la Llave 694 Interior 889", "Continuación Sur Armenta 560 Interior 466", "Calle Fuentes 606 Interior 272", "Peatonal Marrero 022 Edif. 212 - Depto. 808", "Calzada Côte d'Ivoire 791 Interior 876", "Eje vial Barajas 894 Interior 386", "Avenida Sur del Río 747 Interior 900", "Avenida Sur de la Crúz 444 Interior 315", "Avenida Ortiz 654 Edif. 445 - Depto. 235", "Avenida Collado 957 Interior 947", "Callejón Sur Negrete 946 447", "Pasaje Jalisco 963 Edif. 080 - Depto. 291", "Andador Norte Quiñones 354 Edif. 447 - Depto. 114"};    
    char *telefono[] = {"(695)036-3854x80667", "1-850-823-4521x14858", "301-591-1557x61179", "+08(7)4691764241", "1-892-806-1324", "019.308.1976x12235", "258.458.5416", "1-504-167-2016x791", "+64(5)2120846250", "1-124-477-4018", "103.545.4598", "1-269-421-6148x9073", "1-687-708-4151x97961", "1-584-050-6899x498", "485-030-3120x63533", "964.942.4955x25282", "086-049-7686x8322", "458.172.4786", "1-875-807-1294x257", "1-298-047-1825"};
    
    printf("Iniciando llenado de direcciones...\n");

    // Abrimos el archivo UNA SOLA VEZ antes del ciclo
    FILE *table_dir = fopen(dicc.rutas[1], "a");
    if (!table_dir) {
        printf("[ERROR] No se pudo abrir el archivo de direcciones.\n");
        return;
    }

    int id_contador = 1; // Para que las llaves primarias vayan del 1 al 40

    for(int i = 0; i < datos; i++)
    {
        if (consul[i] != NULL) {
            
            // ============================================================
            // 1. REGISTRO DE DIRECCIÓN ACTUAL (Tipo 'A')
            // ============================================================
            DIRECCION dir;
            strcpy(dir.snum_dueno, consul[i]);
            dir.id_dir = id_contador++;
            dir.tipo = 'A';
            
            // Llenamos con los datos
            strcpy(dir.calle, actual[i]);
            strcpy(dir.ciudad, ciudad[i]);
            strcpy(dir.estado, estado[i]);
            strcpy(dir.codigo_postal, cp[i]);
            
            // Lo que no lleva (teléfono) lo llenamos con un espacio
            strcpy(dir.telefono, " "); 

            // Guardamos en el archivo
            agregar_dir(&dir, table_dir);

            // ============================================================
            // 2. REGISTRO DE DIRECCIÓN PERMANENTE (Tipo 'P')
            // ============================================================
            strcpy(dir.snum_dueno, consul[i]);
            dir.id_dir = id_contador++;
            dir.tipo = 'P';
            
            // Solo lleva calle (la permanente) y teléfono
            strcpy(dir.calle, permanente[i]);
            strcpy(dir.telefono, telefono[i]);
            
            // Lo que no lleva (ciudad, estado, cp) lo dejamos con espacios o 0
            strcpy(dir.ciudad, " ");
            strcpy(dir.estado, " ");
            strcpy(dir.codigo_postal, " ");
            

            // Guardamos en el archivo
            agregar_dir(&dir, table_dir);

            // Liberamos la memoria de la consulta
            free(consul[i]);
        }
    }

    // Cerramos el archivo al finalizar todos los registros
    fclose(table_dir);
    printf("[OK] Tabla 'direccion' llenada con 40 registros vinculados por Snum\n");
}