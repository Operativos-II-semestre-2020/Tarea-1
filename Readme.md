# Servicios en contenedores 

Los archivos contenidos dentro de este proyecto son el código fuente para la solución de la tarea 1 del curso de Principios de Sistemas Operativos, del Área Académica de Ingeniería en Computadores del Tecnológico de Costa Rica, II Semestre 2020.

La teras consiste en la integración de un contenedor Docker con un servidor escrito en C que es escucha conexiones proveninietes de un cliente, también escrito en C, que se encuentra corriendo fuera del contenedor. El cliente envía imágenes que el servidor deberá procesar y calcular cuál color es predominante ya sea rojo, verde o azul. Para más información del proyecto referirse al documento ServiciosContenedores.pdf.


## Requisitos

El programa se debe ejecutar en una computadora con sistema operativo Linux con el compilador GCC instalado además de tener instalada la herramienta Docker.


## Importante!
El código ha sido probado para imágenes de formato jpg y jpeg. Pueden ocurrir problemas con otro tipo de formato imágenes.

## Compilación y ejecución del programa
Primeros nos debemos abrir una consola y ubicarnos en la carpeta Tarea.
Para poder construir la imagen del contenedor se debe correr el siguiente comando:

```bash
docker build -t tarea1 .
 ```

 Para poner a correr el contenedor ingresamos en el comando:

 ```bash
docker run -it -p 8585:8585 -v $(pwd)/Resultados:/home/Tarea1/Resultados tarea1
 ```

 Con esto el contenedor iniciará el servidor y se pondrá a la escucha de nuevas conexiones.

 El cliente debe ejecutarse bajo un sistema operativo Linux.
 Para compilar el cliente utilizamos el siguiente comando: 


 ```bash
gcc -o cliente Cliente.c -w
 ```

 Para ejecutar el programa cliente utilizamo el comando:

```bash
./cliente <ip>
 ```

 Se debe remplazar "ip" por la IP del servidor a la cual se quiere conectar. Por ejemplo:


```bash
./cliente 172.17.0.2
 ```
 Para una información más detallada de los comandos y sobre como utilizar el progama revisar la sección Instrucciones del documento ServiciosContenedores.pdf.

 Otra opción  para realizar la compilación y ejecución del servidor es por medio del archivo Makefile que se encuentra en la carpeta del proyecto mediante el siguente comando: 

 ```bash
make BuildRunServer
 ```

Y para compilar el cliente:
 ```bash
make BuildClient
 ```
 Sim embargo para ejecutar el cliente se hace de la forma antes mencionada

 ```bash
./cliente <ip>
 ```

