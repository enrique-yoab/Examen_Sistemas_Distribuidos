#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_SERVIDOR "192.168.1.20" // Esta es la ip del servidor 
#define PUERTO 3000
#define TAM_MAX 1024

void realizar_consulta(int sock);
void realizar_insercion(int sock);
void realizar_eliminacion(int sock);
void realizar_actualizacion(int sock);

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
        printf("\n===============================================================================\n");
        printf(" COMANDOS ACTIVOS: CONSULTA | INSERCION | ELIMINACION | ACTUALIZACION | OFF |\n");
        printf("=============================================================================\n");
        printf("Tu mensaje (Comando): ");
        
        bzero(buffer, TAM_MAX);
        fgets(buffer, TAM_MAX, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strstr(buffer, "OFF") != NULL) {
            printf("[!] Cerrando cliente...\n");
            send(sock, "OFF", 3, 0); 
            break;
        }
        else if (strstr(buffer, "CONSULTA") != NULL) {
            realizar_consulta(sock);
        }
        else if(strstr(buffer, "INSERCION") != NULL)
        {
            realizar_insercion(sock);
        }
        else if(strstr(buffer, "ELIMINACION") != NULL)
        {
            realizar_eliminacion(sock);
        }
        else if(strstr(buffer, "ACTUALIZACION") != NULL)
        {
            realizar_actualizacion(sock);
        }else {
            printf("[-] Comando no reconocido. Escribe: CONSULTA, INSERCION, ACTUALIZACION, ELIMINACION o OFF.\n");
        }
    }

    close(sock);
    printf("\nDesconectado exitosamente.\n");
    return 0;
}

void realizar_consulta(int sock) {
    char tabla[10], llave[50], mascara[50], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];

    printf("\n\t--- MODO CONSULTA ---\n");
    printf("[ESTUDIANTE,0]\t\t[DIRECCION,1]\n[CARRERA,2]\t\t[HISTORIAL,3]\n[INSCRIPCION,4]\t\t[SECCION,5]\n[PROFESOR,6]\t\t[DEPARTAMENTO,7]\n");
    printf("[NIVELES,8]\t\t[HORARIO,9]\n[GRADO,10]\t\t[CURSO,11]\n[AÑOS,12]\t\t[SEMESTRE,13]\n");
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
        if(strstr(llave, "full") != NULL || strstr(llave, "FULL") != NULL || strstr(llave, "low") != NULL || strstr(llave, "LOW") != NULL ) total_filas--;
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

void realizar_insercion(int sock)
{
    char tabla[10], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];
    char datos_csv[TAM_MAX] = ""; // Aquí armaremos la fila con comas
    char entrada_temporal[256]; //El buffer que almacenara lo del teclado

    printf("\n--- MODO INSERCION ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    // FASE 1: Enviar petición inicial al servidor (Operación 2 | Tabla)
    sprintf(paquete_envio, "2|%s", tabla);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    // FASE 2: Esperar respuesta del servidor (Debería ser "COLUMNAS|N")
    bzero(respuesta_servidor, sizeof(respuesta_servidor));
    recv(sock, respuesta_servidor, sizeof(respuesta_servidor), 0);

    if (strncmp(respuesta_servidor, "COLUMNAS|", 9) == 0) {
        
        // Extraemos el número de columnas que el servidor contó
        int num_columnas = atoi(respuesta_servidor + 9);
        printf("[*] El servidor verifico la tabla. Se requieren %d columnas.\n", num_columnas);
        printf("--------------------------------------------------\n");

        // Preparamos el inicio del paquete final
        strcpy(datos_csv, "DATOS|"); 

        // FASE 3: Bucle interactivo pidiendo al usuario dato por dato
        for (int i = 0; i < num_columnas; i++) {
            if (i == 0) {
                printf("Ingrese el dato para la columna %d (LLAVE PRIMARIA): ", i + 1);
            } else {
                printf("Ingrese el dato para la columna %d: ", i + 1);
            }
            
            bzero(entrada_temporal, sizeof(entrada_temporal));
            fgets(entrada_temporal, sizeof(entrada_temporal), stdin);
            entrada_temporal[strcspn(entrada_temporal, "\n")] = 0; // Quitar salto de línea

            // Concatenamos el dato ingresado a nuestra cadena principal
            strcat(datos_csv, entrada_temporal); 

            // Si no es la última columna, le agregamos una coma separadora
            if (i < num_columnas - 1) {
                strcat(datos_csv, ",");
            }
        }

        // FASE 4: Enviar la fila completa al servidor
        printf("\n[Enviando paquete]: %s\n", datos_csv);
        send(sock, datos_csv, strlen(datos_csv), 0);

        // FASE 5: Esperar el veredicto final (Éxito o Error por llave duplicada)
        bzero(respuesta_servidor, sizeof(respuesta_servidor));
        recv(sock, respuesta_servidor, sizeof(respuesta_servidor), 0);

        printf("[Respuesta del Servidor]: %s\n", respuesta_servidor);

    } else {
        // Si el servidor no respondió "COLUMNAS|", significa que hubo un error (ej. tabla inválida)
        printf("\n[Servidor rechazo la peticion]: %s\n", respuesta_servidor);
    }
}

