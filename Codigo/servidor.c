#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "./extensiones/consulta.h"
#include "./extensiones/entidades.h"

typedef struct {
    ARCHIVERO *dir;
    char *carpeta;
    char **archivos;
    char *extension;
    int tamano;
} HILO_DIR;

// Variables globales (se mantienen igual)
ARCHIVERO directorio;
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
    datos.tamano = tamano;

    // 1. Creamos el hilo pasándole la dirección de 'datos'
    pthread_create(&hilo1, NULL, funcion_h1, (void *)&datos);
    pthread_join(hilo1, NULL);

    CONSULTA cliente;
    cliente.numero_tabla = 0;             // 0 = Estudiante
    cliente.cantidad_columnas = 10;        // Según tu estructura ESTUDIANTE
    cliente.llave = "S1016";                // El snum que quieres buscar
    cliente.parametros = "1110000001";     // Máscara: queremos columnas 0, 1 y 2 (Snum, DNI, Nombre)
    cliente.cantidad_resultados = 0;
    cliente.resultado = NULL;              // Se llenará en la función

    printf("\n--- Iniciando consulta de prueba ---\n");
    solicitud_consulta(&cliente, &directorio);

    // // 2. Revisión de resultados
    // if (cliente.cantidad_resultados > 0) {
    //     printf("Se encontraron %d registros:\n", cliente.cantidad_resultados);
    //     for (int i = 0; i < cliente.cantidad_resultados; i++) {
    //         printf("Resultado [%d]: %s\n", i, cliente.resultado[i]);
    //         free(cliente.resultado[i]); // Importante liberar la memoria de cada string
    //     }
    //     free(cliente.resultado); // Liberar el arreglo de punteros
    // } else {
    //     printf("No se encontraron registros para la llave: %s\n", cliente.llave);
    // }
}

// Implementación de la función del hilo
void* funcion_h1(void *arg)
{
    // Convertimos el puntero genérico de vuelta a nuestro tipo de estructura
    HILO_DIR *args = (HILO_DIR *)arg;

    // Usamos los datos para llamar a la función original
    crear_directorio(args->dir, args->carpeta, args->archivos, args->extension, args->tamano);

    return NULL;
}