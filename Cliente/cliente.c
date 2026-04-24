#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_SERVIDOR "127.0.0.1" // Cambiar a la IP de tu red para la prueba real
#define PUERTO 3000
#define TAM_MAX 1024

void realizar_consulta(int sock);

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[TAM_MAX];

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

    // --- BUCLE INTERACTIVO ---
    while(1) {
        printf("\n=====================================\n");
        printf(" COMANDOS ACTIVOS: CONSULTA | OFF\n");
        printf("=====================================\n");
        printf("Tu mensaje (Comando): ");
        
        bzero(buffer, TAM_MAX);
        fgets(buffer, TAM_MAX, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcasecmp(buffer, "OFF") == 0) {
            printf("[!] Cerrando cliente...\n");
            send(sock, "OFF", 3, 0); 
            break;
        }
        else if (strcasecmp(buffer, "CONSULTA") == 0) {
            realizar_consulta(sock);
        }   
        else {
            printf("[-] Comando no reconocido. Escribe: CONSULTA o OFF.\n");
        }
    }

    close(sock);
    printf("\nDesconectado exitosamente.\n");
    return 0;
}

void realizar_consulta(int sock) {
    char tabla[10], llave[50], mascara[50], paquete_envio[TAM_MAX], respuesta_servidor[4096];

    printf("\n--- MODO CONSULTA ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa la llave a buscar ('full', 'low' o ID): ");
    fgets(llave, sizeof(llave), stdin); llave[strcspn(llave, "\n")] = 0;

    printf("Ingresa la mascara de parametros (Bits 1/0): ");
    fgets(mascara, sizeof(mascara), stdin); mascara[strcspn(mascara, "\n")] = 0;

    // Empaquetado estricto
    sprintf(paquete_envio, "1|%s|%s|%s", tabla, llave, mascara);
    
    // Enviamos al servidor
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    // Esperamos resultados
    bzero(respuesta_servidor, sizeof(respuesta_servidor));
    recv(sock, respuesta_servidor, sizeof(respuesta_servidor), 0);
    
    printf("\n--- RESULTADOS DE LA BD ---\n%s\n---------------------------\n", respuesta_servidor);
}