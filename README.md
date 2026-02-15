# ⌨️ Linux Kernel Keylogger (C)

Un **Keylogger simple y educativo** escrito en C para sistemas Linux. Este programa interactúa directamente con el subsistema de entrada del kernel de Linux (`/dev/input/`) para capturar y decodificar eventos de teclado en tiempo real.

> ** AVISO LEGAL / DISCLAIMER**
> Este software ha sido desarrollado **exclusivamente con fines educativos y de aprendizaje** sobre la estructura `input_event` de Linux y el manejo de archivos de dispositivos.
> El autor no se hace responsable del mal uso que se le pueda dar a este código. El uso de keyloggers en sistemas ajenos sin consentimiento es ilegal. Úsalo solo en tu propia máquina.

## Características

* **Autodetección de Hardware:** Escanea automáticamente `/proc/bus/input/devices` para encontrar el teclado conectado, sin necesidad de especificar el `eventX` manualmente.
* **Lectura a Bajo Nivel:** Utiliza `linux/input.h` para leer la estructura `struct input_event` directamente del kernel.
* **Modo Manual:** Permite especificar un dispositivo manualmente si la autodetección falla.
* **Ligero:** Escrito en C puro, sin dependencias externas pesadas.

## Requisitos

* Sistema Operativo: **Linux** (Probado en Ubuntu/Debian).
* Compilador: **GCC**.
* Permisos: **Root / Sudo** (Necesario para leer `/dev/input/`).

## Instalación y Compilación

1.  Clona este repositorio:
    ```bash
    git clone [https://github.com/tu-usuario/linux-keylogger-c.git](https://github.com/tu-usuario/linux-keylogger-c.git)
    cd linux-keylogger-c
    ```

2.  Compila el código fuente:
    ```bash
    gcc -o keylogger keylogger.c
    ```

## Uso

### Modo Automático (Recomendado)
El programa intentará buscar el teclado principal por sí mismo.
```bash
sudo ./keylogger
