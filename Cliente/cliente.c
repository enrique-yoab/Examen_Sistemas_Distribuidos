#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_SERVIDOR "127.0.0.1" // Cambiar a la IP de tu red para la prueba real
#define PUERTO 3000
#define TAM_MAX 1024

char *tablas[] = {"Estudiante", "Direccion", "Carrera", "Historial", "Inscripcion", "Seccion", "Profesor", "Departamento", "Niveles", "Horario", "Grado", "Curso", "Años", "Semestre"};

void realizar_consulta(int sock);
void realizar_insercion(int sock);

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
    char tabla[10], llave[50], mascara[50], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];

    printf("\n--- MODO CONSULTA ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa la llave a buscar ('full', 'low' o ID): ");
    fgets(llave, sizeof(llave), stdin); llave[strcspn(llave, "\n")] = 0;

    printf("Ingresa la mascara de parametros (Bits 1/0): ");
    fgets(mascara, sizeof(mascara), stdin); mascara[strcspn(mascara, "\n")] = 0;

    sprintf(paquete_envio, "1|%s|%s|%s", tabla, llave, mascara);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    // --- NUEVA LÓGICA DE RECEPCIÓN ---
    bzero(respuesta_servidor, sizeof(respuesta_servidor));
    recv(sock, respuesta_servidor, sizeof(respuesta_servidor), 0);
    
    // Verificamos si el servidor activó el modo de envío por renglones
    if (strncmp(respuesta_servidor, "CANTIDAD|", 9) == 0) {
        
        // Extraemos el número que viene después de "CANTIDAD|"
        int total_filas = atoi(respuesta_servidor + 9);
        printf("\n--- RESULTADOS DE LA BD (%d registros) ---\n", total_filas);
        // Abrimos el archivo en donde se guardara la consulta
        FILE *archivo = fopen("./consulta.csv", "w");
        // Le decimos al servidor: "Estoy listo, manda el primero" (ACK)
        send(sock, "ACK", 3, 0);

        // Bucle para recibir uno por uno
        for(int i = 0; i < total_filas; i++) {
            bzero(respuesta_servidor, sizeof(respuesta_servidor));
            recv(sock, respuesta_servidor, sizeof(respuesta_servidor), 0);
            
            // Imprimimos el renglón recibido
            printf("[%d]: %s\n", i, respuesta_servidor);
            fprintf(archivo,"%s\n",respuesta_servidor);
            // Confirmamos recepción (ACK) para que el servidor mande el siguiente
            send(sock, "ACK", 3, 0); 
        }
        printf("----------------------------------------\n");
        fclose(archivo);
    } else {
        // Si no era "CANTIDAD|", significa que es un Error o un "No se encontraron registros"
        printf("\n[Servidor]: %s\n", respuesta_servidor);
    }
}