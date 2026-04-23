#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "./extensiones/entidades.h"
#include "./extensiones/consulta.h"
#include "./extensiones/insercion.h"

#define PUERTO 3000
#define TAM_MAX 500 
// Variables globales
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

    // Levantamos el servicio y obtenemos el socket principal
    int server_fd = levantar_servicio(PUERTO);
    
    int cliente_fd;
    struct sockaddr_in dest;
    socklen_t tam_dest = sizeof(dest);
    char buffer_recepcion[TAM_MAX];

    printf("Esperando conexiones entrantes de clientes...\n");

    // 5. Aceptar la conexión (Se bloquea aquí hasta que el cliente se conecte)
    if ((cliente_fd = accept(server_fd, (struct sockaddr *)&dest, &tam_dest)) < 0) {
        perror("Fallo en accept");
        exit(EXIT_FAILURE);
    }

    printf("\n[+] ¡Cliente conectado exitosamente!\n");

    // --- PRUEBA DE COMUNICACIÓN INICIAL ---
    bzero(buffer_recepcion, TAM_MAX);
    
    
    char *respuesta = "Bienvenido al Gestor de Base de Datos Distribuida\n";
    send(cliente_fd, respuesta, strlen(respuesta), 0);
    
    // Con TCP usamos 'recv' y 'send' en lugar de 'recvfrom' y 'sendto'
    recv(cliente_fd, buffer_recepcion, TAM_MAX, 0);
    printf("El cliente dice: %s\n", buffer_recepcion);
    // ... (Aquí irá tu lógica para desarmar el string, llenar las estructuras CONSULTA/INSERCION y procesarlas) ...

    // Al terminar, cierras el socket del cliente
    close(cliente_fd);
    
    // Y si el servidor se apagara, cierras el principal
    close(server_fd);

    return 0;
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