void realizar_eliminacion(int sock) {
    char tabla[10], llave[50], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];

    printf("\n--- MODO ELIMINACION ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;

    printf("Ingresa el ID (llave primaria) a eliminar: ");
    fgets(llave, sizeof(llave), stdin); llave[strcspn(llave, "\n")] = 0;

    // FASE 1: Enviamos Operacion(3) | Tabla | Llave
    sprintf(paquete_envio, "3|%s|%s", tabla, llave);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    // FASE 2: Esperamos la respuesta del servidor
    bzero(respuesta_servidor, TAM_MAX);
    recv(sock, respuesta_servidor, TAM_MAX, 0);
    printf("\n[Respuesta del Servidor]: %s\n", respuesta_servidor);
}

void realizar_actualizacion(int sock) {
    char tabla[10], llave_vieja[50], paquete_envio[TAM_MAX], respuesta_servidor[TAM_MAX];
    char datos_csv[TAM_MAX] = "";
    char entrada_temporal[256];

    printf("\n--- MODO ACTUALIZACION ---\n");
    printf("Ingresa el numero de tabla (0-13): ");
    fgets(tabla, sizeof(tabla), stdin); tabla[strcspn(tabla, "\n")] = 0;
    
    printf("Ingresa la llave ORIGINAL del registro a modificar: ");
    fgets(llave_vieja, sizeof(llave_vieja), stdin); llave_vieja[strcspn(llave_vieja, "\n")] = 0;

    // FASE 1: Le enviamos la tabla y la llave vieja al servidor para ver si existe
    sprintf(paquete_envio, "4|%s|%s", tabla, llave_vieja);
    send(sock, paquete_envio, strlen(paquete_envio), 0);

    bzero(respuesta_servidor, TAM_MAX);
    recv(sock, respuesta_servidor, TAM_MAX, 0);

    // FASE 2: Si el servidor responde con las columnas, significa que el registro SÍ existe
    if (strncmp(respuesta_servidor, "COLUMNAS|", 9) == 0) {
        
        int num_columnas = atoi(respuesta_servidor + 9);
        printf("[*] Registro encontrado. Ingresa los nuevos datos (%d columnas).\n", num_columnas);
        printf("--------------------------------------------------\n");

        strcpy(datos_csv, "DATOS|"); 

        // FASE 3: Bucle interactivo para armar la nueva fila
        for (int i = 0; i < num_columnas; i++) {
            if (i == 0) {
                printf("NUEVA Llave Primaria (Puede ser la misma '%s'): ", llave_vieja);
            } else {
                printf("NUEVO dato para la columna %d: ", i + 1);
            }
            
            bzero(entrada_temporal, sizeof(entrada_temporal));
            fgets(entrada_temporal, sizeof(entrada_temporal), stdin);
            entrada_temporal[strcspn(entrada_temporal, "\n")] = 0;

            strcat(datos_csv, entrada_temporal); 
            if (i < num_columnas - 1) strcat(datos_csv, ",");
        }

        // FASE 4: Enviar la fila completa
        printf("\n[Enviando actualizacion]: %s\n", datos_csv);
        send(sock, datos_csv, strlen(datos_csv), 0);

        // FASE 5: Recibir éxito o error (ej. si la nueva llave choca con otra)
        bzero(respuesta_servidor, TAM_MAX);
        recv(sock, respuesta_servidor, TAM_MAX, 0);
        printf("[Respuesta del Servidor]: %s\n", respuesta_servidor);

    } else {
        // Si no respondió "COLUMNAS|", es un error (seguramente la llave no existe)
        printf("\n[Servidor rechazo la peticion]: %s\n", respuesta_servidor);
    }
}