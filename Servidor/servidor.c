#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "./extensiones/entidades.h"
#include "./extensiones/consulta.h"

#define PUERTO 3000

// Variables globales
ARCHIVERO directorio;
char *escritura = "./buffer/";
char *carpeta = "./Datos/";
char *extension = ".csv";
char *archivos[] = {"Estudiante", "Direccion", "Carrera", "Historial", "Inscripcion", "Seccion", "Profesor", "Departamento", "Niveles", "Horario", "Grado", "Curso", "Años", "Semestre"};
int tamano = sizeof(archivos) / sizeof(archivos[0]);
int TAM_MAX = 1024;

// Prototipos de funciones
void* funcion_h1(void *arg);
void* funcion_h2(void *arg);
void procesar_consulta(int socket_cliente, int tabla, char *llave, char *mascara, ARCHIVERO *dir);

int main(void){
    // 1. INICIALIZACION DEL DIRECTORIO Y LA CONEXION TCP CON HILOS
    pthread_t hilo_directorio, hilo_red;
    HILO_DIR datos_dir = {&directorio, carpeta, escritura, archivos, extension, tamano};
    HILO_RED datos_red = {PUERTO, -1}; 
    
    printf("[Servidor] Iniciando hilos de preparacion...\n");
    pthread_create(&hilo_directorio, NULL, funcion_h1, (void *)&datos_dir);
    pthread_create(&hilo_red, NULL, funcion_h2, (void *)&datos_red);

    pthread_join(hilo_directorio, NULL);
    pthread_join(hilo_red, NULL);

    int server_fd = datos_red.socket_fd_resultado;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int nuevo_socket;
    char buffer[TAM_MAX];

    printf("\n[+] Servidor iniciado. Esperando conexion...\n");

    nuevo_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (nuevo_socket < 0) {
        perror("ERROR: Falla al intentar conectar");
        close(server_fd);
        return -1;
    }

    printf("[+] Cliente conectado. Iniciando sesion única.\n");
    send(nuevo_socket, "Conexion establecida, el servidor finalizara despues de la sesion.\n", 67, 0);

    // 2. LOOP DE INTERACCION CON EL CLIENTE
    while(1) {
        bzero(buffer, TAM_MAX);
        int bytes_leidos = recv(nuevo_socket, buffer, TAM_MAX, 0);
        
        if (bytes_leidos <= 0 || strcasecmp(buffer, "OFF") == 0) {
            printf("[-] Cliente desconectado con comando OFF recibido. Finalizando...\n");
            break;
        }

        printf("\n[Peticion Recibida]: %s\n", buffer);

        // Desempaquetamos los encabezados de la peticion
        char *op_str = strtok(buffer, "|");
        if (op_str == NULL) continue;
        int operacion = atoi(op_str);

        char *tabla_str = strtok(NULL, "|");
        if (tabla_str == NULL) continue;
        int tabla = atoi(tabla_str);

        // 3. ENRUTADOR DE OPERACIONES (Mini-Switch)
        if (operacion == 1) {
            char *llave = strtok(NULL, "|");
            char *mascara = strtok(NULL, ""); 
            
            // Delegamos todo el trabajo a la función experta
            procesar_consulta(nuevo_socket, tabla, llave, mascara, &directorio);
        }
        // Aquí se agregara los 'else if' para UPDATE (2), DELETE (3) e INSERT (4) en el futuro.
        else {
            char *msj_error = "ERROR: Operacion no reconocida por el servidor.\n";
            send(nuevo_socket, msj_error, strlen(msj_error), 0);
        }
    }

    // FINALIZACION DE SERVIDOR
    close(nuevo_socket);
    close(server_fd);
    printf("[*] Servidor apagado...\n");

    return 0;
}

// =======================================================================
// HILOS DE INICIALIZACIÓN
// =======================================================================

void* funcion_h1(void *arg) {
    HILO_DIR *args = (HILO_DIR *)arg;
    printf("[Hilo DIR] Creando directorio de rutas para la base de datos...\n");
    crear_directorio(args->dir, args->carpeta, args->archivos, args->extension, args->update, args->tamano);
    return NULL;
}

void* funcion_h2(void *arg) {
    HILO_RED *args = (HILO_RED *)arg;
    printf("[Hilo RED] Levantando servicio TCP en el puerto %d...\n", args->puerto);
    args->socket_fd_resultado = levantar_servicio(args->puerto);
    return NULL;
}