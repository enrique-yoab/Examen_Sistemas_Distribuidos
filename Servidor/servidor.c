#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "./extensiones/entidades.h"
#include "./extensiones/consulta.h"
#include "./extensiones/insercion.h"
#include "./extensiones/update.h"

// Variables globales (se mantienen igual)
ARCHIVERO directorio;
char *escritura = "./buffer/";
char *carpeta = "./Datos/";
char *extension = ".csv";
char *archivos[] = {"Estudiante", "Direccion", "Carrera", "Historial", "Inscripcion", "Seccion", "Profesor", "Departamento", "Niveles", "Horario", "Grado", "Curso", "Años", "Semestre"};
int tamano = sizeof(archivos) / sizeof(archivos[0]);

// Prototipo corregido (debe retornar void* y recibir void*)
void* funcion_h1(void *arg);

int main(void){
    // Variables para el hilo
    pthread_t hilo1; HILO_DIR datos;
    // Llenamos la estructura con los datos
    datos.dir = &directorio;
    datos.carpeta = carpeta;
    datos.archivos = archivos;
    datos.extension = extension;
    datos.update = escritura;
    datos.tamano = tamano;

    // 1. Creamos el hilo pasándole la dirección de 'datos'
    pthread_create(&hilo1, NULL, funcion_h1, (void *)&datos);
    pthread_join(hilo1, NULL);

    CONSULTA cliente;
    cliente.numero_tabla = 5;             // 0 = Estudiante
    cliente.error = NULL;
    cliente.llave = "full";                // El snum que quieres buscar
    cliente.parametros = "110000000";     // Máscara: queremos columnas 0, 1 y 2 (Snum, DNI, Nombre)
    cliente.cantidad_resultados = 0;
    cliente.resultado = NULL;              // Se llenará en la función

    printf("\n--- Iniciando consulta de prueba ---\n");
    solicitud_consulta(&cliente, &directorio);

    if (cliente.error != NULL)
    {
        printf("%s", cliente.error);
    }else if (cliente.cantidad_resultados > 0) {
        
        printf("Se encontró %d registros:\n", cliente.cantidad_resultados);

        for (int i = 0; i < cliente.cantidad_resultados; i++) {
            printf("Resultado [%d]: %s\n", i, cliente.resultado[i]);
            free(cliente.resultado[i]); // Importante liberar la memoria de cada string
        }
        free(cliente.resultado); // Liberar el arreglo de punteros
    }

    // Se inserta un año nuevo 
    YEAR insert1;
    insert1.ID_año = "1555";
    insert1.Año = "2056";

    // Se simula la insercion del cliente
    INSERCION cliente2;
    cliente2.numero_tabla = 12; // Tabla Años
    cliente2.error = NULL;
    cliente2.estructura = (void*)&insert1;

    printf("\n--- Iniciando insercion de prueba ---\n");
    solicitud_insercion(&cliente2, &directorio);
    printf("%s\n", cliente2.error);


    cliente.numero_tabla = 12;             // 0 = Estudiante, 12 = Años
    cliente.error = NULL;
    cliente.llave = "full";                // El snum que quieres buscar
    cliente.parametros = "01";     // Máscara: queremos columnas 0, 1 y 2 (Snum, DNI, Nombre)
    cliente.cantidad_resultados = 0;
    cliente.resultado = NULL;  
    // Consultamos nuevamente
    printf("\n--- Iniciando consulta de prueba ---\n");
    solicitud_consulta(&cliente, &directorio);

    if (cliente.error != NULL)
    {
        printf("%s", cliente.error);
    }else if (cliente.cantidad_resultados > 0) {
        
        printf("Se encontró %d registros:\n", cliente.cantidad_resultados - 1);

        for (int i = 0; i < cliente.cantidad_resultados; i++) {
            printf("Resultado [%d]: %s\n", i, cliente.resultado[i]);
            free(cliente.resultado[i]); // Importante liberar la memoria de cada string
        }
        free(cliente.resultado); // Liberar el arreglo de punteros
    }

    printf("\n--- Iniciando actualizacion de prueba ---\n");
    // Realizamos una actualizacion
    UPDATE cliente3;
    cliente3.num_tabla = 12;
    cliente3.error = NULL;
    cliente3.primary_key = "1555";
    cliente3.parametros = "1555,2027";
    cliente3.estructura = NULL;

    solicitud_update(&cliente3, &directorio);

    if(cliente3.error == NULL)
    {
        printf("%s\n", cliente3.error);
        return 0;
    }

    cliente.numero_tabla = 12;             // 0 = Estudiante, 12 = Años
    cliente.error = NULL;
    cliente.llave = "full";                // El snum que quieres buscar
    cliente.parametros = "01";     // Máscara: queremos columnas 0, 1 y 2 (Snum, DNI, Nombre)
    cliente.cantidad_resultados = 0;
    cliente.resultado = NULL;  
    // Consultamos nuevamente
    printf("\n--- Iniciando consulta de prueba ---\n");
    solicitud_consulta(&cliente, &directorio);

    if (cliente.error != NULL)
    {
        printf("%s", cliente.error);
    }else if (cliente.cantidad_resultados > 0) {
        
        printf("Se encontró %d registros:\n", cliente.cantidad_resultados - 1);

        for (int i = 0; i < cliente.cantidad_resultados; i++) {
            printf("Resultado [%d]: %s\n", i, cliente.resultado[i]);
            free(cliente.resultado[i]); // Importante liberar la memoria de cada string
        }
        free(cliente.resultado); // Liberar el arreglo de punteros
    }

    return 0;
}

// Implementación de la función del hilo
void* funcion_h1(void *arg)
{
    // Convertimos el puntero genérico de vuelta a nuestro tipo de estructura
    HILO_DIR *args = (HILO_DIR *)arg;

    // Usamos los datos para llamar a la función original
    crear_directorio(args->dir, args->carpeta, args->archivos, args->extension, args->update, args->tamano);

    return NULL;
}