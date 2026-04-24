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
#define TAM_MAX 1024

// Variáveis globais do archivero
ARCHIVERO directorio;
char *escritura = "./buffer/";
char *carpeta = "./Datos/";
char *extension = ".csv";
char *archivos[] = {"Estudiante", "Direccion", "Carrera", "Historial", "Inscripcion", "Seccion", "Profesor", "Departamento", "Niveles", "Horario", "Grado", "Curso", "Años", "Semestre"};
int tamano = sizeof(archivos) / sizeof(archivos[0]);

void* funcion_h1(void *arg);

int main(void){
    // 1. INICIALIZACION DEL DIRECTORIO (Hilo)
    pthread_t hilo1; 
    HILO_DIR datos = {&directorio, carpeta, escritura, archivos, extension, tamano};
    pthread_create(&hilo1, NULL, funcion_h1, (void *)&datos);
    pthread_join(hilo1, NULL);

    // 2.LEVANTAR SERVICIO TCP
    int server_fd = levantar_servicio(PUERTO);
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int nuevo_socket;
    char buffer[TAM_MAX];
    char respuesta_buffer[4096];

    printf("\n[+] Servidor iniciado. Aguardando uma única conexão...\n");

    nuevo_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (nuevo_socket < 0) {
        perror("ERROR: Falha ao aceitar conexão");
        close(server_fd);
        return -1;
    }

    printf("[+] Cliente conectado. Iniciando sesion única.\n");
    send(nuevo_socket, "Conexion establecida, el servidor finalizara despues de la session.\n", 33, 0);

    // 3. LOOP DE INTERACCION CON EL CLIENTE
    while(1) {
        bzero(buffer, TAM_MAX);
        int bytes_leidos = recv(nuevo_socket, buffer, TAM_MAX, 0);
        
        // Si el cliente se desconecta o envia OFF
        if (bytes_leidos <= 0 || strcasecmp(buffer, "OFF") == 0) {
            printf("[-] Cliente desconectado con comando OFF recibido. Finalizando...\n");
            break;
        }

        printf("\n[Peticion Recibida]: %s\n", buffer);
        bzero(respuesta_buffer, sizeof(respuesta_buffer));

        // Desempaquetamos la peticion del cliente
        char *op_str = strtok(buffer, "|");
        if (op_str == NULL) continue;
        int operacion = atoi(op_str);

        char *tabla_str = strtok(NULL, "|");
        if (tabla_str == NULL) continue;
        int tabla = atoi(tabla_str);

        // PETICION: CONSULTA
        if (operacion == 1) {
            char *llave = strtok(NULL, "|");
            char *mascara = strtok(NULL, ""); 

            if (llave != NULL && mascara != NULL) {
                CONSULTA c = {tabla, NULL, mascara, llave, 0, NULL};
                solicitud_consulta(&c, &directorio);

                if (c.error != NULL) {
                    strcpy(respuesta_buffer, c.error);
                    free(c.error);
                } else if (c.cantidad_resultados == 0) {
                    strcpy(respuesta_buffer, "No hay registro encontrado.\n");
                }
                
                if (c.cantidad_resultados > 0) {
                    strcat(respuesta_buffer, "Registros Encontrados:\n");
                    for (int i = 0; i < c.cantidad_resultados; i++) {
                        strcat(respuesta_buffer, c.resultado[i]);
                        strcat(respuesta_buffer, "\n");
                        free(c.resultado[i]);
                    }
                    free(c.resultado);
                }
            }
        } else {
            strcpy(respuesta_buffer, "El servidor, no reconoce esta indicacion\n");
        }

        // ENVIAR RESPUESTA
        send(nuevo_socket, respuesta_buffer, strlen(respuesta_buffer), 0);
    }

    // FINALIZACION DE SERVIDOR
    close(nuevo_socket);
    close(server_fd);
    printf("[*] Servidor apagado...\n");

    return 0;
}

void* funcion_h1(void *arg) {
    HILO_DIR *args = (HILO_DIR *)arg;
    crear_directorio(args->dir, args->carpeta, args->archivos, args->extension, args->update, args->tamano);
    return NULL;
}