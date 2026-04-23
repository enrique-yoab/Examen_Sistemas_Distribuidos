#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../extensiones/entidades.h"
#include "../extensiones/consulta.h"
#include "../extensiones/insercion.h"

extern ARCHIVERO directorio;

void imprimir_rutas(ARCHIVERO *dir)
{
    int i = 0;
    printf("---------> Mostrando rutas de tablas <---------\n");
    while (i < dir->cantidad)
    {
        printf("Ruta[%d] : %s \n", i + 1, dir->rutas[i]);
        i++;
    }
    printf("-----------------------------------------------\n");
    i = 0;
    printf("---------> Mostrando rutas de escritura <---------\n");
    while (i < dir->cantidad)
    {
        printf("Ruta[%d] : %s \n", i + 1, dir->rutas_update[i]);
        i++;
    }
    printf("--------------------------------------------------\n");
}

void crear_directorio(ARCHIVERO *dir, char *carpeta, char *archivos[], char *tipo, char *update, int cantidad_archivos)
{   
    printf("Se esta creando un directorio con %d rutas de archivos %s\n", cantidad_archivos, tipo);
    
    for(int i = 0; i < cantidad_archivos; i++)
    {
        // 1. Calculamos la memoria exacta para cada cadena por separado
        size_t len_carpeta = strlen(carpeta) + strlen(archivos[i]) + strlen(tipo) + 1;
        size_t len_update  = strlen(update) + strlen(archivos[i]) + strlen(tipo) + 1;

        // 2. Asignamos la memoria correcta a cada puntero
        char *ruta_completa = (char *)malloc(len_carpeta);
        char *ruta_updates  = (char *)malloc(len_update);

        // Buena práctica: Verificar que malloc no falle
        if (ruta_completa == NULL || ruta_updates == NULL) {
            printf("ERROR: Memoria insuficiente para crear rutas.\n");
            return;
        }

        // 3. Construimos la ruta principal (ej. "./Datos/Estudiante.csv")
        strcpy(ruta_completa, carpeta);
        strcat(ruta_completa, archivos[i]);
        strcat(ruta_completa, tipo);

        // 4. Construimos la ruta temporal (ej. "./buffer/Estudiante.csv")
        strcpy(ruta_updates, update);
        strcat(ruta_updates, archivos[i]);
        strcat(ruta_updates, tipo);

        // 5. Guardamos en el archivero
        dir->rutas[i] = ruta_completa;
        dir->rutas_update[i] = ruta_updates; // Asume que cambiaste a un arreglo en la estructura
        dir->cantidad = i + 1;
    }
    imprimir_rutas(dir);
}

ANALISIS_ARCHIVO analizar_archivo(char *ruta)
{
    ANALISIS_ARCHIVO analisis = {0, 0};
    FILE *archivo = fopen(ruta, "r");

    char linea[512];
    int primera_linea = 1;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        analisis.num_lineas++;
        if (primera_linea) {
            // Contar columnas: número de comas + 1
            char *ptr = linea;
            while (*ptr) {
                if (*ptr == ',') {
                    analisis.num_columnas++;
                }
                ptr++;
            }
            analisis.num_columnas++; // Contar ultimo renglon por sintaxis .csv
            primera_linea = 0;
        }
    }
    // Cerramos el archivo y regresamos el analisis
    fclose(archivo);
    return analisis;
}

char *validar_llave(char *llave, int num_table, char *ruta)
{
    // 1. Analizar el archivo para saber cuántas columnas reales tiene
    ANALISIS_ARCHIVO analisis = analizar_archivo(ruta);
    int columnas = analisis.num_columnas;

    // 2. Construir la máscara dinámica (ej. "10000" para 5 columnas)
    // Reservamos memoria para las columnas + el carácter nulo '\0'
    char *mascara = malloc(columnas + 1);
    if (mascara == NULL) return "ERROR: Memoria insuficiente";

    mascara[0] = '1'; // Activamos solo la primera columna (la llave)
    for (int i = 1; i < columnas; i++) {
        mascara[i] = '0'; // Apagamos el resto de las columnas
    }
    mascara[columnas] = '\0'; // Cerramos la cadena de C

    CONSULTA tmp;
    tmp.numero_tabla = num_table;
    tmp.error = NULL;
    tmp.llave = llave;
    tmp.cantidad_resultados = 0;
    tmp.resultado = NULL;
    tmp.parametros = mascara; // Asignamos nuestra cadena recién creada

    // Lanzamos la consulta pasándole el número real de columnas
    consulta_tabla(&tmp, &directorio, columnas);

    // 3. LIMPIEZA: Ya usamos la máscara, debemos liberarla
    free(mascara); 

    // 4. Verificamos si la consulta encontró la llave
    if (tmp.cantidad_resultados > 0)
    {
        // Liberamos la memoria de los resultados
        for (int i = 0; i < tmp.cantidad_resultados; i++) {
            free(tmp.resultado[i]);
        }
        free(tmp.resultado);

        return "ERROR: La llave se repite en la tabla\n";
    }

    return "EXITO";
}

int levantar_servicio(int puerto)
{
    int servidor_fd;
    struct sockaddr_in direct;
    int opt = 1;

    // 1. Crear el socket: AF_INET (IPv4), SOCK_STREAM (TCP)
    if ((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("ERROR: Fallo al crear el socket TCP");
        exit(EXIT_FAILURE);
    }

    // Opcional pero recomendado: Permite reutilizar el puerto inmediatamente 
    // si reinicias el servidor rápido durante tus pruebas
    setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Configurar la estructura de la dirección
    bzero(&direct, sizeof(direct));
    direct.sin_family = AF_INET;
    direct.sin_addr.s_addr = INADDR_ANY; // Escucha en cualquier interfaz de red
    direct.sin_port = htons(puerto);

    // 3. Vincular el socket al puerto (bind)
    if (bind(servidor_fd, (struct sockaddr *)&direct, sizeof(direct)) < 0) {
        perror("ERROR: Fallo en bind (¿Puerto en uso?)");
        exit(EXIT_FAILURE);
    }

    // 4. Poner el socket en modo escucha (listen) - ¡Exclusivo de TCP!
    // El '5' indica cuántas conexiones pueden esperar en la cola antes de ser rechazadas
    if (listen(servidor_fd, 5) < 0) {
        perror("ERROR: Fallo en listen");
        exit(EXIT_FAILURE);
    }

    printf("\n[+] Servicio TCP levantado. Escuchando en el puerto %d...\n", puerto);
    
    // Retornamos el descriptor para que el main pueda aceptar a los clientes
    return servidor_fd;
}