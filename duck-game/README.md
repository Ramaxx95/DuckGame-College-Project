# Trabajo Practico - DuckGame (Grupo 19)

En esta rama se encuentra la entrega de TP grupal del grupo 19, catedra Veiga

**Integrantes:**
* Irala Elbio
* Lozano Ramiro
* Pared Ariel Cristian

**Correctores:**
* Encinoza Nathalia
* Erlich Ivan

## Indice

1. [Instrucciones de uso](#Instrucciones-de-uso)
   
    a. [Requisitos minimos](#Requisitos-minimos)
   
    b. [Correrlo en otros SO](#Requisitos-minimos)
   
    c. [Compilar programa](#Compilar-programa)
   
    d. [Correr programa](#Correr-programa)
   
    e. [Programa Cliente](#Programa-Cliente)
   
3. [Features logradas para esta entrega](#Features-logradas-para-esta-entrega)

## Instrucciones de uso

### Requisitos minimos

El siguiente software fue desarrollado y probado en distribuciones Linux (Ubuntu 20.04). Se recomienda usar dicha distribucion o similares.
Antes de compilar el programa se tiene que tener instalado los siguientes componentes:
* SDL2: `sudo apt-get install libsdl2-dev`
* SDL2_Image: `sudo apt-get install libsdl2-image-dev`
* SDL2_ttf: `sudo apt-get install libsdl2-ttf-dev`
* Qt: `sudo apt install -y qtcreator qtbase5-dev qt5-qmake cmake`
* Qt-wayland: `sudo apt install qtwayland5`
* make y cmake

`Estos componentes pueden instalarse manualmente o corriendo el script "installer.sh"`

### Correrlo en otros SO

Si no contas con una distribuciones Linux (Ubuntu 20.04), dejamos un dockerfile en el repositorio para que puedas levantarlo en cualquier SO.

Para poder correrlo, es necesario que tengas instalado [docker](https://docs.docker.com/engine/install/) previamente. Luego, tenes que correr los siguientes comandos para prepara el ambiente.


Para buildear corre el siguiente comando desde el root del juego:
```bash
docker build -t duckgame-installer .
```

Necesitamos ahora establecer una conexion entre contenedores, para esto creamos una nueva conexion:

```bash
docker network create my_network
```

`Importante:` Tal vez necesites darle permisos en tu SO para que el contenedor ejecute con el DISPLAY de tu SO. Para esto, podes correr: `xhost +local:docker` en un terminal.

Luego, podemos ingresar a la maquina de docker. Para esto te dejo dos comandos, uno para el servidor y el otro para el cliente que pide algunas cosas mas:

Server
```bash
docker run -it --name taller_server --network my_network duckgame-installer
```

Client
```bash
docker run -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix --name taller_client --network my_network --device /dev/snd duckgame-installer
```

Cuando estes dentro de la maquina de docker, vas a tener que hacer unos pasos mas (Ya casi!)

Tenes que buscar la carpeta donde esta instalado el juego, para eso vas a tener que hacer:

```
cd ..
cd .. 
cd game
```

Y listo! ya podes seguir los pasos de como correr el juego del lado server y client!

**Friendly-Reminder**: Si corres con docker, para levantar el cliente tenes que indicarle el nombre del server. Para los ejemplos que te deje tienen que ser `taller_server`

```bash
./build/taller_client taller_server 8080
```

### Compilar programa

Dentro de la carpeta principal de este repositorio correr `make compile-debug` para crear los binarios del programa. Estos se situaran en la carpeta "/build" dentro de la carpeta principal.

### Correr programa

Este software consiste de 2 programas: un Servidor y un Cliente.
Lo primero que se debe hacer es levantar un servidor ejecutando el binario "taller_server" junto con el argumento "PORT", el cual va a ser el puerto por el que oiga este servidor.

Luego, se levanta uno o mas clientes ejecutando el binario "taller_client" junto con sus argumentos "SERVER_IP" y "PORT", el primero seria la direccion ip del servidor con el que se quiere conectar (este mismo se crea en localhost por default) y el segundo corresponde al puerto de servidor al cual conectarse.

> Si al tratar de levantar un Cliente le sale el siguiente error: 
>
> `QSocketNotifier: Can only be used with threads started with QThread
> ./build/taller_client: symbol lookup error: /snap/core20/current/lib/x86_64-linux-gnu/libpthread.so.0: undefined symbol:
> __libc_pthread_init, version GLIBC_PRIVATE`
>
> Puede solucionarlo corriendo el siguiente comando en consola:
>
> 	`unset GTK_PATH`

### Programa Cliente

Para conocer como usar el programa, consultar el Manual de Usuario en la carpeta **docs**

## Features logradas para esta entrega
- [x] Lobby con posibilidad de crear o unirse a una partida ya creada
- [x] Seleccionar mapa de la partida
- [x] Lector de archivos de mapa
- [x] Movimiento del pato (derecha, izquierda y saltar)
- [x] Fisicas (colisiones con pisos, paredes y techos; gravedad empuja hacia abajo al pato)
- [x] Protocolo de lobby completo 
- [x] Protocolo de juego funcional abierto a expansion
- [x] Animacion del pato
- [x] Patos de distintos colores para cada jugador
- [x] Soporte para 4 jugadores en simultaneo
- [x] Camara dinamica
- [x] Archivo de configuracion
- [x] Sonido (musica + efectos de sonido)
- [x] Logica de partida (distribucion de puntos a los jugadores, declaracion de ganador, finalizar partida)
- [x] Uso de armas de fuego
- [x] Equipar armamento a los patos (armadura + casco)
- [x] Apuntar para arriba
- [x] Spawn de armas
- [x] Agarrar armas/equipamiento
- [x] UI (puntuacion y vida de jugador)

## Features que no llegamos a implementar
- [ ] Uso de granadas
- [ ] Animaciones (armas, explosiones)
- [ ] Cajas rompibles
- [ ] Dos jugadores dentro de un mismo cliente
