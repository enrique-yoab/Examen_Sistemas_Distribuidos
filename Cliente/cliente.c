#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_SERVIDOR "127.0.0.1" // Cambiar por la IP real en el examen (ej. 192.168.1.15)
#define PUERTO 3000
#define TAM_MAX 1024

// --- PROTOTIPOS DE LAS FUNCIONES ---
void realizar_consulta(int sock);
void realizar_insercion(int sock);
void realizar_eliminacion(int sock);
void realizar_actualizacion(int sock);

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[TAM_MAX];

    printf("[1] Creando socket TCP...\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PUERTO);
    if (inet_pton(AF_INET, IP_SERVIDOR, &serv_addr.sin_addr) <= 0) {
        printf("Direccion IP invalida\n");
        return -1;
    }

    printf("[2] Conectando al servidor...\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Conexion fallida");
        return -1;
    }

    bzero(buffer, TAM_MAX);
    recv(sock, buffer, TAM_MAX, 0);
    printf("\n[Mensaje Inicial del Servidor]: %s", buffer);

    // --- BUCLE INTERACTIVO PRINCIPAL ---
    while(1) {
        printf("\n=====================================================================\n");
        printf(" COMANDOS: CONSULTA | INSERCION | ELIMINACION | ACTUALIZACION | OFF\n");
        printf("=====================================================================\n");
        printf("Tu mensaje (Comando): ");
        
        bzero(buffer, TAM_MAX);
        fgets(buffer, TAM_MAX, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // 1. CONDICIÓN DE SALIDA
        if (strcasecmp(buffer, "OFF") == 0) {
            printf("[!] Cerrando cliente...\n");
            send(sock, "OFF", 3, 0); 
            break;
        }
        // 2. ENRUTAMIENTO DE COMANDOS
        else if (strcasecmp(buffer, "CONSULTA") == 0) {
            realizar_consulta(sock);
        }   
        else if (strcasecmp(buffer, "INSERCION") == 0) {
            realizar_insercion(sock);
        }
        else if (strcasecmp(buffer, "ELIMINACION") == 0) {
            realizar_eliminacion(sock);
        }
        else if (strcasecmp(buffer, "ACTUALIZACION") == 0) {
            realizar_actualizacion(sock);
        }
        else {
            printf("[-] Comando no reconocido. Intenta escribir: CONSULTA, INSERCION, ELIMINACION, ACTUALIZACION o OFF.\n");
        }
    }

    close(sock);
    printf("\nDesconectado exitosamente.\n");
    return 0;
}

// --- IMPLEMENTACIÓN DE LOS MÓDULOS ---

void realizar_consulta(int sock) {
    char tabla[10], llave[50], mascara[50], paquete_envio[TAM_MAX], respuesta_servidor[4096];

    printf("\n--- MODO CONSULTA ---\n");
    printf("Ingresa el numero de tabla (ej. 0 para Estudiante): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa la llave a buscar ('full', 'low' o ID): ");
    fgets(llave, sizeof(llave), stdin); llave[strcspn(llave, "\n")] = 0;

    printf("Ingresa la mascara de parametros (Bits = num. columnas): ");
    fgets(mascara, sizeof(mascara), stdin); mascara[strcspn(mascara, "\n")] = 0;

    // Formato: Operacion(1)|Tabla|Llave|Mascara
    sprintf(paquete_envio, "1|%s|%s|%s", tabla, llave, mascara);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    bzero(respuesta_servidor, sizeof(respuesta_servidor));
    recv(sock, respuesta_servidor, sizeof(respuesta_servidor), 0);
    printf("\n--- RESULTADOS DE LA BD ---\n%s\n---------------------------\n", respuesta_servidor);
}

void realizar_insercion(int sock) {
    char tabla[10], datos[512], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];

    printf("\n--- MODO INSERCION ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa los datos separados por coma: ");
    fgets(datos, sizeof(datos), stdin); datos[strcspn(datos, "\n")] = 0;

    // Formato: Operacion(2)|Tabla|Datos
    sprintf(paquete_envio, "2|%s|%s", tabla, datos);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    bzero(respuesta_servidor, TAM_MAX);
    recv(sock, respuesta_servidor, TAM_MAX, 0);
    printf("\n--- RESULTADOS DE LA BD ---\n%s\n---------------------------\n", respuesta_servidor);
}

void realizar_eliminacion(int sock) {
    char tabla[10], llave[50], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];

    printf("\n--- MODO ELIMINACION ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa el ID (llave primaria) a eliminar: ");
    fgets(llave, sizeof(llave), stdin); llave[strcspn(llave, "\n")] = 0;

    // Formato: Operacion(3)|Tabla|Llave
    sprintf(paquete_envio, "3|%s|%s", tabla, llave);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    bzero(respuesta_servidor, TAM_MAX);
    recv(sock, respuesta_servidor, TAM_MAX, 0);
    printf("\n--- RESULTADOS DE LA BD ---\n%s\n---------------------------\n", respuesta_servidor);
}

void realizar_actualizacion(int sock) {
    char tabla[10], datos[512], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];

    printf("\n--- MODO ACTUALIZACION ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa los NUEVOS datos de la fila separados por coma (incluye la llave original): ");
    fgets(datos, sizeof(datos), stdin); datos[strcspn(datos, "\n")] = 0;

    // Formato: Operacion(4)|Tabla|Datos
    sprintf(paquete_envio, "4|%s|%s", tabla, datos);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    bzero(respuesta_servidor, TAM_MAX);
    recv(sock, respuesta_servidor, TAM_MAX, 0);
    printf("\n--- RESULTADOS DE LA BD ---\n%s\n---------------------------\n", respuesta_servidor);
}