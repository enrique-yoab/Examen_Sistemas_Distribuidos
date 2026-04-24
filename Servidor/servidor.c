#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "./extensiones/entidades.h"
#include "./extensiones/consulta.h"
#include "./extensiones/insercion.h"
#include "./extensiones/update.h"

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
            printf("[-] Cliente desconectado o se recibio comando OFF. Finalizando...\n");
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

        // ==========================================
        // OPERACION 1: CONSULTA
        // ==========================================
        if (operacion == 1) {
            char *llave = strtok(NULL, "|");
            char *mascara = strtok(NULL, ""); 
            
            // Delegamos todo el trabajo a la función
            procesar_consulta(nuevo_socket, tabla, llave, mascara, &directorio);
        }
        // ==========================================
        // OPERACION 2: INSERCION
        // ==========================================
        else if(operacion == 2) 
        {
            // FASE 1: Descubrimiento de esquema (Columnas)
            char *ruta_archivo = directorio.rutas[tabla];
            ANALISIS_ARCHIVO analisis = analizar_archivo(ruta_archivo);
            
            char msj_columnas[50];
            // Le enviamos al cliente la etiqueta COLUMNAS| seguida del número
            sprintf(msj_columnas, "COLUMNAS|%d", analisis.num_columnas);
            send(nuevo_socket, msj_columnas, strlen(msj_columnas), 0);

            // FASE 2: Esperamos recibir los datos empaquetados por el cliente
            bzero(buffer, TAM_MAX);
            int bytes_datos = recv(nuevo_socket, buffer, TAM_MAX, 0);

            if (bytes_datos > 0 && strncmp(buffer, "DATOS|", 6) == 0) {
                // Brincamos los primeros 6 caracteres ("DATOS|") para obtener solo el CSV
                char *datos_csv = buffer + 6; 

                // Preparamos la estructura
                INSERCION peticion_ins;
                peticion_ins.numero_tabla = tabla;
                peticion_ins.datos = datos_csv;
                peticion_ins.error = NULL;

                // Delegamos al motor físico (el que programamos en insercion.c)
                solicitud_insercion(&peticion_ins, &directorio);

                // FASE 3: Respondemos el resultado final (EXITO o ERROR de llave duplicada)
                if (peticion_ins.error != NULL) {
                    send(nuevo_socket, peticion_ins.error, strlen(peticion_ins.error), 0);
                    free(peticion_ins.error); // IMPORTANTE: Liberar el strdup
                } else {
                    char *err_desconocido = "ERROR: Fallo desconocido en insercion.\n";
                    send(nuevo_socket, err_desconocido, strlen(err_desconocido), 0);
                }
            } else {
                // Si el cliente mandó basura en lugar de "DATOS|..."
                char *err_proto = "ERROR: Se rompio el protocolo de insercion.\n";
                send(nuevo_socket, err_proto, strlen(err_proto), 0);
            }
        }
        // ==========================================
        // OPERACION 3: ELIMINACION
        // ==========================================
        else if (operacion == 3) {
            char *llave_eliminar = strtok(NULL, ""); // Extraemos la llave
            
            if (llave_eliminar != NULL) {
                ELIMINACION peticion_del;
                peticion_del.num_tabla = tabla;
                peticion_del.primary_key = llave_eliminar;
                peticion_del.error = NULL;

                solicitud_eliminacion(&peticion_del, &directorio);

                if (peticion_del.error != NULL) {
                    send(nuevo_socket, peticion_del.error, strlen(peticion_del.error), 0);
                    free(peticion_del.error);
                } else {
                    char *err = "ERROR: Fallo desconocido en eliminacion.\n";
                    send(nuevo_socket, err, strlen(err), 0);
                }
            }
        }
        // ==========================================
        // OPERACION 4: ACTUALIZACION
        // ==========================================
        else if (operacion == 4) {
            char *llave_vieja = strtok(NULL, ""); 
            
            // FASE 1: Verificamos que la llave original EXISTA en la tabla
            char *ruta_archivo = directorio.rutas[tabla];
            char *estado = validar_llave(llave_vieja, tabla, ruta_archivo);
            
            if (strcmp(estado, "EXITO") == 0) {
                // EXITO en validar_llave significa que la llave NO existe.
                char *msj = "ERROR: El registro que intentas actualizar no existe.\n";
                send(nuevo_socket, msj, strlen(msj), 0);
                continue; // Cancelamos la operación
            }

            // *** LA CURA AL BUG: RESPALDAR LA LLAVE ***
            // Hacemos una copia profunda de la llave para que no se destruya al limpiar el buffer
            char llave_respaldo[50];
            strcpy(llave_respaldo, llave_vieja);

            // FASE 2: Si existe, le decimos al cliente cuántas columnas necesitamos
            ANALISIS_ARCHIVO analisis = analizar_archivo(ruta_archivo);
            char msj_columnas[50];
            sprintf(msj_columnas, "COLUMNAS|%d", analisis.num_columnas);
            send(nuevo_socket, msj_columnas, strlen(msj_columnas), 0);

            // FASE 3: Esperamos los datos nuevos
            bzero(buffer, TAM_MAX); // Aquí se destruye el contenido original
            int bytes_datos = recv(nuevo_socket, buffer, TAM_MAX, 0);

            if (bytes_datos > 0 && strncmp(buffer, "DATOS|", 6) == 0) {
                char *datos_csv = buffer + 6;
                
                UPDATE peticion_upd;
                peticion_upd.num_tabla = tabla;
                peticion_upd.primary_key = llave_respaldo; // <-- USAMOS EL RESPALDO SEGURO
                peticion_upd.parametros = datos_csv;    
                peticion_upd.error = NULL;

                solicitud_update(&peticion_upd, &directorio);
                
                if (peticion_upd.error != NULL) {
                    send(nuevo_socket, peticion_upd.error, strlen(peticion_upd.error), 0);
                    free(peticion_upd.error);
                }else {
                    // Si es NULL, significa que pasó todas las validaciones y se actualizó
                    char *msj_exito = "EXITO: El registro fue actualizado correctamente en el servidor.\n";
                    send(nuevo_socket, msj_exito, strlen(msj_exito), 0);
                }
            } else {
                 char *err = "ERROR: Se rompio el protocolo de actualizacion.\n";
                 send(nuevo_socket, err, strlen(err), 0);
            }
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