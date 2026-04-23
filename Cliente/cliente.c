#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// #include "./extensiones/entidades.h" // Descomenta tus includes

#define IP_SERVIDOR "127.0.0.1" 
#define PUERTO 3000
#define TAM_MAX 1024

// --- PROTOTIPO DE LA NUEVA FUNCIÓN ---
void realizar_consulta(int sock);

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[TAM_MAX];

    // ... [Todo tu código de conexión inicial se queda igual] ...
    printf("[1] Creando socket TCP...\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PUERTO);
    if (inet_pton(AF_INET, IP_SERVIDOR, &serv_addr.sin_addr) <= 0) return -1;

    printf("[2] Conectando al servidor...\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) return -1;

    bzero(buffer, TAM_MAX);
    recv(sock, buffer, TAM_MAX, 0);
    printf("\n[Mensaje Inicial del Servidor]: %s", buffer);

    printf("====================================================\n");
    printf(" Escribe 'CONSULTA' para buscar o 'OFF' para salir\n");
    printf("====================================================\n");

    // --- BUCLE INTERACTIVO PRINCIPAL ---
    while(1) {
        printf("\nTu mensaje (Comando): ");
        bzero(buffer, TAM_MAX);
        fgets(buffer, TAM_MAX, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // 1. CONDICIÓN DE SALIDA
        if (strcmp(buffer, "OFF") == 0) {
            printf("[!] Comando de apagado enviado. Cerrando cliente...\n");
            send(sock, buffer, strlen(buffer), 0); // Le avisamos al server que nos vamos
            break;
        }

        // 2. ENTRADA AL MÓDULO DE CONSULTAS
        if (strcmp(buffer, "CONSULTA") == 0) {
            realizar_consulta(sock);
            continue; // Evita que siga ejecutando lo de abajo y vuelve al inicio del while
        }   

        // 3. MENSAJE GENÉRICO (Si escribe algo que no es ni OFF ni CONSULTA)
        send(sock, buffer, strlen(buffer), 0);
        bzero(buffer, TAM_MAX);
        int bytes_recibidos = recv(sock, buffer, TAM_MAX, 0);

        if (bytes_recibidos <= 0) {
            printf("\n[-] El servidor cerro la conexion.\n");
            break;
        }
        printf("[Servidor responde]: %s\n", buffer);
    }

    close(sock);
    printf("\nDesconectado exitosamente.\n");
    return 0;
}

// --- IMPLEMENTACIÓN DEL MÓDULO DE CONSULTA ---
void realizar_consulta(int sock) {
    char tabla[10], llave[50], mascara[50];
    char paquete_envio[TAM_MAX];
    char respuesta_servidor[TAM_MAX];

    printf("\n--- MODO CONSULTA ---\n");
    
    // 1. Recopilamos los datos localmente
    printf("Ingresa el numero de tabla (ej. 0 para Estudiante): ");
    fgets(tabla, sizeof(tabla), stdin);
    tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa la llave a buscar (ej. 12345): ");
    fgets(llave, sizeof(llave), stdin);
    llave[strcspn(llave, "\n")] = 0;

    printf("Ingresa la mascara de parametros (ej. 10110): ");
    fgets(mascara, sizeof(mascara), stdin);
    mascara[strcspn(mascara, "\n")] = 0;

    // 2. Empaquetamos la cadena con un formato estricto: "COMANDO|TABLA|LLAVE|MASCARA"
    sprintf(paquete_envio, "CONSULTA|%s|%s|%s", tabla, llave, mascara);

    printf("[Enviando al servidor]: %s\n", paquete_envio);

    // 3. Enviamos el paquete completo
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    // 4. Esperamos los resultados que nos devuelva el servidor
    bzero(respuesta_servidor, TAM_MAX);
    recv(sock, respuesta_servidor, TAM_MAX, 0);

    printf("\n--- RESULTADOS DE LA BD ---\n");
    printf("%s\n", respuesta_servidor);
    printf("---------------------------\n");
}