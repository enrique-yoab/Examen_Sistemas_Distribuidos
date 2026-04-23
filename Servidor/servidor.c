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

    // 1. Mensaje de bienvenida (SE MUEVE FUERA DEL BUCLE)
    char *bienvenida = "Conexion lista, esperando peticiones...\n";
    send(cliente_fd, bienvenida, strlen(bienvenida), 0);

    // --- BUCLE DE COMUNICACIÓN ---
    while(1)
    {   
        bzero(buffer_recepcion, TAM_MAX);
        
        // 2. El servidor primero ESCUCHA
        int bytes = recv(cliente_fd, buffer_recepcion, TAM_MAX, 0);
        if (bytes <= 0) break; // Por si el cliente cierra de golpe

        printf("El cliente dice: %s\n", buffer_recepcion);

        // 3. Evalúa si es comando de apagado
        if(strstr(buffer_recepcion, "OFF") != NULL)
        {
            printf("[!] El cliente envio comando de cierre...\n");
            break;
        }

        // ... (Aquí irá tu lógica de strtok para desarmar la petición) ...
        if(strstr(buffer_recepcion, "CONSULTA") != NULL)
        {
            bzero(buffer_recepcion, TAM_MAX);
            printf("El cliente quiere una consulta a la BD...[Esperando configuraciones]\n");
        }
        // 4. El servidor RESPONDE a esa petición específica
        char *respuesta = "El mensaje fue recibido\n";
        send(cliente_fd, respuesta, strlen(respuesta), 0);
    }
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