#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "./extensiones/entidades.h"

#define IP_SERVIDOR "192.168.1.15"
#define PUERTO 3000
#define TAM_MAX 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[TAM_MAX];

    printf("[1] Creando socket TCP...\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR: Fallo al crear el socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PUERTO);

    if (inet_pton(AF_INET, IP_SERVIDOR, &serv_addr.sin_addr) <= 0) {
        perror("ERROR: Direccion IP invalida");
        return -1;
    }

    printf("[2] Conectando al servidor %s:%d...\n", IP_SERVIDOR, PUERTO);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR: Conexion fallida (¿Servidor apagado?)");
        return -1;
    }

    // --- PRUEBA DE RECEPCIÓN ---
    bzero(buffer, TAM_MAX);
    recv(sock, buffer, TAM_MAX, 0);
    printf("[3] Mensaje del servidor: %s", buffer);

    // --- PRUEBA DE ENVÍO (Simulando una inserción en Semestre) ---
    // Protocolo: "2" (Insercion) | "13" (Tabla Semestre) | "1555,8vo_semestre" (Datos)
    char *paquete_prueba = "2|13|1555,8vo_semestre";
    
    printf("[4] Enviando paquete de prueba: %s\n", paquete_prueba);
    send(sock, paquete_prueba, strlen(paquete_prueba), 0);

    // --- CERRAR CONEXIÓN ---
    printf("[5] Prueba completada. Cerrando conexion.\n");
    close(sock);

    return 0;
}