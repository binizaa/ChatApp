# ChatApp

ChatApp es una aplicación de chat simple que permite la comunicación entre múltiples clientes conectados a un servidor utilizando sockets TCP/IP.

## Requisitos Previos

Antes de comenzar, asegúrate de tener instalados los siguientes componentes en tu sistema:

- **CMake** (versión 3.10 o superior)
- **GCC** o **Clang** como compilador C++
- **Telnet** (opcional, para probar los clientes)

## Configuración y Compilación

1. **Compila el proyecto**:

    ```bash
    cmake --build build
    ```

    Esto generará un ejecutable llamado `ChatApp` en el directorio `build`.

## Ejecución del Servidor

Para iniciar el servidor de chat:

1. **Ejecuta el servidor**:

    Navega al directorio `build` y ejecuta el servidor especificando el puerto en el que deseas que escuche. Por defecto, en el código proporcionado, el puerto es `1234`.

    ```bash
    cd build
    ./ChatApp 1234
    ```

2. **Servidor en ejecución**:

    Si el servidor se inicia correctamente, verás un mensaje en la consola indicando que el servidor está escuchando en el puerto `1234`.

## Conexión de Clientes con Telnet

Puedes usar Telnet para simular clientes conectándose al servidor:

1. **Abre un terminal nuevo**.

2. **Conéctate al servidor usando Telnet**:

    ```bash
    telnet localhost 1234
    ```

    Si el servidor está en una máquina remota, reemplaza `localhost` con la dirección IP del servidor.

3. **Repite para más clientes**:

    Puedes abrir más terminales y ejecutar el mismo comando Telnet para conectar múltiples clientes al servidor.

## Uso

Una vez conectado, los clientes pueden enviar mensajes que serán retransmitidos a todos los demás clientes conectados al servidor.

- **Enviar un mensaje**: Simplemente escribe tu mensaje en la terminal Telnet y presiona `Enter`.
- **Salir del chat**: Para salir, cierra la sesión Telnet (generalmente con `Ctrl+]`, luego `quit` y `Enter`).